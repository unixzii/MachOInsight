load("@rules_cc//cc:defs.bzl", "objc_library")
load("@build_bazel_rules_apple//apple:macos.bzl", "macos_dylib", "macos_application")

objc_library(
    name = "common",
    srcs = glob(["**/*.m", "**/*.mm", "**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    module_name = "MachOInsightCommon",
    deps = ["//core:core"],
    visibility = ["//gui:__pkg__"],
)