import time
from numba import jit
import cv2 as cv
import numpy as np
import math

'''
reproject omnidirectional frame to the specified viewport
:param pitch: up -> down 90.0 -> -90.0
:param yaw: left -> right -180 -> 180
:param roll: clockwise 0 -> 180 anticlockwise 0 -> -180
:param file: path of the omnidirectional frame
:return: cv image of the reprojected omnidirectional frame
'''


@jit
def spin_angle(pitch, yaw, roll, file):
    # start = time.time()

    img = cv.imread(file)
    height = img.shape[0]
    width = img.shape[1]
    pi = 3.14159265358979323846
    pi_2 = 1.57079632679489661923

    roll = roll * pi / 180.0  # 翻滚
    pitch = pitch * pi / 180.0  # 俯仰
    yaw = yaw * pi / 180.0  # 偏航
    Img = np.zeros((height, width, 3), np.uint8)

    for i in np.arange(0, height):
        for j in np.arange(0, width):
            x_pos = (j + 1) / width * (pi * 2) - pi
            y_pos = (i + 1) / height * pi - pi_2

            x = math.cos(y_pos) * math.cos(x_pos)
            y = math.cos(y_pos) * math.sin(x_pos)
            z = math.sin(y_pos)

            Nx = x * math.cos(pitch) * math.cos(yaw) + y * (
                    -math.cos(roll) * math.sin(yaw) + math.sin(roll) * math.sin(pitch) * math.cos(yaw)) + z * (
                         math.sin(roll) * math.sin(yaw) + math.cos(roll) * math.sin(pitch) * math.cos(yaw))

            Ny = x * math.cos(pitch) * math.sin(yaw) + y * (
                    math.cos(roll) * math.cos(yaw) + math.sin(roll) * math.sin(pitch) * math.sin(yaw)) + z * (
                         -math.sin(roll) * math.cos(yaw) + math.cos(roll) * math.sin(pitch) * math.sin(yaw))
            Nz = x * (-math.sin(pitch)) + y * (math.sin(roll) * math.cos(pitch)) + z * math.cos(roll) * math.cos(pitch)

            temp0 = Nx
            temp1 = math.sqrt(Nx * Nx + Ny * Ny)

            if abs(temp0) < 0.0000001:
                temp0 = 0.0000001

            if abs(temp1) < 0.0000001:
                temp1 = 0.0000001

            f_i = math.atan(Ny / temp0)
            f_j = math.atan(Nz / temp1)

            if Nx < 0 and Ny < 0:
                f_i = f_i - pi

            if Nx < 0 and Ny > 0:
                f_i = f_i + pi

            f_ii = (f_i + pi) / (2 * pi) * width
            f_jj = (f_j + pi_2) / pi * height

            jj = round(f_ii)
            ii = round(f_jj)

            if ii < 0:
                ii = 0

            if jj < 0:
                jj = 0

            if ii > height - 1:
                ii = height - 1

            if jj > width - 1:
                jj = width - 1

            r = img[ii, jj, 0]
            g = img[ii, jj, 1]
            b = img[ii, jj, 2]

            Img[i, j, 0] = r
            Img[i, j, 1] = g
            Img[i, j, 2] = b

    return Img


'''
spin_angle(0, 0, 30, filename)
'''
