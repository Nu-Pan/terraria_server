load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "module",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    visibility = ["//stdio2session:__pkg__"],
)
