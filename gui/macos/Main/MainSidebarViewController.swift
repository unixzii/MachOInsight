import Cocoa

class MainSidebarViewController: NSViewController {

    @IBOutlet private var tableView: NSTableView!
    
    var selectionChangeHandler: ((Int) -> ())?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
    func selectItem(at index: Int) {
        tableView.selectRowIndexes(.init(integer: index), byExtendingSelection: false)
        
        // Once some items in the table view is selected, the selection must not
        // be empty in the future.
        tableView.allowsEmptySelection = false
    }
    
}

extension MainSidebarViewController: NSTableViewDataSource {
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return DocumentPageProvider.pages.count
    }
    
}

extension MainSidebarViewController: NSTableViewDelegate {
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        guard let cell = tableView.makeView(withIdentifier: .init("Cell"), owner: nil) as? NSTableCellView else {
            return nil
        }
        let page = DocumentPageProvider.pages[row]
        cell.textField?.stringValue = page.title
        cell.imageView?.image = .init(systemSymbolName: page.iconName, accessibilityDescription: page.title)
        return cell
    }
    
    func tableViewSelectionDidChange(_ notification: Notification) {
        selectionChangeHandler?(tableView.selectedRow)
    }
    
}
