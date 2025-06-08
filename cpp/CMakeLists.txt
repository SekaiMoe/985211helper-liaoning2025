if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
        message(FATAL_ERROR "ERROR: In-source builds are not allowed, please use an extra build dir.")
    endif()
endif()

cmake_minimum_required(VERSION 3.10)

# set the project name
project(helper LANGUAGES CXX VERSION 0.0.1)

include(CheckIncludeFileCXX)
include(CheckIncludeFile)

# add cxx standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    # MSVC specific flags
    add_compile_options(
        /W4
        /MP     # Enable multi-processor compilation
        /EHsc   # Enable C++ exception handling
        /fp:fast # Fast floating point model
        /DWIN32
        /D_WINDOWS
    )
    # Disable specific warnings
    add_compile_options(/wd4819)  # Code page warning
else()
    # GCC/Clang flags
    add_compile_options(
        "-Wno-unused-result"
        "-Wall"
        "-fstack-clash-protection"
        "-fstack-protector-all"
        "-std=c++17"
        "-pipe"
    )
endif()

option(STATIC_BUILD "Enable static building" OFF)
if(STATIC_BUILD)
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        add_compile_options(/MT$<$<CONFIG:Debug>:d>)
    else()
        add_compile_options("-static")
    endif()
endif()

#add the executable
set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
add_executable(helper
    src/helper.cpp
    src/index.cpp
    src/json.cpp
    src/search.cpp
    src/signal.cpp
    src/usage.cpp
    src/version.cpp
    src/data.cpp
    src/web/web.cpp
)

set(CMAKE_COLOR_DIAGNOSTICS ON)

# Check compiler flags only for GCC/Clang
if(NOT MSVC)
    include(CheckCXXCompilerFlag)
    set(FLAGS_TO_CHECK
        "-static"
        "-pipe"
        "-fdata-sections"
        "-ffunction-sections"
        "-fstack-clash-protection"
        "-fstack-protector-all"
    )

    set(UNSUPPORTED_FLAGS "")

    foreach(FLAG ${FLAGS_TO_CHECK})
        string(REPLACE "-" "_" FLAG_VAR "CXXFLAG_CHECK_${FLAG}")
        check_cxx_compiler_flag(${FLAG} ${FLAG_VAR})
        if(NOT ${FLAG_VAR})
            list(APPEND UNSUPPORTED_FLAGS ${FLAG})
        endif()
    endforeach()

    if(UNSUPPORTED_FLAGS)
        message(FATAL_ERROR "ERROR: The following CXX flags are not supported: ${UNSUPPORTED_FLAGS}")
    endif()
endif()

# Handle date and time for both Windows and Unix
if(WIN32)
    string(TIMESTAMP CMAKE_CURRENT_DATE "%Y-%m-%d")
    string(TIMESTAMP CMAKE_CURRENT_TIME "%H:%M:%S")
else()
    execute_process(
        COMMAND date "+%Y-%m-%d"
        OUTPUT_VARIABLE CMAKE_CURRENT_DATE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND date "+%H:%M:%S"
        OUTPUT_VARIABLE CMAKE_CURRENT_TIME
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

add_definitions(-DBUILD_DATE="${CMAKE_CURRENT_DATE}")
add_definitions(-DBUILD_TIME="${CMAKE_CURRENT_TIME}")

option(DEBUG_MODE "Enable debugging" OFF)
if(DEBUG_MODE)
    message(WARNING "Warning: debug mode is enabled.")
    set(CMAKE_BUILD_TYPE Debug)
    if(MSVC)
        add_compile_options(/Od /Zi /RTC1)
    else()
        add_compile_options(
            -Wpedantic -pedantic
            -Wextra
            -Wredundant-decls -Wlogical-op
            -Wstrict-overflow=5 -Winit-self
            -Wuninitialized -Wsign-conversion
            -Wcast-qual
        )
    endif()
else()
    set(CMAKE_BUILD_TYPE Release)
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_REENTRANT)
        add_custom_command(
            TARGET helper
            POST_BUILD
            COMMAND strip helper
            VERBATIM
        )
    endif()
endif()

option(APPUI_WEBUI "Enable WEBUI (Web User Interface)" OFF)
if(APPUI_WEBUI)
    add_definitions(-DAPPUI="WEBUI" -DWEBUI)
else()
    add_definitions(-DAPPUI="TUI" -DTUI)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Configure CCache if available
if(NOT MSVC)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        message(STATUS "Using ccache: ${CCACHE_FOUND}")
    else()
        message(STATUS "Ccache not found. Compiling with cache will be disabled.")
    endif()
endif()

find_package(Python)
if(NOT PYTHON_FOUND)
    message(FATAL_ERROR "Cannot build test suites without Python")
endif()

option(BUILD_TEST "Whether or not to build the tests" ON)
if(BUILD_TEST)
    if(APPUI_WEBUI)
        message(FATAL_ERROR "Please disable webui to build test")
    else()
        message(STATUS "Tests will be built.")
        enable_testing()
        add_subdirectory(test)
    endif()
endif()

install(TARGETS helper DESTINATION /usr/local/bin/)

# Set linker flags based on compiler
if(NOT MSVC)
    if(STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-ffunction-sections -fdata-sections -z now -z noexecstack -fPIE -static")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "-ffunction-sections -fdata-sections -z now -z noexecstack -fPIE")
    endif()
endif()