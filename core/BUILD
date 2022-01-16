load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "core",
    srcs = glob(["**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    copts = ["-std=c++17"],
    deps = ["@com_github_google_glog//:glog"],
    visibility = ["//visibility:public"],
)