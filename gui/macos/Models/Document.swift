import Cocoa
import MachOInsightCommon

class Document: NSDocument {
    
    private var coreLoader: MISLoader!

    override func makeWindowControllers() {
        if let windowController = NSStoryboard.ensuredMain.instantiateController(
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
        coreLoader = loader
    }

    override class var autosavesInPlace: Bool {
        return true
    }
    
    func loadEssentialData(
        progressHandler: @escaping (_ progress: Double, _ status: String) -> Void,
        completionHandler: @escaping (_ error: Error?) -> Void
    ) {
        // TODO: fake it till you make it.
        progressHandler(0.4, "Fake loading status")
        DispatchQueue.main.asyncAfter(deadline: .now() + .milliseconds(100)) {
            completionHandler(nil)
        }
    }
    
    func withLoader(_ block: (MISLoader) -> Void) {
        // TODO: add concurrency control.
        block(coreLoader)
    }
    
    func withCurrentBinary(_ block: (MISMachOBinary) -> Void) {
        // TODO: add universal binary supports.
        withLoader { loader in
            block(loader.machOBinary(at: 0))
        }
    }

}
