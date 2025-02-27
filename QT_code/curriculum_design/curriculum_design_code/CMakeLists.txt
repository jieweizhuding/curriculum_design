cmake_minimum_required(VERSION 3.16)

project(curriculum_design_code VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(third_parties "${CMAKE_SOURCE_DIR}/../../../third_parties")

include_directories(${third_parties}/../include ${third_parties}/ogdf-master/ogdf-master/include D:/third_parties/ogdf-master/ogdf-master/build_debug/include)
include_directories(${third_paties}/spdlog-1.15.0/spdlog-1.15.0/include)
# link_libraries(D:/third_parties/ogdf-master/ogdf-master/build/Release)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(curriculum_design_code
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        dialog.h dialog.cpp dialog.ui
        ../../../include/arc_node.h ../../../include/my_graph.h ../../../include/index_node.h
        dialog_arc.h dialog_arc.cpp dialog_arc.ui
        dialog_delete.h dialog_delete.cpp dialog_delete.ui
        dialog_delete_arc.h dialog_delete_arc.cpp dialog_delete_arc.ui
        dialog_tsort.h dialog_tsort.cpp dialog_tsort.ui
        tsort.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET curriculum_design_code APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(curriculum_design_code SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(curriculum_design_code
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(curriculum_design_code PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(curriculum_design_code PRIVATE D:/third_parties/ogdf-master/ogdf-master/build_debug/DEBUG/ogdf.lib)
target_link_libraries(curriculum_design_code PRIVATE D:/third_parties/ogdf-master/ogdf-master/build_debug/DEBUG/coin.lib)
target_link_libraries(curriculum_design_code PRIVATE D:/third_parties/spdlog-1.15.0/build/Debug/spdlogd.lib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.curriculum_design_code)
endif()


set_target_properties(curriculum_design_code PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS curriculum_design_code
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(curriculum_design_code)
endif()
