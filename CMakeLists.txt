# basic project config
cmake_minimum_required(VERSION 3.19)
set(project_name "bytebeat")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# load modules
include(SuperColliderServerPlugin RESULT_VARIABLE server_plugin_found)
if(NOT server_plugin_found)
    message(FATAL_ERROR "Could not find server plugin functions module")
endif()

include(SuperColliderCompilerConfig RESULT_VARIABLE compiler_config_found)
if(NOT compiler_config_found)
    message(FATAL_ERROR "Could not find compiler config module")
endif()

# Windows - puts redistributable DLLs in install directory
include(InstallRequiredSystemLibraries)

sc_check_sc_path("${SC_PATH}")
message(STATUS "Found SuperCollider: ${SC_PATH}")
set(SC_PATH "${SC_PATH}" CACHE PATH
    "Path to SuperCollider source. Relative paths are treated as relative to this script" FORCE)

include("${SC_PATH}/SCVersion.txt")
message(STATUS "Building plugins for SuperCollider version: ${SC_VERSION}")

# set project here to avoid SCVersion.txt clobbering our version info
project(${project_name})
sc_do_initial_compiler_config() # do after setting project so compiler ID is available

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX)
    message(WARNING "No install prefix provided, defaulting to $BUILD_DIR/install")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

message(STATUS "Install directory set to: ${CMAKE_INSTALL_PREFIX}")

# options
# option(SUPERNOVA "Build plugins for supernova" ON)
option(SCSYNTH "Build plugins for scsynth" ON)
option(NATIVE "Optimize for native architecture" OFF)
option(STRICT "Use strict warning flags" OFF)
# option(NOVA_SIMD "Build plugins with nova-simd support." ON)

# include libraries
# if (NOVA_SIMD)
#     add_definitions(-DNOVA_SIMD)
#     include_directories(${SC_PATH}/external_libraries/nova-simd)
# endif()

# bytebeat target
set(common_cpp_files
    src/parse.cpp
    src/tokenize.cpp
)
add_executable(
    bytebeat
    src/main.cpp
    ${common_cpp_files}
)
target_include_directories(bytebeat PRIVATE include)

# test_bytebeat target
set(test_cpp_files
    test/test_expression.cpp
    test/test_parse.cpp
    test/test_tokenize.cpp
)
find_package(Catch2 REQUIRED)
add_executable(
    test_bytebeat
    test/test.cpp
    ${common_cpp_files}
    ${test_cpp_files}
)
target_include_directories(test_bytebeat PRIVATE include)
target_link_libraries(test_bytebeat PRIVATE Catch2::Catch2)

# SuperCollider targets
set(plugin_cpp_files
    plugins/ByteBeat/ByteBeat.hpp
    plugins/ByteBeat/ByteBeat.cpp
    ${common_cpp_files}
)
set(plugin_sc_files
    plugins/ByteBeat/ByteBeat.sc
    plugins/ByteBeat/ByteBeatController.sc
)
set(plugin_schelp_files
    plugins/ByteBeat/ByteBeat.schelp
)
sc_add_server_plugin(
    "ByteBeat/ByteBeat" # destination directory
    "ByteBeat" # target name
    include
    "${plugin_cpp_files}"
    "${plugin_sc_files}"
    "${plugin_schelp_files}"
)
message(STATUS "Generating plugin targets done")
