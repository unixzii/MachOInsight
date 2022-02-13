import Cocoa
import MachOInsightCommon

fileprivate class _SidebarDataTableSource: PlainDataTableSource {
    
    let items: [MISVariant]
    
    private var titleCache = [ObjectIdentifier : String]()
    
    init(items: [MISVariant]) {
        self.items = items
    }
    
    override var columns: [String] {
        return ["Name"]
    }
    
    override var numberOfRows: Int {
        return items.count
    }
    
    override func item(at index: Int) -> Any {
        return items[index]
    }
    
    override func displayContent(for item: Any, column: Int) -> DataTableDisplayContent {
        let title: String = {
            let id = ObjectIdentifier(item as AnyObject)
            if let title = titleCache[id] {
                return title
            }
            
            let variant = item as! MISVariant
            let title = variant.toDictionary()?["name"]?.toString() ?? "Unknown"
            titleCache[id] = title
            return title
        }()
        
        return .text(title)
    }
    
}

fileprivate class _ContentDataTableSource: PlainDataTableSource {
    
    let items: [MISVariant]
    
    init(items: [MISVariant]) {
        self.items = items
    }
    
    override var columns: [String] {
        return ["Field Description", "Address", "Value"]
    }
    
    override var numberOfRows: Int {
        return items.count
    }
    
    override func item(at index: Int) -> Any {
        return items[index]
    }
    
    override func displayContent(for item: Any, column: Int) -> DataTableDisplayContent {
        guard let field = (item as! MISVariant).toDictionary() else {
            return .text("???")
        }
        switch column {
        case 0:
            if let description = field["name"]?.toString() {
                return .text(description)
            }
        case 1:
            if let address = field["addr"]?.toUnsignedLongLong() {
                return .code(.init(hexStringWithValue: address, uppercase: true, paddingTo: 8))
            }
        case 2:
            if let value = field["value"]?.description {
                return .text(value)
            }
        default:
            fatalError("Unknown column")
        }
        return .text("???")
    }
    
}

class LoadCommandsViewController: NSViewController {

    @IBOutlet var sidebarContainerView: NSView!
    @IBOutlet var contentContainerView: NSView!
    
    private var sidebarViewController: DataTableViewController!
    private var contentViewController: DataTableViewController!
    
    convenience init() {
        self.init(nibName: "LoadCommandsViewController", bundle: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        sidebarViewController = .init()
        sidebarViewController.showsHeaderView = false
        sidebarViewController.usesAlternatingRowBackgroundColors = false
        sidebarViewController.selectionChangeHandler = { [unowned self] in
            self.handleSidebarSelectionChange(with: $0)
        }
        addChild(sidebarViewController)
        sidebarContainerView.addFilledSubview(sidebarViewController.view)
        
        contentViewController = .init()
        addChild(contentViewController)
        contentContainerView.addFilledSubview(contentViewController.view)
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        populateData()
    }
    
    func populateData() {
        document?.withCurrentBinary({ binary in
            sidebarViewController.source = _SidebarDataTableSource(items: binary.dumpedLoadCommands)
        })
    }
    
    private func handleSidebarSelectionChange(with selectedItem: Any?) {
        guard let selectedItem = selectedItem as? MISVariant else {
            contentViewController.source = nil
            return
        }
        let fields = selectedItem.toDictionary()?["fields"]?.toArray() ?? []
        contentViewController.source = _ContentDataTableSource(items: fields)
    }
    
}

extension LoadCommandsViewController: DocumentPageConstructible {
    
    static func create() -> NSViewController {
        return LoadCommandsViewController()
    }
    
}
