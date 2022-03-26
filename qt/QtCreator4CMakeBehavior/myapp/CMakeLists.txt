cmake_minimum_required(VERSION 3.5)

project(myapp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
#Already added c:/lib/opencv-4.5.5 to CMAKE_PREFIX_PATH on the Custom Kit side
find_package(OpenCV REQUIRED)

#Already added <path to vcpkg.cmake> to CMAKE_TOOLCHAIN_FILE:FILEPATH on the Custom Kit side
#e.x)C:/vcpkg/scripts/buildsystems/vcpkg.cmake
find_package(spdlog REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

if(ANDROID)
  add_library(myapp SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(myapp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

add_subdirectory(./myliba)
target_link_libraries(myapp PRIVATE Qt5::Widgets ${OpenCV_LIBRARIES} spdlog::spdlog myliba)

# Copy dll to exe folder
add_custom_command(TARGET myapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:myliba>"    #src
    "$<TARGET_FILE_DIR:myapp>"    #dst
    )

#message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

