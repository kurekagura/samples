# Switch CUDA Toolkit to other version

Need to change this one to switch.
```
CUDA_PATH=%CUDA_PATH_V11_7%
```

To verify.
```
>where nvcc
>where nvvp
```

## Examples

```
CUDA_PATH=%CUDA_PATH_V?_?%

CUDNN_PATH=%CUDA_PATH%

Path=%CUDA_PATH%\bin;%CUDA_PATH%\libnvvp;<others>

CUDA_PATH_V10_0=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0
CUDA_PATH_V10_1=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1
CUDA_PATH_V10_2=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2
CUDA_PATH_V11_1=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1
CUDA_PATH_V11_3=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3
CUDA_PATH_V11_5=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5
CUDA_PATH_V11_6=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6
CUDA_PATH_V11_7=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7
```

# Verification
>nvcc -V
```
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2022 NVIDIA Corporation
Built on Tue_Mar__8_18:36:24_Pacific_Standard_Time_2022
Cuda compilation tools, release 11.6, V11.6.124
Build cuda_11.6.r11.6/compiler.31057947_0
```

CUDNNは、ヘッダーファイル名がバージョンによって異なるので注意。

CUDNN7以前：`cudnn.h`  
CUDNN8以降：`cudnn_version.h`
```
# CUDNN7以前
PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.?\include\cudnn.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2

> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\include\cudnn.h:57:#define CUDNN_MAJOR 7
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\include\cudnn.h:58:#define CUDNN_MINOR 6
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\include\cudnn.h:59:#define CUDNN_PATCHLEVEL 5
```
```
# CUDNN8以降
PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.?\include\cudnn_version.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2

> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:57:#define CUDNN_MAJOR 8
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:58:#define CUDNN_MINOR 4
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:59:#define CUDNN_PATCHLEVEL 1
```

# Trouble①

v10.0（cuda_10.0.130_411.31_win10.exe）,v10.2（cuda_10.2.89_441.22_win10.exe→cuda_10.2.1_win10.exe→cuda_10.2.2_win10.exe）  
がインストール済みの状態で、v10.1（cuda_10.1.243_426.00_win10.exe）をインストールしようとしたところ、展開後の初期画面（システム互換性のチェック）が延々と終わらない状態になり（３時間近く待ってみたが・・・）、先に進むことが出来なかった。v11.x（v11.1,v11.3,v11.5,v11.6,v11.7）もインストール済みの状態。

そこで、v10.2をアンインストールした後に再度試したところ、正常にインストールできた。但し、この時もシステム互換性チェックで２時間くらい待たされた。v10.2のアンインストールが関係したかどうかは謎である。

その後、v10.2を再度インストールすることができた（先述の3つのexeを順次インストール。この時は互換性のチェックは数秒で終了）。

※旧バージョンはカスタムでCUDAのみをインストール。

# What is it?

## cusolver64_11.dll

[tensorflow-nightly-gpu looking for cusolver64_10.dll on a cuDNN 11.1 installation #44291](https://github.com/tensorflow/tensorflow/issues/44291)
>renaming cusolver64_11.dll to cusolver64_10.dll
