cmake_minimum_required(VERSION 3.12)

project(App)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5PrintSupport)

file(GLOB HEADERS /*.hpp)
file(GLOB SOURCES library/*.cpp)

add_library(library ${HEADERS} ${SOURCES})
target_include_directories(library PUBLIC ./library/)

set_target_properties(library PROPERTIES CXX_STANDARD 17)

add_executable(app
               application/mainwindow.cpp
               application/main.cpp
               3rdparty/qcustomplot.cpp
)

target_link_libraries(app library)
target_link_libraries(app Qt5::Widgets Qt5::PrintSupport)

set_target_properties(app PROPERTIES CXX_STANDARD 17)
