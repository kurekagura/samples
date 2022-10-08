# Whisper
[Reference]

- https://github.com/openai/whisper

>We used Python 3.9.9 and PyTorch 1.10.1 to train and test our models, but the codebase is expected to be compatible with Python 3.7 or later and recent PyTorch versions. The codebase also depends on a few Python packages, most notably HuggingFace Transformers for their fast tokenizer implementation and ffmpeg-python for reading audio files. The following command will pull and install the latest commit from this repository, along with its Python dependencies

- [WhisperとGoogle Colaboratoryで音声の文字起こしをやってみた](https://zenn.dev/kazuki_tam/articles/d59250ecf25628)

[Checkout I used]
```
>git log -n 1
commit 9e653bd0ea0f1e9493cb4939733e9de249493cfb (HEAD -> main, origin/main, origin/HEAD)
Author: Corentin Jemine <corentin.jemine@gmail.com>
Date:   Tue Oct 4 17:49:31 2022 +0200
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
>conda create -y -n Whisper python=3.9
>conda activate Whisper
(Whisper)>python -V
Python 3.9.13

(Whisper)>conda install -y pytorch==1.10.1 torchvision==0.11.2 torchaudio==0.10.1 cudatoolkit=11.3 -c pytorch -c conda-forge
```

Setup Whisper 
```
# At <your_base_dir>
(Whisper)>mkdir openai && cd openai
(Whisper)>git clone https://github.com/openai/whisper.git
(Whisper)>cd whisper
(Whisper)>pip install -e .
```

[FFmpeg]  

I installed it myself from [gyan.dev](https://www.gyan.dev/ffmpeg/builds/)
```
(Whisper) >ffmpeg -version
ffmpeg version 5.1.2-full_build-www.gyan.dev Copyright (c) 2000-2022 the FFmpeg developers
built with gcc 12.1.0 (Rev2, Built by MSYS2 project)
```

e.g.) Your Project(Visual Studio 2022)->Debug->Environment variables
```
PATH=C:\lib\ffmpeg-5.1.2-full_build\bin
```

# Setup by conda yml
```
>conda env create -f conda_Wisper.yml -n <your_env_name>
```
After that, setup Whisper.

# Workaround

## No.1

When I run my main.py in VS2022 Python Project, got this errer.
```
UnicodeDecodeError: 'utf-8' codec can't decode byte
```
=> Save my main.py as UTF-8

## No.2
whisper.load_audio gives an error.
```
Message=[WinError 2] 指定されたファイルが見つかりません。
Source=O:\dev\kurekagura\samples\SetupGuide\Whisper\WhisperVS\main.py
(Current frame)
  audio = whisper.load_audio(r"sample.m4a")
```
=> Path to ffmpeg.exe

## No.3
Cannot debug my main.py (but can run without debugging) in Visual Sutdio 2022 Python Project.

Whisper (0.2.0,64-bit) was displayed. The correct term is Whisper (3.9.13,64 bit).

`ffmpeg-python==0.2.0` seems to be the cause (VS2022 bug?).
```
conda install ffmpeg-python=0.2.0 -c conda-forge
```
It reproduced it when I installed it. 

In Visual Studio 2019, it works for me.
