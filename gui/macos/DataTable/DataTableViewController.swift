import Cocoa

fileprivate let cellIdentifier = NSUserInterfaceItemIdentifier("Cell")

class DataTableViewController: NSViewController {
    
    var source: DataTableSource? {
        didSet {
            reloadColumns()
            outlineView.reloadData()
        }
    }
    
    @IBOutlet private var outlineView: NSOutlineView!
    
    convenience init() {
        self.init(nibName: "DataTable", bundle: nil)
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
    
    private func tableColumnIdentifier(for index: Int) -> NSUserInterfaceItemIdentifier {
        return .init("Column\(index)")
    }
    
}

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
    
    func outlineView(_ outlineView: NSOutlineView, child index: Int, ofItem item: Any?) -> Any {
        guard let source = self.source else {
            fatalError()
        }
        if let item = item {
            return source.child(at: index, ofItem: item)
        }
        return source.item(at: index)
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
            for i in 0..<source.columns.count {
                if tableColumn.identifier == tableColumnIdentifier(for: i) {
                    return i
                }
            }
            fatalError()
        }()
        let content = source.displayContent(for: item, column: columnIndex)
        
        switch content {
        case .text(let string):
            let cell = outlineView.makeView(withIdentifier: cellIdentifier, owner: nil) as! NSTableCellView
            cell.textField?.stringValue = string
            return cell
        }
        
        return nil
    }
    
}
