cmake_minimum_required(VERSION 3.5)

project(render_bmp_as_movie LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(OpenCV_DIR "c:/lib/opencv-4.5.5" CACHE PATH "Test")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# In CMakeCache, set OpenCV_DIR
find_package(OpenCV REQUIRED)

#Already added <path to vcpkg.cmake> to CMAKE_TOOLCHAIN_FILE:FILEPATH on the Custom Kit side
#e.x)C:/vcpkg/scripts/buildsystems/vcpkg.cmake
find_package(spdlog REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(render_bmp_as_movie
    myutilities.h
    myutilities.cpp
    mywindow1.ui
    mywindow2.ui
    mywindow3.ui
    mywindow4.ui
    captureqthread.h
    capturestdthread.h
    imagecapturethread.h
    imageprocessorthread.h
    mywindow1.h
    mywindow2.h
    mywindow3.h
    mywindow4.h
    queuechannel.h
    videodevicethread.h
    captureqthread.cpp
    capturestdthread.cpp
    imagecapturethread.cpp
    imageprocessorthread.cpp
    main.cpp
    mywindow1.cpp
    mywindow2.cpp
    mywindow3.cpp
    mywindow4.cpp
    videodevicethread.cpp
)

target_link_libraries(render_bmp_as_movie PRIVATE Qt5::Widgets ${OpenCV_LIBRARIES} spdlog::spdlog)

if(WIN32)
    set_target_properties(render_bmp_as_movie
        PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${OpenCV_DIR}/x64/vc16/bin"
    )
endif()
