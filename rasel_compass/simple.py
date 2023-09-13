# Simply.py - a simple MPU9250 demo app
# Kevin McAleer
# May 2021

from machine import I2C, Pin
from math import sqrt, atan2, pi, copysign, sin, cos
from mpu9250 import MPU9250
from time import sleep

# addresses 
MPU = 0x68
id = 0
sda = Pin(0)
scl = Pin(1)

# create the I2C
i2c = I2C(id=id, scl=scl, sda=sda)

# Scan the bus
print(i2c.scan())
m = MPU9250(i2c)

# Calibration and bias offset
m.ak8963.calibrate(count=100)  # offset r scale

# For low pass filtering
filtered_x_value = 0.0    #$##############################################
filtered_y_value = 0.0

# declination = 40


def get_reading()->float:
    ''' Returns the readings from the sensor '''
    global filtered_y_value, filtered_x_value
    x = m.acceleration[0] 
    y = m.acceleration[1]
    z = m.acceleration[2] 

    # Get soft_iron adjusted values from the magnetometer
    mag_x, mag_y, magz = m.magnetic

    filtered_x_value = low_pass_filter(mag_x, filtered_x_value)
    filtered_y_value = low_pass_filter(mag_y, filtered_y_value)

    az =  90 - atan2(filtered_y_value, filtered_x_value) * 180 / pi

    # make sure the angle is always positive, and between 0 and 360 degrees
    if az < 0:
        az += 360
        
    return x, y, z,  az

def low_pass_filter(raw_value:float, remembered_value):  #################################################
    ''' Only applied 20% of the raw value to the filtered value '''
    
    # global filtered_value
    alpha = 0.8
    filtered = 0
    filtered = (alpha * remembered_value) + (1.0 - alpha) * raw_value
    return filtered

def show():
    ''' Shows the Pitch, Rool and heading '''
    x, y, z, az= get_reading()
    print("compass", az)
    sleep(0.2)

# reset orientation to zero
x,y,z,az= get_reading()

# main loop
while True:
    show()
