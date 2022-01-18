import Foundation

struct DocumentPage {
    let title: String
    let iconName: String
}

enum DocumentPageProvider {
    
    static var pages: [DocumentPage] {
        return [
            .init(title: "Overview", iconName: "info.circle"),
            .init(title: "Load Commands", iconName: "list.bullet"),
            .init(title: "Dependencies", iconName: "books.vertical"),
            .init(title: "Segments", iconName: "doc"),
            .init(title: "Symbols", iconName: "character.book.closed"),
            .init(title: "Objective-C Insight", iconName: "gearshape"),
            .init(title: "Swift Insight", iconName: "swift"),
        ]
    }
    
}
