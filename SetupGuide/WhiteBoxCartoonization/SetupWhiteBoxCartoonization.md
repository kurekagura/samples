# White-box-Cartoonization
[Reference]

- https://github.com/SystemErrorWang/White-box-Cartoonization
- https://systemerrorwang.github.io/White-box-Cartoonization/

>Assume you already have NVIDIA GPU and CUDA CuDNN installed
>Install tensorflow-gpu, we tested 1.12.0 and 1.13.0rc0
>Install scikit-image==0.14.5, other versions may cause problems

[Checkout I used]
```
git log -n 1
commit c136bdfe930afeaa7e66ff1609496b743441abb9 (HEAD -> master, origin/master, origin/HEAD)
Author: lllyasviel <914847518@qq.com>
Date:   Tue Jun 28 01:40:59 2022 +0800
```

# Environment
```
>systeminfo |findstr /B /C:"OS Name" /B /C:"OS"
OS 名:                  Microsoft Windows 10 Pro
OS バージョン:          10.0.19043 N/A ビルド 19043

>conda --version
conda 22.9.0
```

# Setup
```
>conda create -n WBC python=3.6
>conda activate WBC
(WBC)>python -V
Python 3.6.13 :: Anaconda, Inc.

(WBC)>conda install tensorflow=1.13.1=gpu_py36h1635174_0
(WBC)>conda install scikit-image=0.15.0
(WBC)>conda install tqdm=4.63.0

(WBC)>conda list numpy
# Name                    Version                   Build  Channel
numpy                     1.19.2           py36hadc3359_0
numpy-base                1.19.2           py36ha3acd2a_0

(WBC)conda install numpy=1.16

(WBC)>pip install opencv-python
(WBC)>conda list opencv-python
# Name                    Version                   Build  Channel
opencv-python             4.6.0.66                 pypi_0    pypi

# At <your_base_dir>
(WBC)>mkdir SystemErrorWang && cd SystemErrorWang
(WBC)>git clone https://github.com/SystemErrorWang/White-box-Cartoonization.git

# cd to test_code
(WBC)>python cartoonize.py
# Files under test_ images are processed, and results are generated under cartoonized_images.
```

See: [Inference with Pre-trained Model](https://github.com/SystemErrorWang/White-box-Cartoonization#inference-with-pre-trained-model)

# Warnings
A large number of warnings were output when numpy==1.19.2, but they disappeared when the version was down to 1.16.
```
Anaconda3\envs\Cartoonize\lib\site-packages\tensorflow\python\framework\dtypes.py:526: 
FutureWarning: Passing (type, 1) or '1type' as a synonym of type is deprecated; in a future version of numpy, 
it will be understood as (type, (1,)) / '(1,)type'.
  _np_qint8 = np.dtype([("qint8", np.int8, 1)])
```

# Setup by conda yml
```
>conda env create -f conda_WhiteBoxCartoonization.yml -n <your_env_name>
```

# Other links

- https://github.com/nagadomi/lbpcascade_animeface
- [[論文読み]Learning to Cartoonize Using White-box Cartoon Representations - 写真をイラスト化するAI](https://tech.fusic.co.jp/posts/2020-08-17-ai-cv-gan-cartoonize-with-white-box-rep/)
