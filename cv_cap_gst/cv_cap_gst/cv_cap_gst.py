#-*- coding:utf-8 -*-
# Encode frames obtained by decoding H.264 directly to H.264.
# Use GStreamer's' pipline in OpenCV.
import cv2

print(cv2.getBuildInformation())

decoderElement = "d3d11h264device1dec"
input_mp4 = True

encoderElement = "x264enc";
output_mp4 = True

gst_cmd_cap = 'filesrc location="C:\\\\dev\\\\samplevideo\\\\input.{0}" {1} !h264parse !{2} !videoconvert !appsink' \
	.format("mp4" if input_mp4 else "246", "!qtdemux" if input_mp4 else "", decoderElement)

capture = cv2.VideoCapture(gst_cmd_cap, cv2.CAP_GSTREAMER)

if not capture.isOpened():
	raise Exception("Failed : to open VideoCapture with CAP_GSTREAMER")

width = capture.get(cv2.CAP_PROP_FRAME_WIDTH)
height = capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
count = capture.get(cv2.CAP_PROP_FRAME_COUNT)
fourcc = capture.get(cv2.CAP_PROP_FOURCC)
fps = capture.get(cv2.CAP_PROP_FPS)

out_width = width;
out_height = height;
out_fourcc = cv2.VideoWriter_fourcc('H', '2', '6', '4');
out_fps = fps;
out_size = (int(width),int(height))

outputPath = 'C:\\\\dev\\\\samplevideo\\\\out-cv_cap_gst_py-by-{0}.{1}'\
    .format(encoderElement, "mp4" if output_mp4 else "264")

conv_to_encoder = ''
if encoderElement == "x264enc":
    conv_to_encoder = "!video/x-raw,format=I420"

gst_cmd_writer = 'appsrc !videoconvert {0} !{1} {2} !filesink location="{3}"'\
    .format(conv_to_encoder, encoderElement, "!qtmux" if output_mp4 else "", outputPath)

writer = cv2.VideoWriter(gst_cmd_writer, cv2.CAP_GSTREAMER, out_fourcc, out_fps, out_size, True)
if not writer.isOpened:
    raise Exception("Failed : to open VideoWriter with CAP_GSTREAMER");

windName = "Dec by {0} Enc by {1} in py".format(decoderElement, encoderElement)
while True:
    ret, cvmat = capture.read()
    if cvmat is None:
        break

    cv2.imshow(windName, cvmat)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    if writer.isOpened():
        writer.write(cvmat)

cv2.destroyAllWindows()
capture.release()

if writer.isOpened():
    writer.release()
