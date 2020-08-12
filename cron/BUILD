load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

pkg_tar(
    name = "pkg",
    package_dir = "/etc/cron.d",
    srcs = [
        "terraria_server",
    ],
    mode = "0644",
    visibility = ["//visibility:public"],
)
