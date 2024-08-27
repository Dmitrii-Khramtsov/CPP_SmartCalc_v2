cmake_minimum_required(VERSION 3.5)

project(s21_SmartCalc_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

set(PROJECT_SOURCES
    controller/calc_controller.h
    controller/calc_controller.cc
    model/model_calculator.cc
    model/model_calculator.h
    model/model_credit.cc
    model/model_credit.h
    model/model_deposit.h
    model/model_deposit.cc
    model/polish_notation.h
    model/polish_notation.cc
    qcustomplot.cpp
    qcustomplot.h
    view/main.cpp
    view/mainwindow.cpp
    view/mainwindow.h
    view/mainwindow.ui
    view/creditview.h
    view/creditview.cpp
    view/creditview.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(s21_SmartCalc_v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        view/depositview.h view/depositview.cpp view/depositview.ui
        view/graphview.h view/graphview.cpp view/graphview.ui
    )
else()
    if(ANDROID)
        add_library(s21_SmartCalc_v2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(s21_SmartCalc_v2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(s21_SmartCalc_v2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(s21_SmartCalc_v2 PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(s21_SmartCalc_v2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(s21_SmartCalc_v2)
endif()
