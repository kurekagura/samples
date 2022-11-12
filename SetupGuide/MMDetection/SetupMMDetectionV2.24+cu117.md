# Reference

[mainly]
- [open-mmlab/mmdetection](https://github.com/open-mmlab/mmdetection)
- [MMDetection Doc](https://mmdetection.readthedocs.io/en/stable/get_started.html)
  ```
  MMDetection works on Linux, Windows and macOS. It requires Python 3.6+, CUDA 9.2+ and PyTorch 1.5+.
  ```
- [Compatibility issue between MMCV and MMDetection](https://mmdetection.readthedocs.io/en/stable/faq.html)

  ```
  master	mmcv-full>=1.3.17, \<1.6.0
  2.25.1	mmcv-full>=1.3.17, \<1.6.0
  2.23.0	mmcv-full>=1.3.17, \<1.5.0 ←cuda117では動作不可を確認済 
  2.24.0	mmcv-full>=1.3.17, \<1.6.0 ←Target
  ```

[Revsion]
```
# v2.24.0
mmdetection>git checkout tags/v2.24.0

mmdetection>git log -n 1
commit 1376e77e6ecbaad609f6003725158de24ed42e84 (HEAD, tag: v2.24.0)
Merge: c72bc707 7d1c0970
Author: Wenwei Zhang <40779233+ZwwWayne@users.noreply.github.com>
Date:   Tue Apr 26 21:14:55 2022 +0800
```

# Environment
```
>systeminfo |findstr /B /C:"OS Name" /B /C:"OS"
OS 名:                  Microsoft Windows 10 Pro
OS バージョン:          10.0.19045 N/A ビルド 19045

>ver
Microsoft Windows [Version 10.0.19045.2251]

>conda -V
conda 22.9.0
```
```
>where nvcc && where nvvp
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\bin\nvcc.exe
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\bin\nvvp.bat
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
> (omit)#define CUDNN_MAJOR 8
  (omit)#define CUDNN_MINOR 4
  (omit)#define CUDNN_PATCHLEVEL 1
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda create --name MMDetection2.24cu117 python=3.7.15 -y
conda activate MMDetection2.24cu117

#1.13.0(stable) with specified versions.
conda install pytorch==1.13.0 torchvision==0.14.0 torchaudio==0.13.0 pytorch-cuda=11.7 blas==1.0 -c pytorch -c nvidia -y

pip install opencv-python==4.6.0.66

pip install openmim==0.3.2

#memo 2.24.0 mmcv-full>=1.3.17, \<1.6.0
#set DISTUTILS_USE_SDK=1
pip install mmcv-full==1.6.0
```

Export conda yml
```
conda env export -n MMDetection2.24cu117 > conda_MMDetection2.24+cu117.yml

#Edit
#  - blas=1.0=mkl
#to
#  - defaults::blas=1.0=mkl
```

# Install MMDetection.
```
# cd <base_dir>
mkdir open-mmlab && cd open-mmlab
git clone https://github.com/open-mmlab/mmdetection.git
cd mmdetection
git checkout tags/v2.24.0
pip install -v -e .
```

# GPU Version Setup (use conda YAML)
Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda env create –f conda_MMDetection2.24+cu117.yml --name MMDetection2.24cu117YML
```

# Verify the installation
```
# At mmdetection on conda
mim download mmdet --config mask_rcnn_r50_fpn_1x_coco --dest ./chkp

# MASK R-CNN ON CPU ON GPU
python demo/image_demo.py --device cpu demo/demo.jpg chkp/mask_rcnn_r50_fpn_1x_coco.py chkp/mask_rcnn_r50_fpn_1x_coco_20200205-d4b0c5d6.pth
python demo/image_demo.py --device cuda:0 demo/demo.jpg chkp/mask_rcnn_r50_fpn_1x_coco.py chkp/mask_rcnn_r50_fpn_1x_coco_20200205-d4b0c5d6.pth
```

# memo
```
conda list |findstr "pytorch torchvision torchaudio pytorch-cuda blas"
```
```
#1.13.0(stable)
#Official Command
conda install pytorch torchvision torchaudio pytorch-cuda=11.7 -c pytorch -c nvidia

#specified version for the future.
conda install pytorch==1.13.0 torchvision==0.14.0 torchaudio==0.13.0 pytorch-cuda=11.7 blas==1.0 -c pytorch -c nvidia

blas                      1.0                         mkl
libcublas                 11.11.3.6                     0    nvidia
libcublas-dev             11.11.3.6                     0    nvidia
pytorch                   1.13.0          py3.7_cuda11.7_cudnn8_0    pytorch
pytorch-cuda              11.7                 h67b0de4_0    pytorch
pytorch-mutex             1.0                        cuda    pytorch
torchaudio                0.13.0                   pypi_0    pypi
torchvision               0.14.0                   pypi_0    pypi
```

```
conda uninstall pytorch torchvision torchaudio pytorch-cuda
```

```
#pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu117
```
