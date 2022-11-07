"""
Modified: https://github.com/akTwelve/Mask_RCNN/blob/master/samples/demo.ipynb
"""
import os
import sys
import random
import matplotlib
import matplotlib.pyplot as plt
import mrcnn.model as mrcnn_model
import mrcnn.visualize as mrcnn_viz
import skimage.io as skio

ROOT_DIR = os.path.abspath("./")

sys.path.append(ROOT_DIR)  # To find local version of the library
sys.path.append(os.path.join(ROOT_DIR, "samples/coco/"))  # To import local coco config.
print(f"=======================>{matplotlib.get_backend()}")
import coco
print(f"=======================>{matplotlib.get_backend()}")
class InferenceConfig(coco.CocoConfig):
    # Set batch size to 1 since we'll be running inference on
    # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1

# COCO Class names
# For example, to get ID of　the teddy bear class, use: class_names.index('teddy bear')
class_names = ['BG', 'person', 'bicycle', 'car', 'motorcycle', 'airplane',
                'bus', 'train', 'truck', 'boat', 'traffic light',
                'fire hydrant', 'stop sign', 'parking meter', 'bench', 'bird',
                'cat', 'dog', 'horse', 'sheep', 'cow', 'elephant', 'bear',
                'zebra', 'giraffe', 'backpack', 'umbrella', 'handbag', 'tie',
                'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball',
                'kite', 'baseball bat', 'baseball glove', 'skateboard',
                'surfboard', 'tennis racket', 'bottle', 'wine glass', 'cup',
                'fork', 'knife', 'spoon', 'bowl', 'banana', 'apple',
                'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza',
                'donut', 'cake', 'chair', 'couch', 'potted plant', 'bed',
                'dining table', 'toilet', 'tv', 'laptop', 'mouse', 'remote',
                'keyboard', 'cell phone', 'microwave', 'oven', 'toaster',
                'sink', 'refrigerator', 'book', 'clock', 'vase', 'scissors',
                'teddy bear', 'hair drier', 'toothbrush']

def create_detector(coco_model_path):
    config = InferenceConfig()
    config.display()
    model = mrcnn_model.MaskRCNN(mode="inference", model_dir="", config=config)
    model.load_weights(coco_model_path, by_name=True)   # Load weights trained on MS-COCO
    return model

if __name__ == "__main__":
    matplotlib.use('TkAgg')

    coco_model_path = os.path.join(ROOT_DIR, "mask_rcnn_coco.h5")   # Local path to trained weights file
    if not os.path.exists(coco_model_path):
        utils.download_trained_weights(coco_model_path)

    image_dir = os.path.join(ROOT_DIR, "images")    # Directory of images to run detection on
    file_names = next(os.walk(image_dir))[2]        # Load a random image from the images folder
    image = skio.imread(os.path.join(image_dir, random.choice(file_names)))

    detector = create_detector(coco_model_path)

    results = detector.detect([image], verbose=1)
    r = results[0]
    
    fig = plt.figure()
    ax1 = fig.add_subplot(1, 1, 1)

    mrcnn_viz.display_instances(image, r['rois'], r['masks'], r['class_ids'], class_names, r['scores'], ax=ax1)
    fig.tight_layout()
    plt.show(block=True)
