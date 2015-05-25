cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======================== #
# Set Project Version/Name #
# ======================== #

project (GP_SYMB_REG_CLI)
set(PROJECT_NAME "gp_symb_reg_cli")

# ================= #
# Build Option      #
# ================= #

# default to a debug build if no CMAKE_BUILD_TYPE is defined
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()


set(GP_SYMB_REG_INCLUDE_DIR "../lib/src" CACHE STRING "The include directory of gp_symb_reg")
set(GP_SYMB_REG_LIB_DIR "../lib/build" CACHE STRING "The lib directory of gp_symb_reg")
set(GP_SYMB_REG_LIB_BASE_NAME "gp_symb_reg" CACHE STRING "The base name of the compiled library gp_symb_reg")
set(GP_SYMB_REG_USE_STATIC_LIB false CACHE BOOL "Whether or not to use static libraries")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ================= #
# Dependencies      #
# ================= #

include_directories(${GP_SYMB_REG_INCLUDE_DIR})

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED OFF)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# deduce the libraries suffix from the options
set(FIND_GP_SYMB_REG_SUFFIX "")
if(GP_SYMB_REG_USE_STATIC_LIB)
    set(FIND_GP_SYMB_REG_SUFFIX "${FIND_GP_SYMB_REG_SUFFIX}_s")
endif()

find_library(GP_SYMB_REG_LIBRARY_DEBUG NAMES ${GP_SYMB_REG_LIB_BASE_NAME}_d${FIND_GP_SYMB_REG_SUFFIX} lib${GP_SYMB_REG_LIB_BASE_NAME}_d${FIND_GP_SYMB_REG_SUFFIX} PATH_SUFFIXES lib lib64 PATHS ${GP_SYMB_REG_LIB_DIR})
find_library(GP_SYMB_REG_LIBRARY_RELEASE NAMES ${GP_SYMB_REG_LIB_BASE_NAME}${FIND_GP_SYMB_REG_SUFFIX} lib${GP_SYMB_REG_LIB_BASE_NAME}${FIND_GP_SYMB_REG_SUFFIX} PATH_SUFFIXES lib lib64 PATHS ${GP_SYMB_REG_LIB_DIR})

if(NOT "${GP_SYMB_REG_LIBRARY_DEBUG}" STREQUAL "GP_SYMB_REG_LIBRARY_DEBUG-NOTFOUND")
    set(GP_SYMB_REG_LIBS ${GP_SYMB_REG_LIBS} ${GP_SYMB_REG_LIBRARY_DEBUG})
endif()

if(NOT "${GP_SYMB_REG_LIBRARY_RELEASE}" STREQUAL "GP_SYMB_REG_LIBRARY_RELEASE-NOTFOUND")
    set(GP_SYMB_REG_LIBS ${GP_SYMB_REG_LIBS} ${GP_SYMB_REG_LIBRARY_RELEASE})
endif()

# ================= #
# Build Option      #
# ================= #

# Set the source files to compile
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Make sure we're compiling with C++14 (C++1y)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# Check if were compiling with clang, if we are, we need
# to add "-stdlib=libc++" to the command line in order for it to compile
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Note: on some platforms (OS X), CMAKE_COMPILER_IS_GNUCXX is true even when CLANG is used
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Add the library
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GP_SYMB_REG_LIBS})
