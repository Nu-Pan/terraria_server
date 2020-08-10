load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

pkg_tar(
    name = "pkg",
    package_dir = "/usr/bin",
    srcs = glob(["*.sh"]),
    mode = "0755",
    visibility = ["//visibility:public"],
)
