#-*- coding:utf-8 -*-
# Encode frames obtained by decoding H.264 directly to H.264.
# Use Intel QSV (MFX) as hardware decoder.
# Encoder uses MSMF (Microsoft Media Foundation).
import cv2

filepath = 'C:\\dev\\samplevideo\\Big_Buck_Bunny_360_10s_1MB.264'

capture = cv2.VideoCapture(filepath, cv2.CAP_INTEL_MFX)

width = capture.get(cv2.CAP_PROP_FRAME_WIDTH)
height = capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
count = capture.get(cv2.CAP_PROP_FRAME_COUNT)
fourcc = capture.get(cv2.CAP_PROP_FOURCC)
fps = capture.get(cv2.CAP_PROP_FPS)

out_fourcc = cv2.VideoWriter_fourcc('H', '2', '6', '4')
out_fps = 30

out_size = (int(width),int(height))

writer = cv2.VideoWriter('C:\\dev\\samplevideo\\out_CAP_MSMF_by_py.mp4', cv2.CAP_MSMF, out_fourcc, out_fps, out_size, True)

if writer is None:
    print('Error : to open VideoWriter')

while(capture.isOpened()):
    ret, frame = capture.read()
    if frame is None:
        break

    cv2.imshow("cv_cap_mfx.py-window", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
        
    if writer is not None:
        writer.write(frame)

capture.release()
cv2.destroyAllWindows()
