import cv2

# Load the image captured by the ESP32-CAM
image = cv2.imread("door.jpg")

# Convert the image to grayscale (for simplicity)
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Apply thresholding to distinguish between open and closed
_, thresholded = cv2.threshold(gray_image, 128, 255, cv2.THRESH_BINARY)
cv2.imshow('threshodlo', thresholded)
# Measure the percentage of white pixels (door open) in the thresholded image
total_pixels = thresholded.size
white_pixels = cv2.countNonZero(thresholded)
percentage_white = (white_pixels / total_pixels) * 100

# Define a threshold for door open/closed determination
threshold_percentage = 10  # Adjust this threshold as needed

if percentage_white < threshold_percentage:
    print("Door is closed")
else:
    print("Door is open")

# Display the thresholded image (for debugging)
cv2.imshow("Thresholded Image", thresholded)
cv2.waitKey(0)
cv2.destroyAllWindows()

# import required library
# import cv2

# # read input image
# img = cv2.imread('door.jpg')

# # rotate the image both vertically and horizontally
# img_vh = cv2.flip(img, -1)

# # display the rotated image
# cv2.imshow("Both vertical and horizontal flip", img_vh)
# cv2.imshow('', img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()