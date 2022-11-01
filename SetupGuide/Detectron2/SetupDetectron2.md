# Reference

[products]
- [facebookresearch/detectron2](https://github.com/facebookresearch/detectron2)

[others]
  - [Detectron2をwindowsで動かす](https://qiita.com/techstrom/items/0b00c0fd0ab0b1519a98)

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
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.7\bin\nvcc.exe
>where nvvp.exe
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

> (omit)cudnn_version.h:57:#define CUDNN_MAJOR 8
  (omit)cudnn_version.h:58:#define CUDNN_MINOR 5
  (omit)cudnn_version.h:59:#define CUDNN_PATCHLEVEL 0
```

## Revsion
```
detectron2>git log -n 1
commit 32bd159d7263683e39bf4e87e5c4ac88bad2fd73 (HEAD -> main, origin/main, origin/HEAD)
Author: Zecheng He <zechengh@meta.com>
Date:   Wed Oct 19 22:09:54 2022 -0700
```

# GPU Version Setup (manually)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda create -n Detectron2 python=3.10.6 -y
conda activate Detectron2

conda install cython=0.29.32 -y
conda install pycocotools=2.0.4 -c conda-forge -y

#stable(1.13.0)
conda install pytorch=1.13.0 pytorch-cuda=11.7 pytorch-mutex=1.0 torchaudio=0.13.0 torchvision=0.14.0 blas=1.0 -c pytorch -c nvidia -y

#conda list blas
#blas 1.0 mkl
#libblas  3.9.0 8_mkl conda-forge
#libcblas 3.9.0 8_mkl conda-forge
#libcublas  11.11.3.6 0 nvidia
#libcublas-dev  11.11.3.6 0 nvidia

#conda list |findstr "torch"
#pytorch  1.13.0  py3.10_cuda11.7_cudnn8_0  pytorch
#pytorch-cuda 11.7  h67b0de4_0  pytorch
#pytorch-mutex  1.0 cuda  pytorch
#torchaudio 0.13.0  pypi_0  pypi
#torchvision  0.14.0  pypi_0  pypi

pip install opencv-python==4.6.0.66
```
```
conda env export -n Detectron2 > O:\dev\kurekagura\samples\SetupGuide\Detectron2\conda_Detectron2.yml

#Edit
#  - blas=1.0=mkl
#to
#  - defaults::blas=1.0=mkl
```

# Install detectron2
```
# cd <base_dir>
mkdir facebookresearch && cd facebookresearch
git clone https://github.com/facebookresearch/detectron2.git

set DISTUTILS_USE_SDK=1
pip install -e detectron2
```

# GPU Version Setup (use conda YAML)

Start 'x64 Native Tools Command Prompt for VS 2022'.
```
conda env create –f conda_Detectron2.yml --name Detectron2
conda activate Detectron2
```

# Verify the installation
```
cd detectron2\tests
curl -OL https://raw.githubusercontent.com/yogeshkumarpilli/detectron2/master/tests/test_window.py
python .\test_window.py
```

# Issues

## #1

demo.py doesn't detect anything (on Windows, by the way).

-> Uninvestigated
```
demo>curl -O http://images.cocodataset.org/val2017/000000439715.jpg
demo>python demo.py --config-file ../configs/COCO-InstanceSegmentation/mask_rcnn_R_50_FPN_3x.yaml --input 000000439715.jpg
000000439715.jpg: detected 0 instances in 0.95s
```

## #2

An error occurs in the environment configured from yml. I compared 'conda list' and found to be identical.

-> Unknown cause. Not resolved.
```
>python test_window.py

Traceback (most recent call last):
  File "O:\src\facebookresearch\detectron2\tests\test_window.py", line 29, in <module>
    output = predictor(image)
  File "o:\src\facebookresearch\detectron2\detectron2\engine\defaults.py", line 317, in __call__
    predictions = self.model([inputs])[0]
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torch\nn\modules\module.py", line 1190, in _call_impl
    return forward_call(*input, **kwargs)
  File "o:\src\facebookresearch\detectron2\detectron2\modeling\meta_arch\rcnn.py", line 150, in forward
    return self.inference(batched_inputs)
  File "o:\src\facebookresearch\detectron2\detectron2\modeling\meta_arch\rcnn.py", line 208, in inference
    proposals, _ = self.proposal_generator(images, features, None)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torch\nn\modules\module.py", line 1190, in _call_impl
    return forward_call(*input, **kwargs)
  File "o:\src\facebookresearch\detectron2\detectron2\modeling\proposal_generator\rpn.py", line 477, in forward
    proposals = self.predict_proposals(
  File "o:\src\facebookresearch\detectron2\detectron2\modeling\proposal_generator\rpn.py", line 503, in predict_proposals
    return find_top_rpn_proposals(
  File "o:\src\facebookresearch\detectron2\detectron2\modeling\proposal_generator\proposal_utils.py", line 121, in find_top_rpn_proposals
    keep = batched_nms(boxes.tensor, scores_per_img, lvl, nms_thresh)
  File "o:\src\facebookresearch\detectron2\detectron2\layers\nms.py", line 20, in batched_nms
    return box_ops.batched_nms(boxes.float(), scores, idxs, iou_threshold)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torchvision\ops\boxes.py", line 75, in batched_nms
    return _batched_nms_coordinate_trick(boxes, scores, idxs, iou_threshold)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torch\jit\_trace.py", line 1136, in wrapper
    return fn(*args, **kwargs)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torchvision\ops\boxes.py", line 94, in _batched_nms_coordinate_trick
    keep = nms(boxes_for_nms, scores, iou_threshold)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torchvision\ops\boxes.py", line 41, in nms
    return torch.ops.torchvision.nms(boxes, scores, iou_threshold)
  File "o:\sw\Anaconda3\envs\Detectron2YML\lib\site-packages\torch\_ops.py", line 442, in __call__
    return self._op(*args, **kwargs or {})
NotImplementedError: Could not run 'torchvision::nms' with arguments from the 'CUDA' backend. This could be because the operator doesn't exist for this backend, or was omitted during the selective/custom build process (if using custom build). If you are a Facebook employee using PyTorch on mobile, please visit https://fburl.com/ptmfixes for possible resolutions. 'torchvision::nms' is only available for these backends: [CPU, QuantizedCPU, BackendSelect, Python, FuncTorchDynamicLayerBackMode, Functionalize, Named, Conjugate, Negative, ZeroTensor, ADInplaceOrView, AutogradOther, AutogradCPU, AutogradCUDA, AutogradXLA, AutogradMPS, AutogradXPU, AutogradHPU, AutogradLazy, Tracer, AutocastCPU, AutocastCUDA, FuncTorchBatched, FuncTorchVmapMode, Batched, VmapMode, FuncTorchGradWrapper, PythonTLSSnapshot, FuncTorchDynamicLayerFrontMode, PythonDispatcher].
```
