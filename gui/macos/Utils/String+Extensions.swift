import Foundation

extension String {
    
    init<I: BinaryInteger>(hexStringWithValue value: I, uppercase: Bool, paddingTo length: Int? = nil) {
        let hexString = String(value, radix: 16, uppercase: uppercase)
        guard let length = length, hexString.count < length else {
            self.init(hexString)
            return
        }
        self.init(String(repeating: "0", count: length - hexString.count) + hexString)
    }
    
}
