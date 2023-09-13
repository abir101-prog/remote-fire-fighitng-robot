import numpy as np
import cv2
import time
st=time.time()
# load image
img = cv2.imread('./images/up_corner_wall.png')

img = cv2.resize(img, (0,0), fx=.99, fy=.99)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# corner detection
N=10
corners = cv2.goodFeaturesToTrack(gray, N, 0.6, 10)
# returns N best corners
corners = np.int0(corners)

# draw the corners
for corner in corners:
    x, y = corner.ravel()
    cv2.circle(img, (x, y), 5, (255, 0,0), -1)

cv2.imshow('Frame', img)
et = time.time()
cv2.waitKey(0)
cv2.destroyAllWindows()
print(et-st)


