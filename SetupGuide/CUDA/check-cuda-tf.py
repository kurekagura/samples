import sys
import tensorflow as tf

print(f"sys.version => {sys.version}")
print(f"tf.__version__ => {tf.__version__}")

#deprecated
#is_gpu = tf.test.is_gpu_available()
#print(f"tf.test.is_gpu_available() => {is_gpu}")

print("tf.config.list_physical_devices('GPU') => ")
gpu_devices = tf.config.list_physical_devices('GPU')

if gpu_devices:
    print("tf.python.client.device_lib.list_local_devices() => ")
    tf.python.client.device_lib.list_local_devices()
