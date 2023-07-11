cmake_minimum_required(VERSION 3.22)
project(testpoppler)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(FindPkgConfig)
pkg_check_modules(POPPLER_QT5 REQUIRED IMPORTED_TARGET poppler-qt5)


find_package(Qt5 COMPONENTS
        Core
        Gui
        REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test
        Qt5::Core
        Qt5::Gui
        PkgConfig::POPPLER_QT5
        )

add_executable(extract print_annotations.cpp)
target_link_libraries(extract
        Qt5::Core
        Qt5::Gui
        PkgConfig::POPPLER_QT5
        )
add_executable(create_annot create_debug_annotation.cpp)
target_link_libraries(create_annot
        Qt5::Core
        Qt5::Gui
        PkgConfig::POPPLER_QT5
        )
