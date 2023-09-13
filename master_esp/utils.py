import cv2
import numpy as np   
import requests
import json


# POST REQUEST
# data = {
#     "key1": "value1",
#     "key2": 42
# }

# headers = {
#     "Content-Type": "application/json"
# }
# x = requests.post(baseURL+dataURL, data=json.dumps(data), headers=headers) #json = myobj)
def post(url, data):
    headers = {
        "Content-Type": "application/json"
    }
    x = requests.post(url, data=json.dumps(data), headers=headers) #json = myobj)



# # corner detection
# def corner_detection(img):
#     N=10
#     corners = cv2.goodFeaturesToTrack(img, N, 0.6, 10)
#     # returns N best corners
#     if corners is not None:
#         corners = np.int0(corners)

#         # draw the corners
#         for corner in corners:
#             x, y = corner.ravel()
#             cv2.circle(img, (x, y), 5, (255, 0,0), -1)
#     return img