import Cocoa
import MachOInsightCommon

class Document: NSDocument {

    override func makeWindowControllers() {
        let storyboard = NSStoryboard(name: NSStoryboard.Name("Main"), bundle: nil)
        if let windowController = storyboard.instantiateController(
            withIdentifier: NSStoryboard.SceneIdentifier("Document")
        ) as? NSWindowController {
            addWindowController(windowController)
        }
    }

    override func data(ofType typeName: String) throws -> Data {
        throw NSError(domain: NSOSStatusErrorDomain, code: unimpErr, userInfo: nil)
    }

    override func read(from url: URL, ofType typeName: String) throws {
        let loader = MISLoader(executablePath: url.path)
        guard loader.tryLoad() else {
            throw NSError(domain: "me.cyandev.MacOInsightMac", code: paramErr, userInfo: nil)
        }
    }

    override class var autosavesInPlace: Bool {
        return true
    }

}
