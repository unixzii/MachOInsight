import Foundation

class Navigator: NSObject {
    
    private var sidebar: MainSidebarViewController!
    private var mainViewController: MainViewController!
    private var isConnected: Bool = false
    
    private var indexOfCurrentPage = -1
    
    func connect(withSidebar sidebar: MainSidebarViewController,
                 mainViewController: MainViewController) {
        guard !isConnected else {
            fatalError("The navigator has already been connected")
        }
        
        self.sidebar = sidebar
        self.mainViewController = mainViewController
        
        sidebar.selectionChangeHandler = { [unowned self] in
            self.navigateToPage(at: $0)
        }
        
        isConnected = true
    }
    
    func navigateToPage(at index: Int) {
        guard isConnected else {
            fatalError("The navigator is not connected")
        }
        
        guard indexOfCurrentPage != index else {
            return
        }
        
        indexOfCurrentPage = index
        
        let viewControllerType = DocumentPageProvider.pages[index].viewControllerType
        let viewController = viewControllerType.create()
        mainViewController.replaceContent(viewController)
        
        sidebar.selectItem(at: index)
    }
    
}
