# Reference
- [PeterL1n/RobustVideoMatting](https://github.com/PeterL1n/RobustVideoMatting)

[Revsion]
```
RobustVideoMatting>git log -n 1
commit 81a10937c73f68eeddb863221c61fe6a60a1cca2 (HEAD -> master, origin/master, origin/HEAD)
Author: Peter Lin <peterlin9863@gmail.com>
Date:   Thu Jan 13 22:36:37 2022 -0800
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

# Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda create --name RVMcu117 python=3.9
conda activate RVMcu117
#python -V
#Python 3.9.13

#1.13.0 with specified versions to avoid some unintentional updates.
conda install pytorch==1.13.0 torchvision==0.14.0 torchaudio==0.13.0 pytorch-cuda=11.7 -c pytorch -c nvidia -y

conda install tqdm==4.64.1 -y
conda install av==10.0.0 -c conda-forge -y
conda install pims==0.5 -y
conda install scikit-image==0.19.2 -y
```

Export conda yml
```
conda env export -n RVMcu117 > conda_RVM+cu117.yml

#Edit
#  - blas=1.0=mkl
#to
#  - defaults::blas=1.0=mkl
```

# Install it.
```
# cd <base_dir>
mkdir PeterL1n && cd PeterL1n
git clone https://github.com/PeterL1n/RobustVideoMatting.git
cd RobustVideoMatting
#There is no need to install
#pip install -v -e .
```

# Setup (use conda YAML)
Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda env create –f conda_RVM+cu117.yml --name RVMcu117YML
```

# Problems

## #1 Error
[ImportError: cannot import name 'Iterable' from 'collections' in Python](https://stackoverflow.com/questions/72032032/importerror-cannot-import-name-iterable-from-collections-in-python)

It is deprecated in Python3.10. I used Python3.9 => Resolved.

## #2 Warning
```
lib\site-packages\pims\image_reader.py:26: RuntimeWarning: PIMS image_reader.py could not find scikit-image.
Falling back to matplotlib's imread(), which uses floats instead of integers.This may break your scripts.
(To ignore this warning, include the line "warnings.simplefilter("ignore", RuntimeWarning)" in your script.)
  warnings.warn(RuntimeWarning(ski_preferred))
```
conda install scikit-image==0.19.2 => Resolved.

# What is it.
## [PyAV(av)](https://github.com/PyAV-Org/PyAV)
>PyAV is a Pythonic binding for the FFmpeg libraries.  (…) PyAV is for direct and precise access to your media via containers, streams, packets, codecs, and frames. It exposes a few transformations of that data, and helps you get your data to/from other packages (e.g. Numpy and Pillow). This power does come with some responsibility as working with media is horrendously complicated and PyAV can't abstract it away or make all the best decisions for you. If the ffmpeg command does the job without you bending over backwards, PyAV is likely going to be more of a hindrance than a help.

## [pims](https://github.com/soft-matter/pims)
>Scientific video can be packaged in various ways: familiar video formats like .AVI and .MOV, folders full of numbered images, or "stacks" of TIFF images. Each of these requires a separate Python module. And, once loaded, they have different methods for accessing individual images, looping through the images in bulk, accessing a specific range, or dealing with multidimensional files. <ins>PIMS can do all of these using a consistent interface, handling the differences between different inputs invisibly.</ins>
