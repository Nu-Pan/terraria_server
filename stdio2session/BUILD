load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

pkg_tar(
    name = "pkg",
    package_dir = "/opt/local/s2s",
    srcs = [
        ":s2s-read",
        ":s2s-write",
        ":s2s-vt",
        ":s2s-server",
    ],
    mode = "0755",
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "s2s-read",
    srcs = glob(["stdio2session-read.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
)

cc_binary(
    name = "s2s-write",
    srcs = glob(["stdio2session-write.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
)

cc_binary(
    name = "s2s-vt",
    srcs = glob(["stdio2session-vt.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "s2s-server",
    srcs = glob(["stdio2session-server.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "s2s-test-target",
    srcs = glob(["test-target.cpp"])
)
