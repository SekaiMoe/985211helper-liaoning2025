cmake_minimum_required(VERSION 3.10)

# set the project name
project(helper LANGUAGES CXX VERSION 0.0.1)

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


# add the executable
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

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
