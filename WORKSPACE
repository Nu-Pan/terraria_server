load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Download the rules_docker repository at release v0.14.4
http_archive(
    name = "io_bazel_rules_docker",
    sha256 = "4521794f0fba2e20f3bf15846ab5e01d5332e587e9ce81629c7f96c793bb7036",
    strip_prefix = "rules_docker-0.14.4",
    urls = ["https://github.com/bazelbuild/rules_docker/releases/download/v0.14.4/rules_docker-v0.14.4.tar.gz"],
)

load("@io_bazel_rules_docker//toolchains/docker:toolchain.bzl",
    docker_toolchain_configure="toolchain_configure"
)

# Configure the docker toolchain.
#docker_toolchain_configure(
#  name = "docker_config",
#  # Path to the directory which has a custom docker client config.json with
#  # authentication credentials for registry.gitlab.com (used in this example).
#  client_config="`pwd`/.docker",
#)

load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)
container_repositories()

load("@io_bazel_rules_docker//repositories:deps.bzl", container_deps = "deps")

container_deps()

load("@io_bazel_rules_docker//repositories:pip_repositories.bzl", "pip_deps")

pip_deps()

load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_pull",
)

container_pull(
  name = "official_ubuntu",
  registry = "index.docker.io",
  repository = "library/ubuntu",
  tag = "20.04",
  digest = "sha256:60f560e52264ed1cb7829a0d59b1ee7740d7580e0eb293aca2d722136edb1e24",
)
