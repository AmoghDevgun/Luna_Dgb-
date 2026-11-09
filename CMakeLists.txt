cmake_minimum_required(VERSION 3.21)
project(lunadbg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(lunadbg
  src/main.cpp
  src/ui/MainWindow.h
  src/ui/MainWindow.cpp
  src/ui/TraceTableModel.h
  src/ui/TraceTableModel.cpp
)

target_link_libraries(lunadbg PRIVATE Qt6::Widgets)
