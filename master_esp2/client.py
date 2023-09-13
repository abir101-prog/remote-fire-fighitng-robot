import requests
import json
import cv2
import urllib.request
import numpy as np
import time
# from utils import *

baseURL = 'http://192.168.77.170'
dataURL = ':81/fire'
imgURL = ':80/cam-hi.jpg'
startURL = ':81/start'

# POST REQUEST
def post(url, data):
    headers = {
        "Content-Type": "application/json"
    }
    x = requests.post(url, data=json.dumps(data), headers=headers) #json = myobj)
    print(x.content)

def get(url):
    x = requests.get(url)
    print(x.content)

fire = False
cv2.namedWindow("live Cam Testing", cv2.WINDOW_AUTOSIZE)

# Create a VideoCapture object
cap = cv2.VideoCapture(baseURL+imgURL)

# Check if the IP camera stream is opened successfully
if not cap.isOpened():
    print("Failed to open the IP camera stream")
    exit()

# Read and display video frames
while True:

    frame=urllib.request.urlopen(baseURL+imgURL)
    # ret, frame = cap.read()
    frame = np.array(bytearray(frame.read()),dtype=np.uint8)
    frame = cv2.imdecode(frame,-1)
    frame = cv2.flip(frame, -1)  # since the camera is flipped


    # frame = cv2.resize(frame, (700,500))  # emnei choto
    blur = cv2.GaussianBlur(frame, (15,15), 0)  #(15, 15)
    hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

    lower = [0,150,150] #[0 150 150]
    upper = [30,255,255]  # [30,255,255]
    lower = np.array(lower, dtype='uint8')
    upper = np.array(upper, dtype='uint8')
    mask = cv2.inRange(hsv,lower,upper)
    # temp = frame[:,:,0] > frame[:,:,1]
    # temp = temp.astype(int)
    # temp = temp >frame[:,:,2]
    # temp = temp.astype(int)
    # temp = np.array(temp, dtype='uint8')
    # print(mask.dtype, temp.dtype)
    # mask = cv2.bitwise_and(mask, temp)
    output = cv2.bitwise_and(frame,hsv,mask=mask)
    number_of_total = cv2.countNonZero(mask)
    if int(number_of_total) > 200:
        fire = True

#################
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    max_area = 0
    max_bb = None
    for contour in contours:
        x,y,w,h = cv2.boundingRect(contour)  # get bb of contour
        if max_area > w*h:
            continue
        max_area = w*h
        max_bb = (x, y, w, h)
    # send a post req to esp about the location of fire
    # show image here
    # might have to put a minimum size requirement to call it a fire
    if max_bb is not None:
        x, y, w, h = max_bb
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
    
    cv2.imshow("Result", frame)

    
    if fire:
        # SEND A POST REQUEST
        post(baseURL+dataURL, {'fire': 1, 'centerx': x+w/2, 'img_width': frame.shape[1]}) # horizontal center, total width
        print("detected")
        fire = False
    else:
        post(baseURL+dataURL, {'fire': 0, 'centerx': 0}) # horizontal center
        print('NOT detected............')


    # img_corny = corner_detection(img)

    # Blur the image for better edge detection
    # img_blur = cv2.GaussianBlur(img, (3,3), 0) 
    # img_edgy = cv2.Canny(image=img_blur, threshold1=100, threshold2=200) # Canny Edge Detection

    # concatenate image Horizontally
    # Hori = np.concatenate((img_corny, img_edgy), axis=1)
    # cv2.imshow('live Cam Testing',img)
    time.sleep(.2)

    key=cv2.waitKey(5)
    if key==ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

