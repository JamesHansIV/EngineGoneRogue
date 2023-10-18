load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_import")


cc_import(
    name = "SDL2",
    hdrs = glob(["include/SDL2/*.h"]),
    shared_library = "lib/libSDL2-2.0.0.dylib",
)

cc_import(
    name="SDL_image",
    hdrs = glob(["include/SDL_image.h"]),
    shared_library = "lib/libSDL2_image-2.0.0.dylib",
)

cc_import(
    name = "SDL_ttf",
    hdrs = glob(["include/SDL_ttf.h"]),
    shared_library = "lib/libSDL2_ttf-2.0.0.dylib",
)

cc_library(
    name = "globals",
    hdrs = ["global.h"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":SDL2",
        ":SDL_image",
        ":SDL_ttf",
        ":globals",
    ],
    linkstatic = False
)