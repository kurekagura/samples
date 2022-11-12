# Conclusion
The reason why it did not work is mentioned at the end.

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
  2.23.0	mmcv-full>=1.3.17, \<1.5.0 ←Target(NG)
  ```

[Revsion]
```
# v2.23.0=2.23.final
mmdetection>git checkout tags/v2.23.0

mmdetection>git log -n 1
commit 3e2693151add9b5d6db99b944da020cba837266b (HEAD, tag: v2.23.0)
Merge: 6b87ac22 bab144cd
Author: Wenwei Zhang <40779233+ZwwWayne@users.noreply.github.com>
Date:   Wed Mar 30 09:42:05 2022 +0800
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
conda create --name MMDetection2.23cu117 python=3.7.15 -y
conda activate MMDetection2.23cu117

#1.9.1 Official Command
#conda install pytorch==1.9.1 torchvision==0.10.1 torchaudio==0.9.1 #cudatoolkit=11.3 -c pytorch -c conda-forge
#conda install pytorch==1.9.1 torchvision==0.10.1 torchaudio==0.9.1 cudatoolkit=11.7 blas==2.116 -c pytorch -c conda-forge -y

#1.13.0(stable) Official Command
conda install pytorch torchvision torchaudio pytorch-cuda=11.7 -c pytorch -c nvidia -y

pip install opencv-python==4.6.0.66

pip install openmim==0.3.2

#memo 2.23.0 mmcv-full>=1.3.17, \<1.5.0
set DISTUTILS_USE_SDK=1
pip install mmcv-full==1.5.0
```

Export conda yml
```
conda env export -n MMDetection2.23cu117 > conda_MMDetection2.23+cu117.yml
```

# Install MMDetection.
```
# cd <base_dir>
mkdir open-mmlab && cd open-mmlab
git clone https://github.com/open-mmlab/mmdetection.git
cd mmdetection
git checkout tags/v2.23.0
pip install -v -e .
```

# GPU Version Setup (use conda YAML)
Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda env create –f conda_MMDetection2.23+cu117.yml --name MMDetection2.23cu117YML

* 仮想環境名に+を使うのは控えること。
```

# Verify the installation
```
# At mmdetection on conda

mim download mmdet --config yolov3_mobilenetv2_320_300e_coco --dest ./chkp
#chkp/yolov3_mobilenetv2_320_300e_coco.py
#chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth

python demo/image_demo.py --device cpu demo/demo.jpg chkp/yolov3_mobilenetv2_320_300e_coco.py chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth
#->will open a window with some detected bboxes.

# To use GPU.
python demo/image_demo.py --device cuda:0 demo/demo.jpg chkp/yolov3_mobilenetv2_320_300e_coco.py chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth

#WebCam demo.
mim download mmdet --config faster_rcnn_r50_fpn_1x_coco --dest ./chkp
python demo/webcam_demo.py --device cuda:0 chkp/faster_rcnn_r50_fpn_1x_coco.py chkp/faster_rcnn_r50_fpn_1x_coco_20200130-047c8118.pth
#CVウィンドウが開いて閉じるを繰り返すのは不具合のよう。
#opencv-python==4.3.0.36～4.6.0.66で試したが同じ現象。これ以下のバージョンではエラーが発生しウィンドウが開かない。

#Video
python demo/video_demo.py --device cuda:0 demo/demo.mp4 chkp/faster_rcnn_r50_fpn_1x_coco.py chkp/faster_rcnn_r50_fpn_1x_coco_20200130-047c8118.pth --out result.mp4

#MASK R-CNN
mim download mmdet --config mask_rcnn_r50_fpn_1x_coco --dest ./chkp
python demo/image_demo.py --device cuda:0 demo/demo.jpg chkp/mask_rcnn_r50_fpn_1x_coco.py chkp/mask_rcnn_r50_fpn_1x_coco_20200205-d4b0c5d6.pth

mim download mmdet --config mask_rcnn_x101_64x4d_fpn_2x_coco --dest ./chkp
python demo/image_demo.py --device cuda:0 demo/demo.jpg chkp/mask_rcnn_x101_64x4d_fpn_2x_coco.py chkp/mask_rcnn_x101_64x4d_fpn_2x_coco_20200509_224208-39d6f70c.pth
#mimダウンロードが途中で失敗している場合は、エラーになる。再度ダウンロードすると解決した。
```

# Troubles

## #1

```
python demo/image_demo.py --device cpu demo/demo.jpg chkp/yolov3_mobilenetv2_320_300e_coco.py chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth
=>
o CUDA runtime is found, using CUDA_HOME='C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7'
Traceback (most recent call last):
  File "demo/image_demo.py", line 5, in <module>
    from mmdet.apis import (async_inference_detector, inference_detector,
  (...ommit...)
  File "o:\sw\Anaconda3\envs\MMDetection2.23cu117YML\lib\site-packages\mmcv\utils\ext_loader.py", line 13, in load_ext
    ext = importlib.import_module('mmcv.' + name)
  File "o:\sw\Anaconda3\envs\MMDetection2.23cu117YML\lib\importlib\__init__.py", line 127, in import_module
    return _bootstrap._gcd_import(name[level:], package, level)
ImportError: DLL load failed: 指定されたモジュールが見つかりません。
```
Replaced mmcv-full==1.4.8 to 15.0.
=> Resolved. (Note:1.5.1 ~ 1.5.3(1.5.final) is incompatible)

## #2

```
python demo/image_demo.py --device cuda:0 demo/demo.jpg chkp/yolov3_mobilenetv2_320_300e_coco.py chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth

No CUDA runtime is found, using CUDA_HOME='C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7'
load checkpoint from local path: chkp/yolov3_mobilenetv2_320_300e_coco_20210719_215349-d18dff72.pth
Traceback (most recent call last):
  File "demo/image_demo.py", line 65, in <module>
    main(args)
  (...ommit...)
  File "o:\sw\Anaconda3\envs\MMDetection2.23cu117\lib\site-packages\torch\cuda\__init__.py", line 166, in _lazy_init
    raise AssertionError("Torch not compiled with CUDA enabled")
AssertionError: Torch not compiled with CUDA enabled
```
torch.cuda.is_available()がFalseを戻すことを確認。「pytorch==1.9.1はcuda11.7に未対応」と思われたのでpytorchバージョンを変更。
```
conda install pytorch==1.9.1 torchvision==0.10.1 torchaudio==0.9.1 cudatoolkit=11.7 blas==2.116 -c pytorch -c conda-forge -y
↓
conda install pytorch torchvision torchaudio pytorch-cuda=11.7 -c pytorch -c nvidia -y
# 今度は、#1と同じエラー。
```
結論：cuda11.7に対応しているpytorchは1.13.0(1.13.latest)のみである。しかしこのバージョンで、mmcvエラーが発生する。mmdet 2.23.0 は mmcv-full>=1.3.17, \<1.5.0の依存があると明記されている（但し、#1で確認した通り、実際はmmcv-full==15.0のみで正常稼働）。mmdetとmmcvをバージョンアップするしかなさそうである。

mmdet:2.23.0(x), 2.24.0, 2.24.1, 2.25.0, 2.25.1, 2.25.2, 2.25.3, 3.0.0rc0, 3.0.0rc1, 3.0.0rc2, 3.0.0rc3

mmcv-full:1.5.0(x), 1.5.1, 1.5.2, 1.5.3, 1.6.0, 1.6.1, 1.6.2, 1.7.0, 2.0.0rc1, 2.0.0rc2

https://download.pytorch.org/whl/torch_stable.html

```
pip install torch==x
1.7.0, 1.7.1, 
1.8.0, 1.8.1, 
1.9.0, 1.9.1(x), 
1.10.0, 1.10.1, 
1.10.2, 
1.11.0, 
1.12.0, 1.12.1, 
1.13.0(x)
```
