load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

cmake(
    name = "capstone",
    cache_entries = {
        "CAPSTONE_INSTALL": "ON",
        "CAPSTONE_ARCHITECTURE_DEFAULT": "OFF",
        "CAPSTONE_ARM64_SUPPORT": "ON",
        "CAPSTONE_X86_SUPPORT": "ON",
    },
    lib_source = "@capstone//:all_srcs",
    out_static_libs = ["libcapstone.a"],
)

cc_library(
    name = "core",
    srcs = glob(["**/*.cc"]),
    hdrs = glob(["**/*.h"]),
    copts = ["-std=c++17"],
    deps = ["@glog//:glog", ":capstone"],
    visibility = ["//visibility:public"],
)