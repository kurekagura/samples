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

pip install openmim==0.3.2

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
#set DISTUTILS_USE_SDK=1
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
