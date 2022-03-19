# My Latest

```
rmdir c:\lib\poco-1.11.1
mklink /d c:\lib\poco-1.11.1 c:\lib\poco-1.11.1c
```
# build1.11.1c

## [ Summary ]

In addition to build1.11.1b, build the following, `NetSSL_Win`.

## [ CMake ]

Using `[x64 Native Tools Command Prompt for VS 2019]`
```
set BUILDID=1.11.1c
mkdir build%BUILDID% && cd build%BUILDID%

* Important
set OPENSSL_ROOT_DIR=C:/lib/openssl-1.1.1n/x64

cmake .. ^
-G "Visual Studio 16 2019" ^
-D CMAKE_INSTALL_PREFIX=C:/lib/poco-%BUILDID% ^
-D ENABLE_NETSSL_WIN=ON

msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild && ^
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild

cmake ".." -LA > build%BUILDID%-CMakeCachedVars.txt 2>&1
```
[build1.11.1c-CMakeCachedVars.txt](build1.11.1c-CMakeCachedVars.txt)

# build1.11.1b

## [ Summary ]

In addition to build1.11.1a, build the followings, `JWT, NetSSL_OpenSSL, Building: Crypto`.
These can be enabled using `OpenSSL`.

## [ CMake ]

Using `[x64 Native Tools Command Prompt for VS 2019]`
```
git checkout refs/tags/poco-1.11.1-release

set BUILDID=1.11.1b
mkdir build%BUILDID% && cd build%BUILDID%

set OPENSSL_ROOT_DIR=C:/lib/openssl-1.1.1n/x64

cmake .. ^
-G "Visual Studio 16 2019" ^
-D CMAKE_INSTALL_PREFIX=C:/lib/poco-%BUILDID%

msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild && ^
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild
```

```
cmake ".." -LA > build%BUILDID%-CMakeCachedVars.txt 2>&1
```
[build1.11.1b-CMakeCachedVars.txt](./build1.11.1b-CMakeCachedVars.txt)

## [ Conclusion ]

If the environment variable `OPENSSL_ROOT_DIR` is set to the OpenSSL path, the path to `libssl.lib` and `libcrypto.lib` and `OPENSSL_INCLUDE_DIR` are automatically detected and the following cmake variable is set to ON.

https://cmake.org/cmake/help/v3.7/module/FindOpenSSL.html

```
ENABLE_CRYPTO:BOOL=ON
ENABLE_JWT:BOOL=ON
ENABLE_NET:BOOL=ON
ENABLE_NETSSL:BOOL=ON
```
I used Self-Build of OpenSSL 1.1.1n ([See](../OpenSSLBuildLog.md)).

# build1.11.1a

## [ CMake ]

Using `[x64 Native Tools Command Prompt for VS 2019]`
```
git checkout refs/tags/poco-1.11.1-release

set BUILDID=1.11.1a
mkdir build%BUILDID% && cd build%BUILDID%

cmake .. ^
-G "Visual Studio 16 2019" ^
-D CMAKE_INSTALL_PREFIX=C:/lib/poco-%BUILDID%

msbuild INSTALL.vcxproj /p:Configuration="debug" /target:ReBuild && ^
msbuild INSTALL.vcxproj /p:Configuration="release" /target:ReBuild
```

```
cmake ".." -LA > build%BUILDID%-CMakeCachedVars.txt 2>&1
```
[build1.11.1a-CMakeCachedVars.txt](./build1.11.1a-CMakeCachedVars.txt)

# Binary Package Install

```
O:\>o:\src\vcpkg\vcpkg.exe install POCO
Computing installation plan...
warning: vcpkg appears to be in a Visual Studio prompt targeting x64
but is installing packages for x86-windows.
Consider using --triplet x64-windows or --triplet x64-uwp.

o:\src\vcpkg\vcpkg.exe install POCO --triplet x64-windows
```
