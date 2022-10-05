import os
import json
from PIL import Image
import torch
from torchvision import transforms

print(os.getcwd())

# Load ViT
from pytorch_pretrained_vit import ViT
model = ViT('B_16_imagenet1k', pretrained=True)
model.eval()

img_path = r'O:\src\lukemelas\ViT-PyTorch\examples\simple\img.jpg'
# Load image
# NOTE: Assumes an image `img.jpg` exists in the current directory
img = transforms.Compose([
    transforms.Resize((384, 384)), 
    transforms.ToTensor(),
    transforms.Normalize(0.5, 0.5),
])(Image.open(img_path)).unsqueeze(0)
print(img.shape) # torch.Size([1, 3, 384, 384])

# Classify
with torch.no_grad():
    outputs = model(img)

print(outputs.shape)  # (1, 1000)