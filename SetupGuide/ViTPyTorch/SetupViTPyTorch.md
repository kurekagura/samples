# ViT PyTorch
[References]

- https://github.com/lukemelas/PyTorch-Pretrained-ViT

[Revision I have used]
```
>git log -n 1
commit 8ec2cab38b2dfae6386f37d4879a325f552eb791 (HEAD -> master, origin/master, origin/HEAD)
Author: Luke <luke.melas@gmail.com>
Date:   Sun Nov 8 17:47:54 2020 -0500
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
Start 'x64 Native Tools Command Prompt for VS 2022'.
```
>conda create -n ViTPyTorch python=3
>conda activate ViTPyTorch
(ViTPyTorch)>python -V
Python 3.10.4
(ViTPyTorch)>conda install pytorch torchvision torchaudio cudatoolkit=11.6 -c pytorch -c conda-forge
(ViTPyTorch)>conda list pytorch
# Name          Version     Build  Channel
pytorch         1.12.1      py3.10_cuda11.6_cudnn8_0    pytorch
pytorch-mutex   1.0         cuda    pytorch

(ViTPyTorch)>conda install jupyter

# At the <your_base_dir>
(ViTPyTorch)>mkdir lukemelas && cd lukemelas
(ViTPyTorch)>git clone https://github.com/lukemelas/ViT-PyTorch
(ViTPyTorch)>cd ViT-PyTorch
(ViTPyTorch)>pip install -e .
```

Confirm the execution
```
(ViTPyTorch)>jypyter notebook
# Got to expample/simple
# Open and Run example.ipynb
```

# Train
See:　examples/imagenet/README.md

But I haven't tried it yet.

# memo
```
conda env export -n ViTPyTorch > conda_vitpytorch.yml
```