cmake_minimum_required(VERSION 3.25...3.31)

project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

find_package(pybind11 CONFIG REQUIRED)

python_add_library(_core MODULE src/main.cpp WITH_SOABI)
target_link_libraries(_core PRIVATE pybind11::headers)

target_compile_definitions(_core PRIVATE VERSION_INFO=0.0.1)

install(TARGETS _core DESTINATION typed_bindings)
