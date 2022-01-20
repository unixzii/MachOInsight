//
//  NSStoryboard+Extensions.swift
//  MachOInsight
//
//  Created by Cyandev on 2022/1/20.
//

import Cocoa

extension NSStoryboard {
    
    static var ensuredMain: NSStoryboard {
        return .init(name: NSStoryboard.Name("Main"), bundle: nil)
    }
    
}
