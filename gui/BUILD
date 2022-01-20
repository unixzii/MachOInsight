load("@rules_cc//cc:defs.bzl", "objc_library")
load("@build_bazel_rules_swift//swift:swift.bzl", "swift_library")
load("@build_bazel_rules_apple//apple:macos.bzl", "macos_application")

# macOS target

swift_library(
    name = "macos-main",
    srcs = glob(["macos/**/*.swift"]),
    module_name = "MachOInsightMain",
    deps = ["//gui/common:common"],
)

objc_library(
    name = "macos-resources",
    data = ["macos/Base.lproj/Main.storyboard"],
)

macos_application(
    name = "macos-app",
    bundle_id = "me.cyandev.MacOInsightMac",
    infoplists = ["macos/Info.plist"],
    minimum_os_version = "11.0",
    deps = [
        ":macos-main",
        ":macos-resources"
    ],
)