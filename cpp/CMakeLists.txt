cmake_minimum_required(VERSION 3.10)

# set the project name
project(helper LANGUAGES CXX VERSION 0.0.1)

include(CheckIncludeFileCXX)
include(CheckIncludeFile)

# add cxx standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options("-Wno-unused-result")
add_compile_options("-Wall")
add_compile_options("-fstack-clash-protection")
add_compile_options("-fstack-protector-all")
add_compile_options("-std=c++17")
add_compile_options("-pipe")
add_compile_options("-enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang")

option(DEBUG_MODE "Enable debugging" ON)
if (DEBUG_MODE)
    message(WARNING "Warning: debug mode is enabled.")
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options(
        -Wpedantic -pedantic
        -Wextra
        -Wredundant-decls -Wlogical-op
        -Wstrict-overflow=5 -Winit-self
        -Wuninitialized -Wsign-conversion
        -Wcast-qual
    )
else()
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_REENTRANT)
endif()

# add the source
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "Using ccache: ${CCACHE_FOUND}")
else()
    message(STATUS "Ccache not found. Compiling with cache will be disabled.")
endif(CCACHE_FOUND)

find_package(Python)
if(NOT PYTHON_FOUND)
    message(FATAL_ERROR "Cannot build test suites without Python")
endif()

#add the executable
set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
add_executable(helper ${SOURCES})
add_custom_command(
        TARGET helper
        POST_BUILD
        COMMAND strip helper
        VERBATIM
)
install (TARGETS helper DESTINATION /usr/local/bin/)

# add cxx flags
set(CMAKE_EXE_LINKER_FLAGS "-ffunction-sections -fdata-sections -z now -z noexecstack -fPIE -flto")
