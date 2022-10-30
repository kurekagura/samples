# Reference

[products]
- [facebookresearch/detectron2](https://github.com/facebookresearch/detectron2)

[others]
  - [Detectron2をwindowsで動かす](https://qiita.com/techstrom/items/0b00c0fd0ab0b1519a98)

# Environment

```
>systeminfo |findstr /B /C:"OS Name" /B /C:"OS"
OS 名:                  Microsoft Windows 10 Pro
OS バージョン:          10.0.19045 N/A ビルド 19045

>conda --version
conda 22.9.0
```

```
>where nvcc.exe
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\bin\nvcc.exe
>where nvvp.exe
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\libnvvp\nvvp.exe
```

```
>nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2022 NVIDIA Corporation
Built on Wed_Jun__8_16:59:34_Pacific_Daylight_Time_2022
Cuda compilation tools, release 11.7, V11.7.99
Build cuda_11.7.r11.7/compiler.31442593_0
```
```
PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\include\cudnn_version.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2

> (omit)cudnn_version.h:57:#define CUDNN_MAJOR 8
  (omit)cudnn_version.h:58:#define CUDNN_MINOR 5
  (omit)cudnn_version.h:59:#define CUDNN_PATCHLEVEL 0
```

## Revsion
```
detectron2>git log -n 1
commit 32bd159d7263683e39bf4e87e5c4ac88bad2fd73 (HEAD -> main, origin/main, origin/HEAD)
Author: Zecheng He <zechengh@meta.com>
Date:   Wed Oct 19 22:09:54 2022 -0700
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda create -n Detectron2 python=3.10.6 -y
conda activate Detectron2

conda install cython=0.29.32 -y
conda install pycocotools=2.0.4 -c conda-forge -y

#stable(1.13.0)
conda install pytorch=1.13.0 pytorch-cuda=11.7 pytorch-mutex=1.0 torchaudio=0.13.0 torchvision=0.14.0 blas=1.0 -c pytorch -c nvidia -y

#conda list blas -f
#blas 1.0 mkl

#conda list |findstr "torch"
#pytorch  1.13.0  py3.10_cuda11.7_cudnn8_0  pytorch
#pytorch-cuda 11.7  h67b0de4_0  pytorch
#pytorch-mutex  1.0 cuda  pytorch
#torchaudio 0.13.0  pypi_0  pypi
#torchvision  0.14.0  pypi_0  pypi

pip install opencv-python==4.6.0.66

# cd <base_dir>
mkdir facebookresearch && cd facebookresearch
git clone https://github.com/facebookresearch/detectron2.git

set DISTUTILS_USE_SDK=1
pip install -e detectron2
```

```
conda env export -n Detectron2 > O:\dev\kurekagura\samples\SetupGuide\Detectron2\conda_Detectron2.yml
```

# GPU Version Setup (use conda YAML)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_Detectron2.yml --name Detectron2
conda activate Detectron2
```

# Verify the installation
```
cd detectron2\tests
curl -OL https://raw.githubusercontent.com/yogeshkumarpilli/detectron2/master/tests/test_window.py
python .\test_window.py
```
