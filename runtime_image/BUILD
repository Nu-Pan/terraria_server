load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@io_bazel_rules_docker//container:container.bzl", "container_image", "container_push")

pkg_tar(
    name = "pkg",
    deps = [
        "//terraria_server:pkg",
        "//stdio2session:pkg",
    ],
)

container_image(
    name = "image",
    base = "@official_ubuntu//image",
    tars = [":pkg"],
    #symlinks = {"":""},
    #cmd = "",
    #ports = ""
)

container_push(
    name = "push",
    image = ":image",
    format = "Docker",
    registry = "index.docker.io",
    repository = "nupan/terraria-server",
    tag = "dev",
)
