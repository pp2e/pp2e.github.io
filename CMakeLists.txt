cmake_minimum_required(VERSION 3.16)

project(PowerPoint2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickParticlesPrivate Multimedia)
find_package(zstd REQUIRED)

if(EMSCRIPTEN)
else()
    find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)
endif()

qt_standard_project_setup()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    qt-web-utils
)

add_subdirectory(lilrcc)

qt_add_executable(appPowerPoint2
    main.cpp
    lilrcc/tree.cpp
    lilrcc/tree.h
    lilrcc/lilrcc.cpp
    lilrcc/lilrcc.h
    lilrcc/resourcereader.cpp
    lilrcc/resourcereader.h
    lilrcc/resourcewriter.cpp
    lilrcc/resourcewriter.h
)

qt_add_qml_module(appPowerPoint2
    URI PowerPoint2
    VERSION 1.0
    QML_FILES
        Main.qml
        Imports.qml
    SOURCES
        pp2window.h
        pp2window.cpp
        pp2backent.h
        pp2backent.cpp
        pp2rccmodel.h
        pp2rccmodel.cpp
        text.h
        text.cpp
        lilrccnetworkaccessmanager.h
        lilrccnetworkaccessmanager.cpp
)

target_link_libraries(appPowerPoint2
    PRIVATE Qt6::Quick #[[zstd::libzstd_static]] zstd
)

if(EMSCRIPTEN)
else()
    target_link_libraries(appPowerPoint2
        PRIVATE Qt6::Widgets
    )
endif()

include_directories(${Qt6Quick_PRIVATE_INCLUDE_DIRS})
