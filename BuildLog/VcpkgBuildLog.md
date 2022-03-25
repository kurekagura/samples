# Build & Setup
Run `x64 Native Tools Command Prompt for VS 2019` and
```
cd c:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```
vcpkg.exe is generated.

Run `x64 Native Tools Command Prompt for VS 2019` with administrator elevation and
```
**********************************************************************
** Visual Studio 2019 Developer Command Prompt v16.11.9
** Copyright (c) 2021 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'

C:\Windows\System32>cd c:\vcpkg

c:\vcpkg>.\vcpkg.exe integrate install
Applied user-wide integration for this vcpkg root.

All MSBuild C++ projects can now #include any installed libraries.
Linking will be handled automatically.
Installing new libraries will make them instantly available.

CMake projects should use:
 "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

```
c:\vcpkg>date /T
2022/03/23
c:\vcpkg>vcpkg.exe --version
Vcpkg package management program version
 2022-03-09-1affd32f93b299d5a907816c328ca3ededb73a7e
```
# Usage to install a package
```
c:\vcpkg>.\vcpkg install spdlog:x64-windows
```

# CMake in Qt Creator

Create a custom Kit and add CMAKE_TOOLCHAIN_FILE in CMake configuration, not in CMakeLists.txt. In CMakeLists.txt, just find_package and you will be able to find the package.

# What I don't understand (for Post-learning)

https://github.com/microsoft/vcpkg

- No description about VCPKG_ROOT. Arbitrary? 

See `vpckg --help`
```
--vcpkg-root=<path>
    Specify the vcpkg root directory(default: %VCPKG_ROOT%)
```


# Reference

https://kenkyu-note.hatenablog.com/entry/2021/05/01/181500
https://github.com/gabime/spdlog/blob/v1.x/example/CMakeLists.txt
