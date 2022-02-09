import Cocoa

fileprivate class _PickerControllerDelegateForwarder: ListPickerControllerDelegate {
    
    var cancelHandler: (() -> ())?
    var selectHandler: ((Int) -> ())?
    
    func listPickerControllerDidCancel(_ sender: ListPickerController) {
        cancelHandler?()
    }
    
    func listPickerController(_ sender: ListPickerController, didSelectItemAt index: Int) {
        selectHandler?(index)
    }
    
}

class MainWindowController: NSWindowController {

    private var navigator: Navigator!
    
    override var document: AnyObject? {
        didSet {
            guard document != nil else { return }
            handleDocumentChange()
        }
    }
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        // Setup the navigator.
        let splitViewController = self.contentViewController as! NSSplitViewController
        let sidebarViewController = splitViewController.splitViewItems[0].viewController as! MainSidebarViewController
        let mainViewController = splitViewController.splitViewItems[1].viewController as! MainViewController
        navigator = Navigator()
        navigator.connect(withSidebar: sidebarViewController, mainViewController: mainViewController)
    }
    
    private func handleDocumentChange() {
        promptToSelectArchIfNeeded {
            self.loadDocumentAndReportProgress()
        }
    }
    
    private func promptToSelectArchIfNeeded(completionHandler: @escaping () -> ()) {
        let appDocument = self.appDocument!
        let archs: [String] = appDocument.archs.map({ archType in
            switch archType {
            case .X86_64:
                return "x86 (64 bits)"
            case .ARM64:
                return "AArch64"
            default:
                return "(Unknown)"
            }
        })
        
        guard archs.count > 1 else {
            completionHandler()
            return
        }
        
        // Ensure the window has displayed.
        OperationQueue.main.addOperation {
            let parentViewController = self.contentViewController!
            
            let picker = ListPickerController()
            picker.title = "Select an arch to load:"
            picker.items = archs.map { .plainText(text: $0) }
            
            let delegate = _PickerControllerDelegateForwarder()
            delegate.cancelHandler = {
                parentViewController.dismiss(picker)
                self.close()
                
                // Break the retain cycle.
                delegate.cancelHandler = nil
            }
            delegate.selectHandler = { [unowned delegate] in
                appDocument.indexOfCurrentArch = UInt($0)
                parentViewController.dismiss(picker)
                
                completionHandler()
                
                delegate.cancelHandler = nil
            }
            picker.delegate = delegate
            
            parentViewController.presentAsSheet(picker)
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
            // TODO: error handling...
            OperationQueue.main.addOperation {
                view.removeFromSuperview()
                viewController.removeFromParent()
                
                window.titleVisibility = .visible
                window.titlebarAppearsTransparent = false
                window.toolbar?.isVisible = true
                
                self.navigator.navigateToPage(at: 0)
            }
        }
    }

}

extension MainWindowController: NSToolbarDelegate {
    
    func toolbarDefaultItemIdentifiers(_ toolbar: NSToolbar) -> [NSToolbarItem.Identifier] {
        return [.toggleSidebar]
    }
    
}
