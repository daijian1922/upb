load(
    "//bazel:build_defs.bzl",
    "UPB_DEFAULT_COPTS",
    "upb_amalgamation",  # copybara:strip_for_google3
)
load(
    "//bazel:upb_proto_library.bzl",
    "upb_fasttable_enabled",
    "upb_proto_library",
    "upb_proto_library_copts",
    "upb_proto_reflection_library",
)

licenses(["notice"])

exports_files([
    "LICENSE",
    "build_defs",
])

config_setting(
    name = "windows",
    constraint_values = ["@bazel_tools//platforms:windows"],
)

upb_fasttable_enabled(
    name = "fasttable_enabled",
    build_setting_default = False,
    visibility = ["//visibility:public"],
)

config_setting(
    name = "fasttable_enabled_setting",
    flag_values = {"//:fasttable_enabled": "true"},
)

upb_proto_library_copts(
    name = "upb_proto_library_copts__for_generated_code_only_do_not_use",
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
)

# Public C/C++ libraries #######################################################

cc_library(
    name = "port",
    copts = UPB_DEFAULT_COPTS,
    textual_hdrs = [
        "upb/port_def.inc",
        "upb/port_undef.inc",
    ],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "upb",
    srcs = [
        "upb/decode.c",
        "upb/decode.int.h",
        "upb/encode.c",
        "upb/msg.c",
        "upb/msg.h",
        "upb/table.c",
        "upb/table.int.h",
        "upb/upb.c",
        "upb/upb.int.h",
    ],
    hdrs = [
        "upb/decode.h",
        "upb/encode.h",
        "upb/upb.h",
        "upb/upb.hpp",
    ],
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":fastdecode",
        ":port",
        "//third_party/wyhash",
    ],
)

cc_library(
    name = "fastdecode",
    srcs = [
        "upb/decode.int.h",
        "upb/decode_fast.c",
        "upb/decode_fast.h",
        "upb/msg.h",
        "upb/upb.int.h",
    ],
    copts = UPB_DEFAULT_COPTS,
    deps = [
        ":port",
        ":table",
    ],
)

# Common support routines used by generated code.  This library has no
# implementation, but depends on :upb and exposes a few more hdrs.
#
# This is public only because we have no way of visibility-limiting it to
# upb_proto_library() only.  This interface is not stable and by using it you
# give up any backward compatibility guarantees.
cc_library(
    name = "generated_code_support__only_for_generated_code_do_not_use__i_give_permission_to_break_me",
    hdrs = [
        "upb/decode_fast.h",
        "upb/msg.h",
        "upb/port_def.inc",
        "upb/port_undef.inc",
    ],
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":table",
        ":upb",
    ],
)

upb_proto_library(
    name = "descriptor_upb_proto",
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:descriptor_proto"],
)

upb_proto_reflection_library(
    name = "descriptor_upb_proto_reflection",
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:descriptor_proto"],
)

cc_library(
    name = "reflection",
    srcs = [
        "upb/def.c",
        "upb/msg.h",
        "upb/reflection.c",
    ],
    hdrs = [
        "upb/def.h",
        "upb/def.hpp",
        "upb/reflection.h",
        "upb/reflection.hpp",
    ],
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":descriptor_upb_proto",
        ":port",
        ":table",
        ":upb",
    ],
)

cc_library(
    name = "textformat",
    srcs = [
        "upb/text_encode.c",
    ],
    hdrs = [
        "upb/text_encode.h",
    ],
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":port",
        ":reflection",
    ],
)

cc_library(
    name = "json",
    srcs = [
        "upb/json_decode.c",
        "upb/json_encode.c",
    ],
    hdrs = [
        "upb/json_decode.h",
        "upb/json_encode.h",
    ],
    copts = UPB_DEFAULT_COPTS,
    visibility = ["//visibility:public"],
    deps = [
        ":port",
        ":reflection",
        ":upb",
    ],
)

# Internal C/C++ libraries #####################################################

cc_library(
    name = "table",
    hdrs = [
        "upb/table.int.h",
        "upb/upb.h",
    ],
    visibility = ["//tests:__pkg__"],
    deps = [
        ":port",
    ],
)

# Amalgamation #################################################################

# copybara:strip_for_google3_begin

py_binary(
    name = "amalgamate",
    srcs = ["tools/amalgamate.py"],
)

upb_amalgamation(
    name = "gen_amalgamation",
    outs = [
        "upb.c",
        "upb.h",
    ],
    amalgamator = ":amalgamate",
    libs = [
        ":upb",
        ":fastdecode",
        ":descriptor_upb_proto",
        ":reflection",
        ":port",
    ],
)

cc_library(
    name = "amalgamation",
    srcs = ["upb.c"],
    hdrs = ["upb.h"],
    copts = UPB_DEFAULT_COPTS,
    deps = ["//third_party/wyhash"],
)

upb_amalgamation(
    name = "gen_php_amalgamation",
    outs = [
        "php-upb.c",
        "php-upb.h",
    ],
    amalgamator = ":amalgamate",
    libs = [
        ":upb",
        ":fastdecode",
        ":descriptor_upb_proto",
        ":descriptor_upb_proto_reflection",
        ":reflection",
        ":port",
        ":json",
    ],
    prefix = "php-",
)

cc_library(
    name = "php_amalgamation",
    srcs = ["php-upb.c"],
    hdrs = ["php-upb.h"],
    copts = UPB_DEFAULT_COPTS,
    deps = ["//third_party/wyhash"],
)

upb_amalgamation(
    name = "gen_ruby_amalgamation",
    outs = [
        "ruby-upb.c",
        "ruby-upb.h",
    ],
    amalgamator = ":amalgamate",
    libs = [
        ":upb",
        ":fastdecode",
        ":descriptor_upb_proto",
        ":reflection",
        ":port",
        ":json",
    ],
    prefix = "ruby-",
)

cc_library(
    name = "ruby_amalgamation",
    srcs = ["ruby-upb.c"],
    hdrs = ["ruby-upb.h"],
    copts = UPB_DEFAULT_COPTS,
    deps = ["//third_party/wyhash"],
)

exports_files(
    [
        "upb/json/parser.rl",
        "BUILD",
        "WORKSPACE",
    ],
    visibility = ["//cmake:__pkg__"],
)

exports_files(
    [
        "third_party/lunit/console.lua",
        "third_party/lunit/lunit.lua",
    ],
    visibility = ["//tests/bindings/lua:__pkg__"],
)

filegroup(
    name = "cmake_files",
    srcs = glob([
        "google/**/*",
        "upbc/**/*",
        "upb/**/*",
        "tests/**/*",
        "third_party/**/*",
    ]),
    visibility = ["//cmake:__pkg__"],
)

# copybara:strip_end
