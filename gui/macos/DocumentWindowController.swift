import Cocoa

class DocumentWindowController: NSWindowController {

    override var document: AnyObject? {
        didSet {
            if document != nil {
                loadDocumentAndReportProgress()
            }
        }
    }
    
    private func loadDocumentAndReportProgress() {
        guard let viewController = NSStoryboard.ensuredMain.instantiateController(
            withIdentifier: NSStoryboard.SceneIdentifier("LoadingProgressViewController")
        ) as? LoadingProgressViewController, let window = self.window else {
            return
        }
        
        window.titleVisibility = .hidden
        window.titlebarAppearsTransparent = true
        window.toolbar?.isVisible = false
        
        guard let contentView = window.contentView else {
            return
        }
        
        contentViewController?.addChild(viewController)
        
        let view = viewController.view
        contentView.addSubview(view)
        NSLayoutConstraint.activate([
            view.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),
            view.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            view.topAnchor.constraint(equalTo: contentView.topAnchor),
            view.bottomAnchor.constraint(equalTo: contentView.bottomAnchor),
        ])
        
        let document = self.document as! Document
        document.loadEssentialData { progress, status in
            OperationQueue.main.addOperation {
                viewController.updateProgress(progress, with: status)
            }
        } completionHandler: { error in
            OperationQueue.main.addOperation {
                view.removeFromSuperview()
                viewController.removeFromParent()
                
                window.titleVisibility = .visible
                window.titlebarAppearsTransparent = false
                window.toolbar?.isVisible = true
            }
        }
    }

}

extension DocumentWindowController: NSToolbarDelegate {
    
    func toolbarDefaultItemIdentifiers(_ toolbar: NSToolbar) -> [NSToolbarItem.Identifier] {
        return [.toggleSidebar]
    }
    
}
