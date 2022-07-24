# My Latest

```
mklink /d C:\lib\darknet-0.2.5.4 C:\lib\darknet-0.2.5.4a
```

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
