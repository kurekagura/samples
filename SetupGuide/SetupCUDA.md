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

## Examaples
```
CUDA_PATH=%CUDA_PATH_V?_?%

CUDNN_PATH=%CUDA_PATH%

CUDA_PATH_V10_2=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2
CUDA_PATH_V11_1=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1
CUDA_PATH_V11_5=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5
CUDA_PATH_V11_6=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6
CUDA_PATH_V11_7=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7

Path=%CUDA_PATH%\bin;%CUDA_PATH%\libnvvp;<others>
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

PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v?.?\include\cudnn_version.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2
```
> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:57:#define CUDNN_MAJOR 8
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:58:#define CUDNN_MINOR 4
  C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include\cudnn_version.h:59:#define CUDNN_PATCHLEVEL 1
```