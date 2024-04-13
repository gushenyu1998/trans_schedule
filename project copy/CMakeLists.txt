# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(Assignment3 VERSION 1.0 DESCRIPTION "Project" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)


# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)

set(ENV{PKG_CONFIG_PATH} "/usr/lib/arm-linux-gnueabihf/pkgconfig")

find_package(PkgConfig)
pkg_check_modules(LIBNFC REQUIRED libnfc)
pkg_check_modules(JSON-C REQUIRED json-c)