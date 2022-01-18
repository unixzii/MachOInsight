import Cocoa

class DocumentWindowController: NSWindowController {

    override func windowDidLoad() {
        super.windowDidLoad()
    }

}

extension DocumentWindowController: NSToolbarDelegate {
    
    func toolbarDefaultItemIdentifiers(_ toolbar: NSToolbar) -> [NSToolbarItem.Identifier] {
        return [.toggleSidebar]
    }
    
}
