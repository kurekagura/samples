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

# Develop using Visual Studio

If you set [構成プロパティ] -> [デバッグ]:[環境] of myapp project as follows, you don't need to copy dll to the same folder as exe file. Also you can avoid PATH contamination. 
```
PATH=%PATH%;C:\lib\opencv-4.5.5\x64\vc16\bin
* no need to "set"
```

This can be generated automatically by CMake instead of manually. Use `set_target_properties`
```
# CMakeLists.txt
if(WIN32)
    set_target_properties(myapp
        PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${OpenCV_DIR}/x64/vc16/bin"
    )
endif()
```
will generate
```
-- myapp.vcxproj --
<LocalDebuggerEnvironment Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">PATH=%PATH%;c:/lib/opencv-4.5.5/x64/vc16/bin</LocalDebuggerEnvironment>
```

# Copy dll to the same folder as exe

Use `add_custom_command`
```
# CMakeLists.txt
add_custom_command(TARGET myapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:myliba>"    #src
    "$<TARGET_FILE_DIR:myapp>"    #dst
    )
```
will generate the following script as PostBuildEvent.
```
-- myapp.vcxproj --
setlocal
C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -noprofile -executionpolicy Bypass -file C:/vcpkg/scripts/buildsystems/msbuild/applocal.ps1 -targetBinary C:/dev/kurekagura/samples/qt/QtCreator4CMakeBehavior/build/Debug/myapp.exe -installedDir C:/vcpkg/installed/x64-windows/debug/bin -OutVariable out
if %errorlevel% neq 0 goto :cmEnd
:cmEnd
endlocal & call :cmErrorLevel %errorlevel% & goto :cmDone
:cmErrorLevel
exit /b %1
:cmDone
if %errorlevel% neq 0 goto :VCEnd
setlocal
C:\sw\CMake\bin\cmake.exe -E copy_if_different C:/dev/kurekagura/samples/qt/QtCreator4CMakeBehavior/build/myliba/Debug/myliba.dll C:/dev/kurekagura/samples/qt/QtCreator4CMakeBehavior/build/Debug
if %errorlevel% neq 0 goto :cmEnd
:cmEnd
endlocal & call :cmErrorLevel %errorlevel% & goto :cmDone
:cmErrorLevel
exit /b %1
:cmDone
if %errorlevel% neq 0 goto :VCEnd
```

The important thing is the following copy command.
```
C:\sw\CMake\bin\cmake.exe -E copy_if_different 
 <yours>/QtCreator4CMakeBehavior/build/myliba/Debug/myliba.dll
 <yours>/QtCreator4CMakeBehavior/build/Debug
```

This source and destination works well in Qt Creator.　i.e.
```
src: "<yours>\QtCreator4CMakeBehavior\build-myapp-Qt5_14_1VS16_x64-Debug\myliba\myliba.dll"
dst: "<yours>\QtCreator4CMakeBehavior\build-myapp-Qt5_14_1VS16_x64-Debug"
```
