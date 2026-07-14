cmake_minimum_required(VERSION 3.23)

project(isophine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.11 REQUIRED COMPONENTS Core Widgets Pdf)
find_package(qpdf REQUIRED)

qt_standard_project_setup()

add_subdirectory(src)
