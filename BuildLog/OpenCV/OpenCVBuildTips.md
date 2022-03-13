# First, release build, and finally debug build

To save time, first try the 'release' build.
Finally, remove CMAKE_CONFIGURATION_TYPES and CMAKE_BUILD_TYPE to generate a project file (vcxproj) containing all Configurations.
```
cmake "path to the opencv" ^
-G "Visual Studio 16 2019" ^
-D CMAKE_CONFIGURATION_TYPES="Release" ^
-D CMAKE_BUILD_TYPE="Release" ^
```

If CMAKE_CONFIGURATION_TYPES and CMAKE_BUILD_TYPE are not specified, a project file (vcxproj) containing multiple Configurations can be generated (only when generating for Visual Studio).
When building, you can use msbuild to build only RELEASE.

```
msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild
```

# WITH_GSTREAMER

ver.4.5.5

If you set the path to the GStreamer bin in the PATH, CMake will automatically detect the path to the library files. Maybe the environment variable GSTREAMER_1_0_ROOT_MSVC_X86_64 had an influence (not sure).
```
In PATH
    C:\gstreamer\1.0\msvc_x86_64\bin

GSTREAMER_1_0_ROOT_MSVC_X86_64 =
    C:\gstreamer\1.0\msvc_x86_64\
```

# WITH_MFX

```
In PATH
 C:\sw\IntelSWTools\Intel(R) Media SDK 2021 R1\Software Development Kit\bin\x64
 C:\sw\IntelSWTools\Intel(R) Media SDK 2021 R1\Software Development Kit\bin\win32

INTELMEDIASDKROOT =
 C:\sw\IntelSWTools\Intel(R) Media SDK 2021 R1\Software Development Kit\
```

# WITH_QT

ver.4.5.5

It is not clear if the environment variable Qt5_DIR had an effect.
```
In PATH
    C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin
Qt5_DIR =
    C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\lib\cmake\Qt5
```
