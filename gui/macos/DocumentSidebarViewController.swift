import Cocoa

class DocumentSidebarViewController: NSViewController {

    @IBOutlet var tableView: NSTableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
}

extension DocumentSidebarViewController: NSTableViewDataSource {
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return DocumentPageProvider.pages.count
    }
    
}

extension DocumentSidebarViewController: NSTableViewDelegate {
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        guard let cell = tableView.makeView(withIdentifier: .init("Cell"), owner: nil) as? NSTableCellView else {
            return nil
        }
        let page = DocumentPageProvider.pages[row]
        cell.textField?.stringValue = page.title
        cell.imageView?.image = .init(systemSymbolName: page.iconName, accessibilityDescription: page.title)
        return cell
    }
    
}
