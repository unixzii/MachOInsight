import Cocoa

class MainViewController: NSViewController {
    
    private(set) var contentViewController: NSViewController?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // TODO: here we only add a hard-coded view controller for test.
        switchTo(DependenciesViewController())
    }
    
    private func switchTo(_ viewController: NSViewController) {
        if let contentViewController = self.contentViewController {
            contentViewController.view.removeFromSuperview()
            contentViewController.removeFromParent()
        }
        
        self.contentViewController = viewController
        addChild(viewController)
        
        let contentView = viewController.view
        contentView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(contentView)
        NSLayoutConstraint.activate([
            contentView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            contentView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            contentView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            contentView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
        ])
    }
    
}
