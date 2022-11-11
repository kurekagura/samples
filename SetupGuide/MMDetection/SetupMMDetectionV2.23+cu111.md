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
  2.23.0	mmcv-full>=1.3.17, \<1.5.0 ←Target
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

>conda -V
conda 22.9.0
```
```
>where nvcc && where nvvp
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\bin\nvcc.exe
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\bin\nvvp.bat
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\libnvvp\nvvp.exe
```
```
>nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2020 NVIDIA Corporation
Built on Mon_Oct_12_20:54:10_Pacific_Daylight_Time_2020
Cuda compilation tools, release 11.1, V11.1.105
Build cuda_11.1.relgpu_drvr455TC455_06.29190527_0
```
```
PS>Select-String -Path "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include\cudnn_version.h" -Pattern "#define CUDNN_MAJOR" -Context 0,2
> (omit)#define CUDNN_MAJOR 8
  (omit)#define CUDNN_MINOR 2
  (omit)#define CUDNN_PATCHLEVEL 0
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2017'.
```
conda create --name MMDetection2.23 python=3.7.15 -y
conda activate MMDetection2.23

#1.9.1 Official Command
#conda install pytorch==1.9.1 torchvision==0.10.1 torchaudio==0.9.1 cudatoolkit=11.3 -c pytorch -c conda-forge
conda install pytorch==1.9.1 torchvision==0.10.1 torchaudio==0.9.1 cudatoolkit=11.1 blas==2.116 -c pytorch -c conda-forge -y

pip install opencv-python==4.6.0.66

pip install openmim==0.3.2

#memo 2.23.0 mmcv-full>=1.3.17, \<1.5.0
set DISTUTILS_USE_SDK=1
pip install mmcv-full==1.4.8 -f https://download.openmmlab.com/mmcv/dist/cu111/torch1.9.1/index.html
```

Export conda yml
```
conda env export -n MMDetection2.23 > conda_MMDetection2.23+cu111.yml
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
conda env create –f conda_MMDetection2.23+cu111.yml --name MMDetection2.23YML
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
