load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "call",
    srcs = glob(["stdio2session-call.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "interpreter",
    srcs = glob(["stdio2session-interpreter.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "peek",
    srcs = glob(["stdio2session-peek.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "server",
    srcs = glob(["stdio2session-server.cpp"]),
    deps = [
        "//stdio2session/module",
    ],
    linkopts = ["-pthread"],
)

cc_binary(
    name = "test-target",
    srcs = glob(["test-target.cpp"])
)
