# Reference

- [akTwelve/Mask_RCNN](https://github.com/akTwelve/Mask_RCNN)
>Python 3.6, TensorFlow 2.0, and other common packages listed in requirements.txt.

## Revsion
```
Mask_RCNN>git log -n 1
commit d9f01dcd83b1e90e5ea0b4b6d5ba753770e114ac (HEAD -> master, origin/master, origin/HEAD)
Merge: 068572c 29c2d9c
Author: Adam Kelly <ak12@outlook.com>
Date:   Fri Jun 19 14:34:02 2020 -0500
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
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\bin\nvcc.exe
>where nvvp.exe
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\libnvvp\nvvp.exe
>nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2019 NVIDIA Corporation
Built on Sun_Jul_28_19:12:52_Pacific_Daylight_Time_2019
Cuda compilation tools, release 10.1, V10.1.243
```
```
PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.1\include\cudnn_version.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2

> (omit)cudnn_version.h:57:#define CUDNN_MAJOR 8
  (omit)cudnn_version.h:58:#define CUDNN_MINOR 0
  (omit)cudnn_version.h:59:#define CUDNN_PATCHLEVEL 5
```
⇒　インストール前でも動作してたような気がするが…

# Setup
Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda create -n akTwelve-Mask_RCNN python=3.7 -y
conda activate akTwelve-Mask_RCNN
pip install -r requirements-specifiedversion.txt
#Require ""
pip install "git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI"
```

```
conda env export -n akTwelve-Mask_RCNN > conda_akTwelve-Mask_RCNN.yml

#Edit
#  - pycocotools==2.0
#to
#
```

# Setup (use my conda YAML)
Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_akTwelve-Mask_RCNN.yml --name akTwelve-Mask_RCNNYML
conda activate akTwelve-Mask_RCNNYML
pip install "git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI"
```

# Install akTwelve/Mask_RCNN
```
# At <base_dir>
mkdir akTwelve && cd akTwelve
git clone https://github.com/akTwelve/Mask_RCNN.git
cd Mask_RCNN
pip install -e .
#Successfully installed mask-rcnn-2.1
```

# Verify the installation

1. Download pre-trained COCO weights (mask_rcnn_coco.h5) from the [releases page](https://github.com/matterport/Mask_RCNN/releases). Save it into "<base_dir>/akTwelve/Mask_RCNN/"

2. Run samples/demo.ipynb on jupyter-notebook

# Troubles

## pip install -r (my) requirements.txt
```
cython==0.29.32
git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI
```
順序が保証されていないためかエラーになる。あとで手動でインストールすることで回避する。
```
>pip install "git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI"
```
## ModuleNotFoundError: No module named 'tensorflow'

```
>jypter-notebook
# run demo.ipynb
```
pip install jupyter==1.0.0 into this conda env. =>resolved.

## AttributeError: module 'brotli' has no attribute 'error'
```
>jypter-notebook
# run demo.ipynb
```
pip install brotli==1.0.9 into this conda env.=>resolved.

## Could not load dynamic library 'cudart64_101.dll'

```
# tensorflow 2.2.0
from tensorflow.python.client import device_lib

Could not load dynamic library 'cudart64_101.dll'; dlerror: cudart64_101.dll not found
```
Changed CUDA v11.7 to v10.1. In my case, set `CUDA_PATH=%CUDA_PATH_V10_1%`. => resolved.

## 検出結果の表示がおかしい

```
visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], class_names, r['scores'])
```
オーバーレイ表示されるはずのBBoxやMaskの表示がおかしい（細い線状になっていて枠になっていない）。

numpy 1.21.6以外で発生する。1.21.6以外の幾つかの他のバージョンで確認したが`正常表示されたバージョンは1.21.6`のみだった（表示のみの問題か検出が不正かは未調査）。

# What is it.

## [imgaug](https://github.com/aleju/imgaug)
>This python library helps you with augmenting images for your machine learning projects. It converts a set of input images into a new, much larger set of slightly altered images.

[Docs](https://imgaug.readthedocs.io/en/latest/)

## [h5py](https://github.com/h5py/h5py)
>h5py is a thin, pythonic wrapper around HDF5, which runs on Python 3 (3.6+).

[Docs](https://docs.h5py.org/en/stable/index.html)

## [brotli](https://github.com/google/brotli)
>Brotli is a generic-purpose lossless compression algorithm that compresses data using a combination of a modern variant of the LZ77 algorithm, Huffman coding and 2nd order context modeling, with a compression ratio comparable to the best currently available general-purpose compression methods.
