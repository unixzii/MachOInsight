import Cocoa
import MachOInsightCommon

fileprivate struct _DependencyItem {
    let path: String
    let currentVersion: String
    let compatibilityVersion: String
}

fileprivate func humanReadableString(from version: UInt) -> String {
    let major = (version >> 16) & 0xffff
    let minor = (version >> 8) & 0xff
    let patch = version & 0xff
    return "\(major).\(minor).\(patch)"
}

fileprivate class _DependenciesDataTableSource: PlainDataTableSource {
    
    let items: [_DependencyItem]
    
    init(items: [_DependencyItem]) {
        self.items = items
    }
    
    override var columns: [String] {
        return ["Path", "Current Version", "Compatibility Version"]
    }
    
    override var numberOfRows: Int {
        return items.count
    }
    
    override func item(at index: Int) -> Any {
        return items[index]
    }
    
    override func displayContent(for item: Any, column: Int) -> DataTableDisplayContent {
        guard let typedItem = item as? _DependencyItem else {
            fatalError()
        }
        
        if column == 0 {
            return .text(typedItem.path)
        } else if column == 1 {
            return .text(typedItem.currentVersion)
        } else if column == 2 {
            return .text(typedItem.compatibilityVersion)
        }
        
        fatalError("Unknown column")
    }
    
}

class DependenciesViewController: NSViewController {
    
    private var dataTableViewController: DataTableViewController!
    
    convenience init() {
        self.init(nibName: "DependenciesViewController", bundle: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        dataTableViewController = .init()
        addChild(dataTableViewController)
        
        let dataTableView = dataTableViewController.view
        dataTableView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(dataTableView)
        NSLayoutConstraint.activate([
            dataTableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            dataTableView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            dataTableView.topAnchor.constraint(equalTo: view.topAnchor),
            dataTableView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        populateData()
    }
    
    func populateData() {
        var items = [_DependencyItem]()
        document?.withCurrentBinary({ binary in
            for i in 0..<binary.numberOfLoadDylibs {
                let dylib = binary.loadDylib(at: i)
                let item = _DependencyItem(
                    path: dylib.name,
                    currentVersion: humanReadableString(from: dylib.currentVersion),
                    compatibilityVersion: humanReadableString(from: dylib.compatibilityVersion)
                )
                items.append(item)
            }
        })
        
        dataTableViewController.source = _DependenciesDataTableSource(items: items)
    }
    
}

extension DependenciesViewController: DocumentPageConstructible {
    
    static func create() -> NSViewController {
        return DependenciesViewController()
    }
    
}
