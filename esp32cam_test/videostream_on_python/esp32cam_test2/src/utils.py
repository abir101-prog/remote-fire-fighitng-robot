import cv2
import numpy as np    

# corner detection
def corner_detection(img):
    N=10
    corners = cv2.goodFeaturesToTrack(img, N, 0.6, 10)
    # returns N best corners
    if corners is not None:
        corners = np.int0(corners)

        # draw the corners
        for corner in corners:
            x, y = corner.ravel()
            cv2.circle(img, (x, y), 5, (255, 0,0), -1)
    return img