import Cocoa

@objc protocol ListPickerControllerDelegate {
    
    @objc optional func listPickerControllerDidCancel(_ sender: ListPickerController)
    
    @objc optional func listPickerController(_ sender: ListPickerController, didSelectItemAt index: Int)
    
}

struct ListPickerItem {
    
    fileprivate let string: String
    
    static func plainText(text: String) -> Self {
        return .init(string: text)
    }
    
}

class ListPickerController: NSViewController {
    
    @IBOutlet private var titleLabel: NSTextField!
    @IBOutlet private var tableView: NSTableView!
    
    weak var delegate: ListPickerControllerDelegate?
    var items: [ListPickerItem]? {
        didSet {
            guard isViewLoaded else { return }
            tableView.reloadData()
        }
    }
    
    override var title: String? {
        didSet {
            guard isViewLoaded else { return }
            titleLabel.stringValue = title ?? ""
        }
    }
    
    convenience init() {
        self.init(nibName: "ListPickerController", bundle: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        titleLabel.stringValue = title ?? ""
        tableView.doubleAction = #selector(done(_:))
    }
    
    @IBAction func cancel(_ sender: Any) {
        delegate?.listPickerControllerDidCancel?(self)
    }
    
    @IBAction func done(_ sender: Any) {
        delegate?.listPickerController?(self, didSelectItemAt: tableView.selectedRow)
    }
    
}

extension ListPickerController: NSTableViewDelegate {
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        let cell = tableView.makeView(withIdentifier: .init("Cell"), owner: nil) as? NSTableCellView
        cell?.textField?.stringValue = items![row].string
        return cell
    }
    
}

extension ListPickerController: NSTableViewDataSource {
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return items?.count ?? 0
    }
    
}
