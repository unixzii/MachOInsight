import Cocoa

class SelectableTextField: NSTextField {

    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        commonInit()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        commonInit()
    }
    
    private func commonInit() {
        isSelectable = true
        
        NotificationCenter.default.addObserver(
            self, selector: #selector(handleTextViewSelectionChange(note:)),
            name: NSTextView.didChangeSelectionNotification, object: nil
        )
    }
    
    @objc private func handleTextViewSelectionChange(note: Notification) {
        guard let textView = note.object as? NSTextView else {
            return
        }
        if textView === self.currentEditor() {
            self.backgroundColor = .windowBackgroundColor
            self.drawsBackground = true
        } else {
            self.backgroundColor = .clear
            self.drawsBackground = false
        }
    }
    
}
