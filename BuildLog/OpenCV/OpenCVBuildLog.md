# build4.5.5a

## [ Version&Environment ]
```
Microsoft Visual Studio Community 2019
Version 16.11.9

* The following is based on 'cmake -L'
Host: Windows 10.0.19043 AMD64
CMake: 3.17.0
CMake generator: Visual Studio 16 2019
MSVC: 1929
Python 3 Interpreter: ver 3.7.3
QT: ver 5.14.1
GStreamer: 1.18.5
Intel Media SDK: Media SDK 2021 R1 x64/libmfx_vs2015.lib
NVIDIA CUDA: ver 11.5
cuDNN: ver 8.3.1
```
```
c:\>where python && python --version
C:\sw\Python37\python.exe
Python 3.7.3

c:\>where Qt5Core.dll
C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin\Qt5Core.dll

c:\>where nppc64_11.dll
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5\bin\nppc64_11.dll

c:\>where cudnn64_8.dll
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5\bin\cudnn64_8.dll

c:\>where libmfxsw64.dll
C:\sw\IntelSWTools\Intel(R) Media SDK 2021 R1\Software Development Kit\bin\x64\libmfxsw64.dll

c:\>where gst-inspect-1.0.exe
C:\gstreamer\1.0\msvc_x86_64\bin\gst-inspect-1.0.exe

// Not used.
c:\>where tbb.dll
C:\lib\tbb-2020.3\bin\intel64\vc14\tbb.dll
// Not used.
c:\>where vulkaninfoSDK.exe
C:\VulkanSDK\1.2.198.1\Bin\vulkaninfoSDK.exe
```
## [ Important Workaround ]

Build fails with WITH_GSTREAMER=ON, so the following patch was applied to solve the problem.

- [videoio: added explicit gst-audio dependency for windows #21500](https://github.com/opencv/opencv/pull/21500/files)
  "opencv\modules\videoio\cmake\detect_gstreamer.cmake"

## [ Preparation ]

```
// Using [x64 Native Tools Command Prompt for VS 2019],

vs>python -m venv c:\venv\opencv-4.5.5a
vs>c:\venv\opencv-4.5.5a\Scripts\activate.bat

// Using this venv,

venv>pip install numpy
venv>python -m pip install --upgrade pip
```
## [ Cmake & Build ]

```
// Using this venv,

cd /d o:\src\opencv-all
mkdir build4.5.5a && cd build4.5.5a

set BUILDID=4.5.5a

cmake "../opencv" ^
-G "Visual Studio 16 2019" ^
-D CMAKE_CONFIGURATION_TYPES="Release" ^
-D CMAKE_BUILD_TYPE="Release" ^
-D OPENCV_EXTRA_MODULES_PATH=O:/src/opencv-all/opencv_contrib/modules ^
-D BUILD_opencv_world=ON ^
-D BUILD_opencv_python3=ON ^
-D CPU_BASELINE=AVX2 ^
-D WITH_QT=ON ^
-D WITH_OPENGL=ON ^
-D BUILD_opencv_cvv=OFF ^
-D CMAKE_INSTALL_PREFIX=C:/lib/opencv-%BUILDID% ^
-D PYTHON3_PACKAGES_PATH=C:/venv/opencv-%BUILDID%/Lib/site-packages ^
-D INSTALL_PYTHON_EXAMPLES=ON ^
-D WITH_CUDA=ON ^
-D CUDA_GENERATION="Turing" ^
-D OPENCV_DNN_CUDA=ON ^
-D CUDA_FAST_MATH=ON ^
-D ENABLE_FAST_MATH=ON ^
-D WITH_NVCUVID=ON ^
-D WITH_MFX=ON ^
-D WITH_GSTREAMER=ON

msbuild INSTALL.vcxproj /p:Configuration=Release /target:Clean;ReBuild
* 経過時間 01:01:24.66
```
```
cmake "../opencv" -LA > build%BUILDID%-CMakeCachedVars.txt 2>&1
```
[build4.5.5a-CMakeCachedVars.txt](./build4.5.5a-CMakeCachedVars.txt)

## [ Validation ]
```
//set PATH=C:\lib\opencv-%BUILDID%\x64\vc16\bin;%PATH%
python -c "import cv2; print(f'version={cv2.__version__}')"
version=4.5.5
python -c "import cv2; print(cv2.getBuildInformation())" > build%BUILDID%-getBuildInformation.txt 2>&1
```
[build4.5.5a-getBuildInformation.txt](./build4.5.5a-getBuildInformation.txt)
