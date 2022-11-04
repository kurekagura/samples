# Reference

- [Okery/PyTorch-Simple-MaskRCNN](https://github.com/Okery/PyTorch-Simple-MaskRCNN)

## Revsion
```
PyTorch-Simple-MaskRCNN>git log -n 1
commit 21e6c6983b34061800280573ebe705ae17212972 (HEAD -> master, origin/master, origin/HEAD)
Author: Xin Wu <47376448+Okery@users.noreply.github.com>
Date:   Fri Oct 29 21:01:33 2021 +0800
```

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

# Installation
Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda create -n PyTorch-Simple-MaskRCNN python=3.9.13 -y
conda activate PyTorch-Simple-MaskRCNN

#latest stable(1.13.0)
conda install pytorch=1.13.0 pytorch-cuda=11.7 pytorch-mutex=1.0 torchaudio=0.13.0 torchvision=0.14.0 blas=1.0 -c pytorch -c nvidia -y

conda install opencv=4.6.0 -y

#conda install cython=0.29.32 -y
conda install pycocotools=2.0.4 -c conda-forge -y

conda install matplotlib=3.5.3 -y
conda install jupyter=1.0.0 -y

#cd <base_dir>
mkdir Okery && cd Okery
git clone https://github.com/Okery/PyTorch-Simple-MaskRCNN.git
cd PyTorch-Simple-MaskRCNN
# Don't need to install.
```

```
conda env export -n PyTorch-Simple-MaskRCNN > conda_PyTorch-Simple-MaskRCNN.yml
#Edit
#  - blas=1.0=mkl
#  - winpty=0.4.3=4
#to
#  - defaults::blas=1.0=mkl
#  - defaults::winpty=0.4.3=4
```

# Installation (use my conda YAML)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_PyTorch-Simple-MaskRCNN.yml --name PyTorch-Simple-MaskRCNNYML
conda activate PyTorch-Simple-MaskRCNNYML
```

# Verify the installation

```
#Prepare coco dataset and run tain.py
>python train.py --use-cuda --iters 200 --dataset coco --data-dir O:\coco\coco2017
#produced 4 files.
#maskrcnn_coco-1.pth
#maskrcnn_coco-2.pth
#maskrcnn_coco-3.pth
#maskrcnn_results.pth

#Update demo.ipynb for my settings like this.
dataset = "coco"
ckpt_path = "maskrcnn_coco-3.pth"
data_dir = "O:\coco\coco2017"
ds = pmr.datasets(dataset, data_dir, "val2017", train=True)
#Runt it on jypyter-notebook
```

# Troubles

## 1. Got the following error in the code using torchvision.

This error appears to occur when installing from YML.

>NotImplementedError: Could not run 'torchvision::nms' with arguments from the 'CUDA' backend. This could be because the operator doesn't exist for this backend, or was omitted during the selective/custom build process (if using custom build). If you are a Facebook employee using PyTorch on mobile, please visit https://fburl.com/ptmfixes for possible resolutions. 'torchvision::nms' is only available for these backends: [CPU, QuantizedCPU, BackendSelect, Python, FuncTorchDynamicLayerBackMode, Functionalize, Named, Conjugate, Negative, ZeroTensor, ADInplaceOrView, AutogradOther, AutogradCPU, AutogradCUDA, AutogradXLA, AutogradMPS, AutogradXPU, AutogradHPU, AutogradLazy, Tracer, AutocastCPU, AutocastCUDA, FuncTorchBatched, FuncTorchVmapMode, Batched, VmapMode, FuncTorchGradWrapper, PythonTLSSnapshot, FuncTorchDynamicLayerFrontMode, PythonDispatcher].

```
>pip install torchvision==0.14.0 --force-reinstall
```
=> resolved.

[How to solve this error? RuntimeError: Could not run 'torchvision::nms' with arguments from the 'CUDA' backend #305](https://github.com/pytorch/vision/issues/3058)