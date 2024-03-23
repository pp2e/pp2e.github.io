cmake_minimum_required(VERSION 3.16)

project(PowerPoint2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickParticlesPrivate Multimedia)

if(EMSCRIPTEN)
else()
    find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)
endif()

qt_standard_project_setup()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    qt-web-utils
)

qt_add_executable(appPowerPoint2
    main.cpp
    # pp2inspector.h
    # pp2inspector.cpp
)

qt_add_qml_module(appPowerPoint2
    URI PowerPoint2
    VERSION 1.0
    SOURCES
        pp2window.h
        pp2window.cpp
        pp2backent.h
        pp2backent.cpp
        pp2rccmodel.h
        pp2rccmodel.cpp
        text.h
        text.cpp
    QML_FILES
        Main.qml
        Imports.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPowerPoint2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPowerPoint2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPowerPoint2
    PRIVATE Qt6::Quick #[[Qt6::GuiPrivate]]
)

if(EMSCRIPTEN)
else()
    target_link_libraries(appPowerPoint2
        PRIVATE Qt6::Widgets
    )
endif()

include_directories(${Qt6Quick_PRIVATE_INCLUDE_DIRS})

# https://bugreports.qt.io/browse/QTBUG-109959
# set_property(TARGET appPowerPoint2 PROPERTY WASM_SHELL_HTML_TEMPLATE_FILE "${CMAKE_SOURCE_DIR}/wasm/wasm_shell.html")

include(GNUInstallDirs)
install(TARGETS appPowerPoint2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
