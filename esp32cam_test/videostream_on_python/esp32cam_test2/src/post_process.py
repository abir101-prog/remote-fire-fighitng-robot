import cv2
import urllib.request
import numpy as np
from utils import *

# Replace the URL with the IP camera's stream URL
url = 'http://192.168.0.104/cam-lo.jpg'
cv2.namedWindow("live Cam Testing", cv2.WINDOW_AUTOSIZE)


# Create a VideoCapture object
cap = cv2.VideoCapture(url)

# Check if the IP camera stream is opened successfully
if not cap.isOpened():
    print("Failed to open the IP camera stream")
    exit()

# Read and display video frames
while True:
    # Read a frame from the video stream
    img_resp=urllib.request.urlopen(url)
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    #ret, frame = cap.read()
    img = cv2.imdecode(imgnp, -1)  # 0 for grayscale. -1 for default


    # img_corny = corner_detection(img)

    # Blur the image for better edge detection
    # img_blur = cv2.GaussianBlur(img, (3,3), 0) 
    # img_edgy = cv2.Canny(image=img_blur, threshold1=100, threshold2=200) # Canny Edge Detection

    # concatenate image Horizontally
    # Hori = np.concatenate((img_corny, img_edgy), axis=1)
    cv2.imshow('live Cam Testing',img)

    key=cv2.waitKey(5)
    if key==ord('q'):
        break
    

cap.release()
cv2.destroyAllWindows()