# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(light_sampler VERSION 1.0 DESCRIPTION "Starter project" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
#add_compile_options(-Wall -Werror -Wpedantic -Wextra)
add_compile_options(-Wall -Wpedantic -Wextra -Wno-implicit-function-declaration)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)
add_subdirectory(app)

# Include the directory where pkg-config can find .pc files for cross-compiled libraries
set(ENV{PKG_CONFIG_PATH} "/usr/lib/arm-linux-gnueabihf/pkgconfig")

# Find libnfc using pkg-config
find_package(PkgConfig)
pkg_check_modules(LIBNFC REQUIRED libnfc)
pkg_check_modules(FLITE REQUIRED flite)

