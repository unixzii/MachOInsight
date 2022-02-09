import Cocoa

class LoadCommandsViewController: NSViewController {

    convenience init() {
        self.init(nibName: "LoadCommandsViewController", bundle: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
}

extension LoadCommandsViewController: DocumentPageConstructible {
    
    static func create() -> NSViewController {
        return LoadCommandsViewController()
    }
    
}
