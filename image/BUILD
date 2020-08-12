load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@io_bazel_rules_docker//container:container.bzl", "container_image", "container_push")

pkg_tar(
    name = "pkg",
    deps = [
        "//terraria_server:pkg",
        "//stdio2session:pkg",
        "//scripts:pkg",
        "//cron:pkg",
    ],
)

container_image(
    name = "image",
    base = "@terraria-server-base//image",
    tars = [":pkg"],
    cmd = "run_terraria_server.sh",
    ports = ["7777/udp", "7777/tcp"],    
)

container_push(
    name = "push",
    image = ":image",
    format = "Docker",
    registry = "index.docker.io",
    repository = "nupan/terraria-server",
    tag = "dev",
)

genrule(
    name = "test",
    srcs = [":pkg"],
    outs = ["test.log"],
    cmd = "tar xf $< -C / > $@ 2>&1",
)
