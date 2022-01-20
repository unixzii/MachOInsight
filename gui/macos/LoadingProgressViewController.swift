import Cocoa

class LoadingProgressViewController: NSViewController {

    @IBOutlet var statusLabel: NSTextField!
    @IBOutlet var progressBar: NSProgressIndicator!
    
    func updateProgress(_ value: Double, with status: String) {
        progressBar.doubleValue = value * 100
        statusLabel.stringValue = status
    }
    
}
