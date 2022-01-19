def third_party_library_dependencies():
    __WORKDIR = "."

    native.local_repository(
        name = "glog",
        path = __WORKDIR + "/third_party/glog/",
    )

    native.local_repository(
        name = "com_github_gflags_gflags",
        path = __WORKDIR + "/third_party/gflags/",
    )

    _CAPSTONE_ALL_CONTENT = """\
filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
    """

    native.new_local_repository(
        name = "capstone",
        path = __WORKDIR + "/third_party/capstone/",
        build_file_content = _CAPSTONE_ALL_CONTENT,
    )
