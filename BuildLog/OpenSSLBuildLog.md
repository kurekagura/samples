# build3.0.2

## [ Environment ]

strawberry-perl-5.32.1.1-64bit.msi
```
>where nasm && nasm --version
C:\sw\NASM\nasm.exe
NASM version 2.15.05 compiled on Aug 28 2020

>where perl && perl -v
C:\sw\Strawberry\perl\bin\perl.exe
This is perl 5, version 32, subversion 1 (v5.32.1) built for MSWin32-x64-multi-thread
```

## [ nmake ]

Using "x64 Native Tools Command Prompt for VS 2019"
```
git clone https://github.com/openssl/openssl.git
git checkout refs/tags/openssl-3.0.2
perl Configure VC-WIN64A --prefix=C:\lib\openssl-3.0.2\x64 --openssldir=C:\lib\openssl-3.0.2\ssl
nmake install
```

## [ Conclusion ]

The build was successful with no problems.

# build1.1.1n

## [ namke ]
```
git checkout refs/tags/OpenSSL_1_1_1n
perl Configure VC-WIN64A --prefix=C:\lib\openssl-1.1.1n\x64 --openssldir=C:\lib\openssl-1.1.1n\ssl
nmake
nmake install
```

## [ A trouble ]

The following error occurred.
```
Error: DECLARE_ASN1_DUP_FUNCTION does not have a number assigned
NMAKE : fatal error U1077: '"C:\sw\Strawberry\perl\bin\perl.exe"' : リターン コード '0x19'
Stop.
NMAKE : fatal error U1077: '"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\HostX64\x64\nmake.exe"' : リターン コード '0x2'
Stop.
```

I reverted back to 1.1.1l and tried the following with different options, but the problem did not occur.
```
git checkout refs/tags/OpenSSL_1_1_1l
perl Configure VC-WIN64A no-asm no-shared
nmake->success
namke clean

perl Configure VC-WIN64A no-asm
nmake->success
nmake clean

perl Configure VC-WIN64A no-shared
nmake->success
nmake clean

perl Configure VC-WIN64A
nmake->success
nmake clean

perl Configure VC-WIN64A --prefix=C:\lib\openssl-1.1.1l\x64 --openssldir=C:\lib\openssl-1.1.1l\ssl
nmake->success
```
Forwarded again to 1.1.1n and the error was no longer reproduced, most likely because I forgot to do `nmake clean` after changing tags from 3.0.3.

## [ Conclusion ]

The build was successful with no problems.

# Reference

- [【C++】OpenSSLのビルド、VS2019でインクルード](https://chigusa-web.com/blog/cpp-openssl-install/)
- [OpenSSLをビルドする for Windows (+ Visual Studio )](https://qiita.com/bonkoturyu/items/0d9ce584ca45e5a29ea6)
- [Visual Studio 2019 における OpenSSL のビルド](https://qiita.com/QGv/items/dbc3e8b70d1f9f6a2f6f)
