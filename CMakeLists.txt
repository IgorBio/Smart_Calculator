cmake_minimum_required(VERSION 3.5)

project(SmartCalc VERSION 2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR}Charts)

include_directories(
        ${PROJECT_SOURCE_DIR}/model
        ${PROJECT_SOURCE_DIR}/controller
        ${PROJECT_SOURCE_DIR}/view
)

set(HEADERS
        ${PROJECT_SOURCE_DIR}/controller/controller.h
        ${PROJECT_SOURCE_DIR}/model/token.h
        ${PROJECT_SOURCE_DIR}/model/math_calc.h
        ${PROJECT_SOURCE_DIR}/model/credit_calc.h
        ${PROJECT_SOURCE_DIR}/model/deposit_calc.h
        ${PROJECT_SOURCE_DIR}/view/view.h
        ${PROJECT_SOURCE_DIR}/view/chart.h
        ${PROJECT_SOURCE_DIR}/view/validator.h
)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/main.cc
        ${PROJECT_SOURCE_DIR}/controller/controller.cc
        ${PROJECT_SOURCE_DIR}/model/math_calc.cc
        ${PROJECT_SOURCE_DIR}/model/credit_calc.cc
        ${PROJECT_SOURCE_DIR}/model/deposit_calc.cc
        ${PROJECT_SOURCE_DIR}/view/view.cc
        ${PROJECT_SOURCE_DIR}/view/chart.cc
        ${PROJECT_SOURCE_DIR}/view/validator.cc
)

set(FORMS
        ${PROJECT_SOURCE_DIR}/view/view.ui
)

set(PROJECT_SOURCES
        ${HEADERS}
        ${SOURCES}
        ${FORMS}
        ${CMAKE_SOURCE_DIR}/main.cc
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SmartCalc
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET SmartCalc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(SmartCalc SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(SmartCalc
                ${PROJECT_SOURCES}
        )
    endif ()
endif ()

target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        -Wall
        -Werror
        -Wextra
        -Wpedantic
        -std=c++17
)

target_link_libraries(SmartCalc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)

set_target_properties(SmartCalc PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SmartCalc)
endif ()

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    message(STATUS "cppcheck found: ${CPPCHECK}")
    list(
        APPEND CPPCHECK_ARGS
        "--enable=all"
        "--inconclusive"
        "--quiet"
        "--language=c++"
        "--std=c++17"
        "--suppress=missingInclude"
        "--suppress=unusedStructMember"
        "--suppress=unusedFunction"
        "--suppress=unusedPrivateFunction"
    )
    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${SOURCES} ${HEADERS}
    )
else()
    message(STATUS "cppcheck not found")
endif()