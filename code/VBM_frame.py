import fnmatch
import os
import time
import cv2 as cv
import numpy as np
import math

'''
to downsample and cut the FOV, margin and base, then generate VBM frame
:param frame: omnidirectional frame, saved as 
:param out_path: save path for VBM frame
:return: none
'''


def down_cut_stitch(frame, out_path):
    # start = time.time()

    img = frame  # omnidirectional frame
    img_down1_2 = cv.pyrDown(img)  # 1/2 downsampling
    img_down1_4 = cv.pyrDown(img_down1_2)  # 1/2 downsampling，base layer

    height = img.shape[0]  # rows
    width = img.shape[1]  # cols
    img_margin = img[int(height / 4): int(height / 4 * 3), int(width / 4): int(width / 4 * 3), :]  # margin layer
    img_margin_down = cv.resize(img_margin, (int(width / 4), int(height / 4)), interpolation=cv.INTER_CUBIC)

    height_new = int(img.shape[0] / 2)  # rows for VBM
    width_new = int(img.shape[1] / 3 + img.shape[1] / 4)  # cols for VBM
    # print(height, width)
    img_stitch = np.zeros((height_new, width_new, 3))  # VBM frame
    img_cut = img[int(height / 4): int(height / 4) + height_new, int(width / 3): int(width / 3) * 2, :]  # FOV area

    # combine FOV，margin，base as VBM frame
    img_stitch[:, :int(width / 3), :] = img_cut  # FOV
    img_stitch[:int(height / 4), int(width / 3):, :] = img_margin_down  # margin
    img_stitch[int(height / 4):int(height / 2), int(width / 3):, :] = img_down1_4  # base
    cv.imwrite(out_path, img_stitch)  # save path


'''
frame_folder = '../database/videos/extract_frame_result_30' #原始帧文件夹
out_path = '../database/videos/reshape_VBM'  #VBM帧文件夹

if not os.path.isdir(out_path):
    os.mkdir(out_path)

for root, dirnames, filenames in os.walk(frame_folder):
    for dir_name in dirnames:
        path = os.path.join(out_path, dir_name)
        if not os.path.isdir(path):
            os.mkdir(path)
        frame_path = os.path.join(root, dir_name)
        down_cut_stitch(frame_path, path)


print('finish')
'''
