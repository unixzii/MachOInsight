import Foundation

enum DataTableDisplayContent {
    case text(String)
}

protocol DataTableSource {
    
    // MARK: General
    
    var columns: [String] { get }
    
    var numberOfRows: Int { get }
    func item(at index: Int) -> Any
    
    // MARK: Hierarchical data supports
    
    func numberOfChildren(of item: Any) -> Int
    func child(at index: Int, ofItem item: Any) -> Any
    
    // MARK: Data binding
    
    func displayContent(for item: Any, column: Int) -> DataTableDisplayContent
    
}

class PlainDataTableSource: DataTableSource {
    
    var columns: [String] {
        fatalError("Not implemented")
    }
    
    var numberOfRows: Int {
        fatalError("Not implemented")
    }
    
    func item(at index: Int) -> Any {
        fatalError("Not implemented")
    }
    
    func displayContent(for item: Any, column: Int) -> DataTableDisplayContent {
        fatalError("Not implemented")
    }
    
    // MARK: Default implementations
    
    func numberOfChildren(of item: Any) -> Int {
        return 0
    }
    
    func child(at index: Int, ofItem item: Any) -> Any {
        fatalError()
    }
    
}
