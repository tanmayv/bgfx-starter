cmake_minimum_required(VERSION 3.18)
project(MyApplication)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)

# Add Corrade as a subproject
message("")
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(BGFX_BUILD_TOOLS_SHADER ON)
set(BGFX_BUILD_EXAMPLES OFF)
set(BGFX_BUILD_TOOLS_GEOMETRY OFF)
set(BGFX_BUILD_TOOLS_TEXTURE OFF)
add_subdirectory(bgfx.cmake)
add_subdirectory(src)
