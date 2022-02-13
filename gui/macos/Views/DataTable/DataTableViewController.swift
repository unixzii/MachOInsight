import Cocoa

fileprivate let cellIdentifier = NSUserInterfaceItemIdentifier("Cell")

class DataTableViewController: NSViewController {
    
    var source: DataTableSource? {
        didSet {
            reloadColumns()
            reloadMenu()
            outlineView.reloadData()
        }
    }
    
    var selectionChangeHandler: ((Any?) -> ())?
    
    var showsHeaderView: Bool {
        get {
            return outlineView.headerView != nil
        }
        set {
            if newValue {
                outlineView.headerView = .init()
            } else {
                outlineView.headerView = nil
            }
        }
    }
    
    var usesAlternatingRowBackgroundColors: Bool {
        get {
            return outlineView.usesAlternatingRowBackgroundColors
        }
        set {
            outlineView.usesAlternatingRowBackgroundColors = newValue
        }
    }
    
    @IBOutlet private var outlineView: NSOutlineView!
    
    convenience init() {
        self.init(nibName: "DataTable", bundle: nil)
    }
    
    override init(nibName nibNameOrNil: NSNib.Name?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
        
        // Eager load view to make sure that setting a property immediately after
        // the view controller is created will take effect.
        let _ = view
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        outlineView.register(.init(nibNamed: "DataTableCell", bundle: nil), forIdentifier: cellIdentifier)
    }
    
    private func reloadColumns() {
        let tableColumns = outlineView.tableColumns
        for tableColumn in tableColumns {
            outlineView.removeTableColumn(tableColumn)
        }
        
        guard let source = self.source else {
            return
        }
        
        for (index, column) in source.columns.enumerated() {
            let tableColumnIdentifier = tableColumnIdentifier(for: index)
            if index == 0 {
                // There must be at least one column in NSOutlineView.
                let tableColumn = outlineView.tableColumns.first!
                tableColumn.identifier = tableColumnIdentifier
                tableColumn.title = column
            } else {
                let tableColumn = NSTableColumn()
                tableColumn.identifier = tableColumnIdentifier
                tableColumn.title = column
                tableColumn.resizingMask = [.userResizingMask]
                outlineView.addTableColumn(tableColumn)
            }
        }
    }
    
    private func reloadMenu() {
        let menu = NSMenu(title: "Context Menu")
        menu.delegate = self
        
        // Add default items.
        menu.addItem(withTitle: "", action: nil, keyEquivalent: "")
        menu.addItem(withTitle: "Copy", action: #selector(copyText(_:)), keyEquivalent: "")
        menu.addItem(withTitle: "Copy All", action: #selector(copyAllText(_:)), keyEquivalent: "")
        
        outlineView.menu = menu
    }
    
    private func tableColumnIdentifier(for index: Int) -> NSUserInterfaceItemIdentifier {
        return .init("Column\(index)")
    }
    
    private func index(of tableColumn: NSTableColumn) -> Int? {
        guard let source = self.source else {
            return nil
        }

        for i in 0..<source.columns.count {
            if tableColumn.identifier == tableColumnIdentifier(for: i) {
                return i
            }
        }
        return nil
    }
    
}

// MARK: - Context Menu

extension DataTableViewController: NSMenuDelegate {
    
    private var clickedText: String? {
        guard let source = self.source else {
            return nil
        }
        guard let item = outlineView.item(atRow: outlineView.clickedRow) else {
            return nil
        }
        let column = index(of: outlineView.tableColumns[outlineView.clickedColumn])!
        let content = source.displayContent(for: item, column: column)
        switch content {
        case .text(let string):
            return string
        case .code(let string):
            return string
        }
        return nil
    }
    
    func menuNeedsUpdate(_ menu: NSMenu) {
        guard let clickedText = clickedText else {
            menu.removeItem(at: 0)
            return
        }
        menu.item(at: 0)?.title = clickedText
    }
    
    @objc private func copyText(_ sender: Any) {
        guard let clickedText = clickedText else {
            return
        }

        let pasteboard = NSPasteboard.general
        pasteboard.clearContents()
        pasteboard.setString(clickedText, forType: .string)
    }
    
    @objc private func copyAllText(_ sender: Any) {
        guard let source = self.source else {
            return
        }
        guard let item = outlineView.item(atRow: outlineView.clickedRow) else {
            return
        }
        
        var strings = [String]()
        for column in 0..<outlineView.tableColumns.count {
            let content = source.displayContent(for: item, column: column)
            switch content {
            case .text(let string):
                strings.append(string)
            case .code(let string):
                strings.append(string)
            }
        }
        
        let string = strings.joined(separator: " ")
        
        let pasteboard = NSPasteboard.general
        pasteboard.clearContents()
        pasteboard.setString(string, forType: .string)
    }
    
}

// MARK: -

extension DataTableViewController: NSOutlineViewDataSource {
    
    func outlineView(_ outlineView: NSOutlineView, numberOfChildrenOfItem item: Any?) -> Int {
        guard let source = self.source else {
            return 0
        }
        if let item = item {
            return source.numberOfChildren(of: item)
        }
        return source.numberOfRows
    }
    
    func outlineView(_ outlineView: NSOutlineView, isItemExpandable item: Any) -> Bool {
        return (source?.numberOfChildren(of: item) ?? 0) > 0
    }
    
    func outlineView(_ outlineView: NSOutlineView, child index: Int, ofItem item: Any?) -> Any {
        guard let source = self.source else {
            fatalError()
        }
        if let item = item {
            return source.child(at: index, ofItem: item)
        }
        return source.item(at: index)
    }
    
    func outlineView(_ outlineView: NSOutlineView, objectValueFor tableColumn: NSTableColumn?, byItem item: Any?) -> Any? {
        return item
    }
    
}

extension DataTableViewController: NSOutlineViewDelegate {
    
    func outlineView(_ outlineView: NSOutlineView, viewFor tableColumn: NSTableColumn?, item: Any) -> NSView? {
        guard let source = self.source else {
            fatalError()
        }
        let columnIndex: Int = {
            guard let tableColumn = tableColumn else {
                return 0
            }
            return index(of: tableColumn)!
        }()
        let content = source.displayContent(for: item, column: columnIndex)
        
        switch content {
        case .text(let string):
            let cell = outlineView.makeView(withIdentifier: cellIdentifier, owner: nil) as! NSTableCellView
            cell.textField?.stringValue = string
            cell.textField?.font = .systemFont(ofSize: 12)
            return cell
        case .code(let string):
            let cell = outlineView.makeView(withIdentifier: cellIdentifier, owner: nil) as! NSTableCellView
            cell.textField?.stringValue = string
            cell.textField?.font = .monospacedSystemFont(ofSize: 12, weight: .regular)
            return cell
        }
    }
    
    func outlineViewSelectionDidChange(_ notification: Notification) {
        if outlineView.numberOfSelectedRows == 0 {
            selectionChangeHandler?(nil)
            return
        }
        
        let selectedItem = outlineView.item(atRow: outlineView.selectedRow)
        selectionChangeHandler?(selectedItem)
    }
    
}
