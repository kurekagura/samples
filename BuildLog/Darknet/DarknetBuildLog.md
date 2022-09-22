# My Latest

```
rmdir C:\lib\darknet-0.2.5.4
mklink /d C:\lib\darknet-0.2.5.4 C:\lib\darknet-0.2.5.4b
```

# build0.2.5.4b (2022/09/22)

## [Version&Environment]

```
>nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2022 NVIDIA Corporation
Built on Wed_Jun__8_16:59:34_Pacific_Daylight_Time_2022
Cuda compilation tools, release 11.7, V11.7.99
Build cuda_11.7.r11.7/compiler.31442593_0
```

```
*"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5\include\cudnn_version.h"
#define CUDNN_MAJOR 8
#define CUDNN_MINOR 5
#define CUDNN_PATCHLEVEL

*"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\include\cudnn_version.h"
#define CUDNN_MAJOR 8
#define CUDNN_MINOR 5
#define CUDNN_PATCHLEVEL
```
11.5 is required at build time, but 11.7 was fine at run time.

```
O:\src\AlexeyAB\darknet>cmake --version
cmake version 3.24.2
```

## [Prepare & Build]
Start `x64 Native Tools Command Prompt for VS 2022`

```
O:\src\AlexeyAB\darknet>git rev-parse HEAD
62e5549cef53bd11890808615e0bfae59cfd4491

O:\src\AlexeyAB>mkdir build0.2.5.4b & cd build0.2.5.4b
```

Differences from previous: VS version

```
cmake "../darknet" ^
-G "Visual Studio 17 2022" ^
-D CMAKE_INSTALL_PREFIX=C:/lib/darknet-0.2.5.4b ^
-D OpenCV_DIR=C:/lib/opencv-4.5.5 ^
-D CUDA_TOOLKIT_ROOT_DIR="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.5" ^
-D CMAKE_CUDA_COMPILER="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.5/bin/nvcc.exe"
```

```
msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild && ^
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild

cmake "../darknet" -LA > build0.2.5.4b-CMakeCachedVars.txt 2>&1
```
Add C:/lib/opencv-4.5.5 in PATH
```
>copy ..\darknet\build\darknet\x64\pthreadVC2.dll ..\darknet
>copy Release\darknet.* ..\darknet
>cd ..\darknet
>darknet detect cfg/yolov3.cfg yolov3.weights data/dog.jpg
>darknet detector demo cfg/coco.data cfg/yolov3.cfg yolov3.weights
```

## [Workaround]

```
CMake Error at C:/sw/CMake3.24/share/cmake-3.24/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find CUDA: Found unsuitable version "11.7", but required is exact
  version "11.5" (found C:/Program Files/NVIDIA GPU Computing
  Toolkit/CUDA/v11.7)
```
The CUDA for cmake must be 11.5.

```
Could not locate zlibwapi.dll. Please make sure it is in your library path!
```
It was downloaded and placed in C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5\bin.

# build0.2.5.4a (2022/7/24)

## [Version&Environment]
```
# CMake outputs at the top
Darknet_VERSION: 0.2.5.4

**********************************************************************
** Visual Studio 2019 Developer Command Prompt v16.11.9
** Copyright (c) 2021 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'

>cmake --version
cmake version 3.23.2

>set CUDA_PATH
CUDA_PATH=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5
CUDA_PATH_V11_5=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5
CUDA_PATH_V11_6=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6

>set CUDNN
CUDNN_PATH=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5
```

# [Prepare & Build]
Start `x64 Native Tools Command Prompt for VS 2019`
```
>o:
>cd src
>mkdir AlexeyAB && cd AlexeyAB
>git clone https://github.com/AlexeyAB/darknet
>cd darknet && git rev-parse HEAD
25505164a3bd6235c75deaad325878ceda90249a
>cd ..
>mkdir build0.2.5.4a & cd build0.2.5.4a
```

```
cmake "../darknet" ^
-G "Visual Studio 16 2019" ^
-D CMAKE_INSTALL_PREFIX=C:/lib/darknet-0.2.5.4a ^
-D OpenCV_DIR=C:/lib/opencv-4.5.5 ^
-D CUDA_TOOLKIT_ROOT_DIR=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.5 ^
-D CMAKE_CUDA_COMPILER=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.5/bin/nvcc.exe

msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild && ^
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild

cmake "../darknet" -LA > build0.2.5.4a-CMakeCachedVars.txt 2>&1
```
Add C:/lib/opencv-4.5.5 in PATH
```
>copy ..\darknet\build\darknet\x64\pthreadVC2.dll Release
>Release\darknet.exe detect
 CUDA-version: 11050 (11060), cuDNN: 8.4.1, GPU count: 1
 OpenCV version: 4.5.5
Couldn't open file: cfg/coco.data
```

## [Workaround]

Uninstalled old CMake, then installed cmake-3.23.2-windows-x86_64.msi.

```
Could NOT find CUDA: Found unsuitable version "11.6",
but required is exact version "11.5"
(found C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6)
```
-> Installed `cuda_11.5.2_496.13_windows.exe`

-> Installed `cudnn-windows-x86_64-8.4.1.50_cuda11.6-archive.zip`

Copied cuDNN into `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5`
