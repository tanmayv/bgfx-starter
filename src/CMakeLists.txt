find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(MyApplication MyApplication.cpp bgfx-application.cpp shaders/vs.sc shaders/fs.sc)

target_link_libraries(MyApplication 
    bgfx 
    bx
    glfw
    glm::glm
)

target_include_directories(MyApplication PRIVATE ${TINYGLTF_INCLUDE_DIRS})

message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/bin/assets
    DEPENDS MyApplication)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/include/generated/shaders
  INCLUDE_DIRS ${BGFX_DIR}/src
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/include/generated/shaders
  INCLUDE_DIRS ${BGFX_DIR}/src
)

set_target_properties(MyApplication 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (UNIX)
add_custom_target(dx11vsc
                   COMMAND shaderc ARGS --platform windows --type v --profile 120 -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs.sc -i ${BGFX_DIR}/src -o ${CMAKE_BINARY_DIR}/bin/include/generated/shaders/vs.sc.dx11.bin.h --bin2c vs_dx11
                   COMMENT Here we go
                   VERBATIM)
add_custom_target(dx11fsc
                   COMMAND shaderc ARGS --platform windows --type f --profile 120 -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs.sc -i ${BGFX_DIR}/src -o ${CMAKE_BINARY_DIR}/bin/include/generated/shaders/fs.sc.dx11.bin.h --bin2c fs_dx11
                   COMMENT Here we go
                   VERBATIM)
add_dependencies(MyApplication dx11vsc)
add_dependencies(MyApplication dx11fsc)
endif (UNIX)
target_include_directories(MyApplication PUBLIC ${CMAKE_BINARY_DIR}/bin/include/generated/shaders)
target_include_directories(MyApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)