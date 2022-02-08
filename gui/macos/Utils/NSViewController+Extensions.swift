import Cocoa

extension NSViewController {
    
    var document: Document? {
        return view.window?.windowController?.document as? Document
    }
    
}

extension NSWindowController {
    
    var appDocument: Document? {
        return contentViewController?.document
    }
    
}
