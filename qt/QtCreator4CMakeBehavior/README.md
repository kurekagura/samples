# Generates Visual Studio files

Using `x64 Native Tools Command Prompt for VS 2019`
```
mkdir build && cd build

cmake -G "Visual Studio 16 2019" ^
-D CMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" ^
-D OpenCV_DIR="c:/lib/opencv-4.5.5" ^
..\myapp

msbuild ALL_BUILD.vcxproj /p:Configuration="debug" /t:ReBuild
msbuild ALL_BUILD.vcxproj /p:Configuration="release" /t:ReBuild
```

The following variables related to libraries installed with Vcpkg are not required.
```
-D spdlog_DIR="C:/vcpkg/installed/x64-windows/share/spdlog" ^
-D fmt_DIR=C:/vcpkg/installed/x64-windows/share/fmt ^
```

If you set [構成プロパティ] -> [デバッグ]:[環境] of myapp project as follows, you don't need to copy dll to exe folder. Also, you can suppress PATH contamination.
```
PATH=%PATH%;C:\lib\opencv-4.5.5\x64\vc16\bin
* no need to "set"
```

