load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "build_bazel_rules_apple",
    sha256 = "77e8bf6fda706f420a55874ae6ee4df0c9d95da6c7838228b26910fc82eea5a2",
    url = "https://github.com/bazelbuild/rules_apple/releases/download/0.32.0/rules_apple.0.32.0.tar.gz",
)

http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "rules_foreign_cc-e24d9cecfe4cadc496fe0c128ef5f8399a4e24a5",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/e24d9cecfe4cadc496fe0c128ef5f8399a4e24a5.tar.gz",
)

load(
    "@build_bazel_rules_apple//apple:repositories.bzl",
    "apple_rules_dependencies",
)

apple_rules_dependencies()

load(
    "@build_bazel_rules_swift//swift:repositories.bzl",
    "swift_rules_dependencies",
)

swift_rules_dependencies()

load(
    "@build_bazel_rules_swift//swift:extras.bzl",
    "swift_rules_extra_dependencies",
)

swift_rules_extra_dependencies()

load(
    "@build_bazel_apple_support//lib:repositories.bzl",
    "apple_support_dependencies",
)

apple_support_dependencies()

load(
    "@rules_foreign_cc//foreign_cc:repositories.bzl",
    "rules_foreign_cc_dependencies"
)

rules_foreign_cc_dependencies()

# 3rd-party deps

load(":deps.bzl", "third_party_library_dependencies")

third_party_library_dependencies()
