# Reference

[mainly]
- matterport/Mask_RCNN
  
  https://github.com/matterport/Mask_RCNN

- philferriere/cocoapi

  https://github.com/philferriere/cocoapi

- augmentedstartups/mask-rcnn

  https://github.com/augmentedstartups/mask-rcnn

- taisuke-ito/samples2
  
  https://github.com/taisuke-ito/samples2


[others]
  - https://github.com/akTwelve/Mask_RCNN

[Revsion]
```
matterport\Mask_RCNN>git log -n 1
commit 3deaec5d902d16e1daf56b62d5971d428dc920bc (HEAD -> master, origin/master, origin/HEAD)
Author: cclauss <cclauss@me.com>
Date:   Sat Mar 9 19:20:31 2019 +0100

matterport\Mask_RCNN\cocoapi>git log -n 1
commit 2929bd2ef6b451054755dfd7ceb09278f935f7ad (HEAD -> master, origin/master, origin/HEAD)
Author: Phil Ferriere <pferriere@hotmail.com>
Date:   Wed Oct 24 16:00:13 2018 -0700

augmentedstartups\mask-rcnn>git log -n 1
commit 2da4169011719abe83ba1e339faca573b8af3497 (HEAD -> master, origin/master, origin/HEAD)
Author: Augmented Startups <reigngt09@gmail.com>
Date:   Mon Mar 1 08:53:40 2021 +0200
```

# Environment

```
>systeminfo |findstr /B /C:"OS Name" /B /C:"OS"
OS 名:                  Microsoft Windows 10 Pro
OS バージョン:          10.0.19043 N/A ビルド 19043

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

# GPU Version Setup (use conda YAML)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
# cd <base_dir>

conda env create –f conda_matterport@Mask_RCNN.yml --name matterport-Mask_RCNNYML
conda activate matterport-Mask_RCNNYML

pip install opencv-python==4.0.0.21

#git clone https://github.com/philferriere/cocoapi.git
#cd cocoapi\PythonAPI
#python setup.py build_ext install
pip install "git+https://github.com/philferriere/cocoapi.git#egg=pycocotools&subdirectory=PythonAPI"

mkdir matterport && cd matterport
git clone https://github.com/matterport/Mask_RCNN.git
cd Mask_RCNN
python setup.py install

jupyter-notebook
# go to samples dir.
# run demo.py
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
# cd <base_dir>

conda create --name MaskRCNN_GPU python=3.6
conda activate MaskRCNN_GPU

conda install tensorflow=1.13.1=gpu_py36h1635174_0
#numpy                     1.19.2           py36hadc3359_0
#numpy-base                1.19.2
#In order to prevent a lot of warnings, the version has been downgraded.
conda install numpy=1.16.2=py36h19fb1c0_0
conda install keras=2.2.4=0
conda install jupyter=1.0.0=py36_7
conda install -c conda-forge imgaug=0.3
# The #numpy version is not upgraded.
#matplotlib                3.3.2            py36ha15d459_1    conda-forge
#matplotlib-base           3.3.2            py36h856a30b_0    conda-forge
#numpy                     1.16.2           py36h19fb1c0_0
#numpy-base                1.16.2           py36hc3f5095_0
#opencv

pip install opencv-python==4.0.0.21

# At the <base_dir>
mkdir matterport && cd matterport
git clone https://github.com/matterport/Mask_RCNN.git

cd Mask_RCNN
python setup.py install

# Get cocoapi from modified (setup. py) repository
git clone https://github.com/philferriere/cocoapi.git
cd cocoapi\PythonAPI
python setup.py build_ext install
```

# CPU Version Setup

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
# cd <base_dir>
mkdir augmentedstartups && cd augmentedstartups
git clone https://github.com/augmentedstartups/mask-rcnn.git
cd "mask-rcnn\7. Anaconda Setup"
conda env create –f mask_rcnn.yml --name MaskRCNN

#pip installed them at end of. yaml
#- pip:
#  - imgaug==0.2.8
#  - opencv-python==4.0.0.21
#  - pycocotools==2.0

# But closed with pycocotools related error (ignored here)
conda activate MaskRCNN

# You know the pycocotools installation is failing
pip list | findstr "imgaug opencv-python pycocotools"

# Back to the <base_dir>
mkdir matterport && cd matterport
git clone https://github.com/matterport/Mask_RCNN.git

cd Mask_RCNN
python setup.py install

# Get cocoapi from modified (setup. py) repository
git clone https://github.com/philferriere/cocoapi.git
cd cocoapi\PythonAPI
python setup.py build_ext install

# Go to samples and launch the demo.ipynb.
# At the beginning, mask_rcnn_coco.h5 is downloaded.
jupyter notebook
```

# Other samples
```
# Other samples by Ito-san
# At <Mask_RCNN> dir.
git clone https://github.com/taisuke-ito/samples2.git
```
