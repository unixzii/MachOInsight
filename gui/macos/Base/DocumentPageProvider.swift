import Foundation
import AppKit

protocol DocumentPageConstructible {
    
    static func create() -> NSViewController
    
}

struct DocumentPage {
    let title: String
    let iconName: String
    let viewControllerType: DocumentPageConstructible.Type
}

enum DocumentPageProvider {
    
    static var pages: [DocumentPage] {
        return [
//            .init(title: "Overview", iconName: "info.circle"),
            .init(title: "Load Commands", iconName: "list.bullet",
                  viewControllerType: LoadCommandsViewController.self),
            .init(title: "Dependencies", iconName: "books.vertical",
                  viewControllerType: DependenciesViewController.self),
//            .init(title: "Segments", iconName: "doc"),
//            .init(title: "Symbols", iconName: "character.book.closed"),
//            .init(title: "Objective-C Insight", iconName: "gearshape"),
//            .init(title: "Swift Insight", iconName: "swift"),
        ]
    }
    
}
