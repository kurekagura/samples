# Reference

[mainly]
- [open-mmlab/mmdetection](https://github.com/open-mmlab/mmdetection)
- [MMDetection Doc](https://mmdetection.readthedocs.io/en/stable/get_started.html)
- [Compatibility issue between MMCV and MMDetection](https://mmdetection.readthedocs.io/en/stable/faq.html)

```
master	mmcv-full>=1.3.17, \<1.6.0
2.25.1	mmcv-full>=1.3.17, \<1.6.0
```

[others]
- [MMDetectionを使って物体検出モデルの推論をサクッと行う【PyTorch】](https://touch-sp.hatenablog.com/entry/2021/10/26/113906)

[Revsion]
```
mmdetection>git checkout tags/v2.25.3

mmdetection>git log -n 1
commit e71b499608e9c3ccd4211e7c815fa20eeedf18a2 (HEAD, tag: v2.25.3, origin/master, origin/HEAD, master)
Author: RangiLyu <lyuchqi@gmail.com>
Date:   Tue Oct 25 16:06:32 2022 +0800
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
  (omit)#define CUDNN_MINOR 5
  (omit)#define CUDNN_PATCHLEVEL 0
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda create --name MMDetection python=3.10.6 -y
conda activate MMDetection

conda install pytorch=1.13.0 pytorch-cuda=11.7 pytorch-mutex=1.0 torchaudio=0.13.0 torchvision=0.14.0 blas=1.0 -c pytorch -c nvidia -y

conda install opencv=4.6.0 -y

pip install openmim

set DISTUTILS_USE_SDK=1
pip install mmcv-full==1.5.3 -f https://download.openmmlab.com/mmcv/dist/cu117/torch1.13.0/index.html
```

Export conda yml
```
conda env export -n MMDetection > conda_MMDetection.yml

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
git checkout tags/v2.25.3
pip install -v -e .
```

# GPU Version Setup (use conda YAML)
Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_MMDetection.yml --name MMDetection
conda activate MMDetection
```

# Verify the installation
```
# At mmdetection On conda
mim download mmdet --config yolov3_mobilenetv2_320_300e_coco --dest .
#yolov3_mobilenetv2_320_300e_coco.py
#yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth

python demo/image_demo.py demo/demo.jpg yolov3_mobilenetv2_320_300e_coco.py yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth --device cpu --out-file result.jpg
```











```
conda create --name MMDetection3 python=3.10 -y
conda activate MMDetection3

conda install pytorch==1.12.1 torchvision==0.13.1 torchaudio==0.12.1 cudatoolkit=11.6 -c pytorch -c conda-forge
pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html
pip install openmim
conda install opencv

cd mmdetect2
pip install -v -e .
=>OK
```

```
conda create --name MMDetection2 python=3.9.13 -y
conda activate MMDetection2

#pip install torch torchvision --extra-index-url https://download.pytorch.org/whl/cu116
#torch        1.13.0+cu116             pypi_0    pypi
#torchvision  0.14.0+cu116             pypi_0    pypi
pip install torch==1.12.0 torchvision --extra-index-url https://download.pytorch.org/whl/cu116

pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html
pip install mmdet
pip install openmim
pip install mmengine

mmcv-full          1.6.1
mmdet              2.25.3
openmim            0.3.2
mmengine           0.2.0
=>OK
```


```
pip install mmcv-full==1.4.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html

～残骸～
conda create --name MMDetection1 python=3.8 -y
conda activate MMDetection1

#stable(1.13.0)
conda install pytorch torchvision torchaudio pytorch-cuda=11.7 -c pytorch -c nvidia

pip install -U openmim

mim install mmcv-full
pip install mmcv-full==1.1.3
->Error

pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu117/torch1.13.0/index.html
->NG

pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html
->OK

pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.13.0/index.html

git clone https://github.com/open-mmlab/mmdetection.git
pip install -v -e mmdetection

pip install mmdet

```

～残骸～

```
pip install mmcv-full==1.6.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html

conda install pytorch torchvision torchaudio cudatoolkit=11.6 -c pytorch -c conda-forge

# cd <base_dir>
mkdir open-mmlab
git clone https://github.com/open-mmlab/mmdetection.git
cd mmdetection
pip install -v -e .
# "-e" means installing a project in editable mode,
# thus any local modifications made to the code will take effect without reinstallation.

cd ..
git clone https://github.com/open-mmlab/mim.git
cd mim
pip install -e .
mim install mmengine
mim install mmcv-full
```

# Troubles

## error while installing mmcv-full

mim install mmcv-full

```
× Running setup.py install for mmcv-full did not run successfully.
  │ exit code: 1
  ╰─> [1229 lines of output]
      No CUDA runtime is found, using CUDA_HOME='C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6'
```

set CUDA_HOME="C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6"


# GPU Version Setup (use conda YAML)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_MMDetection.yml --name MMDetection
conda activate MMDetection

x pip install opencv-python==4.0.0.21

# cd <base_dir>

mkdir open-mmlab
git clone https://github.com/open-mmlab/mmdetection.git
cd mmdetection
```

# About

## MMCV

https://github.com/open-mmlab/mmcv

MMCV is a foundational library for computer vision research and it provides the following functionalities:

Universal IO APIs
Image/Video processing
Image and annotation visualization
Useful utilities (progress bar, timer, ...)
PyTorch runner with hooking mechanism
Various CNN architectures
High-quality implementation of common CPU and CUDA ops

## MIM

https://github.com/open-mmlab/mim

MIM provides a unified interface for launching and installing OpenMMLab projects and their extensions, and managing the OpenMMLab model zoo.

Major Features
Package Management

You can use MIM to manage OpenMMLab codebases, install or uninstall them conveniently.

Model Management

You can use MIM to manage OpenMMLab model zoo, e.g., download checkpoints by name, search checkpoints that meet specific criteria.

Unified Entrypoint for Scripts

You can execute any script provided by all OpenMMLab codebases with unified commands. Train, test and inference become easier than ever. Besides, you can use gridsearch command for vanilla hyper-parameter search.

->openmim?

## mmdet

# Tips

```
pip install -U
-U, --upgrade
Upgrade all specified packages to the newest available version.
The handling of dependencies depends on the upgrade-strategy used.
```

```
pip install -v -e .
# "-v" means verbose, or more output
# "-e" means installing a project in editable mode,
# thus any local modifications made to the code will take effect without reinstallation.
```

```
(MMDetection2) O:\src\open-mmlab\mmdetection>pip install mmcv-full==1.14.1 -f https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html
Looking in links: https://download.openmmlab.com/mmcv/dist/cu116/torch1.12.0/index.html
ERROR: Could not find a version that satisfies the requirement mmcv-full==1.14.1 (from versions: 1.0rc0, 1.0rc2, 1.0.0, 1.0.1, 1.0.2, 1.0.3, 1.0.4, 1.0.5, 1.1.0, 1.1.1, 1.1.2, 1.1.3, 1.1.4, 1.1.5, 1.1.6, 1.2.0, 1.2.1, 1.2.2, 1.2.3, 1.2.4, 1.2.5, 1.2.6, 1.2.7, 1.3.0, 1.3.1, 1.3.3, 1.3.4, 1.3.5, 1.3.6, 1.3.7, 1.3.8, 1.3.9, 1.3.10, 1.3.11, 1.3.12, 1.3.13, 1.3.14, 1.3.15, 1.3.16, 1.3.17, 1.3.18, 1.4.0, 1.4.1, 1.4.2, 1.4.3, 1.4.4, 1.4.5, 1.4.6, 1.4.7, 1.4.8, 1.5.0, 1.5.1, 1.5.2, 1.5.3, 1.6.0, 1.6.1, 1.6.2)
ERROR: No matching distribution found for mmcv-full==1.14.1
```