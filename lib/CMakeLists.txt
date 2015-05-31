cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======================== #
# Set Project Version/Name #
# ======================== #

project (gp_symb_reg)

# ================= #
# Build Option      #
# ================= #

find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# default to a debug build if no CMAKE_BUILD_TYPE is defined
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()

set(BUILD_SHARED_LIBS true CACHE BOOL "A flag to build shared (dynamic) libs")
set(VERBOSE_LOG false CACHE BOOL "Verbose logging")

if(VERBOSE_LOG)
    set(VERBOSE_LOG true)
endif()


set(SCALAR_TYPE int CACHE STRING "Choose the type of scalar you want to use")
set_property(CACHE SCALAR_TYPE PROPERTY STRINGS "int" "float" "double")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/Config.hpp.inl ${CMAKE_CURRENT_SOURCE_DIR}/src/Config.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Determine if we're building a shared (dynamic) library
# And set appropriate suffixes for the executables
if(BUILD_SHARED_LIBS)
    set(CMAKE_DEBUG_POSTFIX "_d")
else()
    add_definitions(-DSTATIC_LIB)
    set(CMAKE_DEBUG_POSTFIX "_d_s")
    set(CMAKE_RELEASE_POSTFIX "_s")
endif()

# ===================== #
# Compiling/Building... #
# ===================== #

# Set the source files to compile
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Make sure we're compiling with C++14 (C++1y)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Check if were compiling with clang, if we are, we need
# to add "-stdlib=libc++" to the command line in order for it to compile
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Note: on some platforms (OS X), CMAKE_COMPILER_IS_GNUCXX is true even when CLANG is used
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Add the library
add_library(${PROJECT_NAME} ${SOURCES})

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "x86_64;")
endif()
