cmake_minimum_required(VERSION 3.5)

project(HeartWave VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg)

set(PROJECT_SOURCES
        qcustomplot.h qcustomplot.cpp
        enums.h
        heartrategraph.h heartrategraph.cpp
        settingsconfig.h settingsconfig.cpp
        breathmeter.h breathmeter.cpp
        breathpacer.h breathpacer.cpp
        plotter.h plotter.cpp
        batteryindicator.h batteryindicator.cpp
        clabar.h clabar.cpp
        heartrateindicator.h heartrateindicator.cpp
        mainmenuview.h mainmenuview.cpp
        screencontextmanager.h screencontextmanager.cpp
        screen.h screen.cpp
        navigationbar.h navigationbar.cpp
        navigationpad.h navigationpad.cpp
        newsessionview.h newsessionview.cpp
        summaryview.h summaryview.cpp
        initsessionview.h initsessionview.cpp
        logview.h logview.cpp
        deviceconfig.h deviceconfig.cpp
        settingsview.h settingsview.cpp
        coherencebar.h coherencebar.cpp
        coherencecalculator.h coherencecalculator.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HeartWave
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET HeartWave APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(HeartWave SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(HeartWave
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(HeartWave PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(HeartWave PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(HeartWave PRIVATE Qt${QT_VERSION_MAJOR}::Svg)

set_target_properties(HeartWave PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS HeartWave
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HeartWave)
endif()
