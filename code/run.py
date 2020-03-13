from extract_videos_frame import extract_frame
from reprojection import spin_angle
from VBM_frame import down_cut_stitch
from xyz_pitch_yaw import xyz_convert
import fnmatch
import os
import math
import cv2 as cv

# lists to save coordinates of user's viewport
pitch = []
yaw = []
roll = []

video_path = '../database/videos/2OzlksZBTiA.mp4'
frame_out_path = '../database/videos/frames'
videop_path = '../database/videos/output1.mp4'
viewport_path = '../database/viewpoints_new/uid-17954da2-a83b-46ac-af8b-f32943d2e36a/Diving-2OzlksZBTiA_0.txt'
fps = 30

# make temporary for frames and sound file
if os.path.isdir(frame_out_path):
    for root, dirnames, filenames in os.walk(frame_out_path):
        for name in filenames:
            os.remove(os.path.join(root, name))
    os.rmdir(frame_out_path)
os.mkdir(frame_out_path)

# extract frames and sound file of the omnidirectional video
extract_frame(video_path, frame_out_path, fps)

# read the viewport file
view_file = open(viewport_path)
try:
    for line in view_file:
        line = line.rstrip('\n')
        line = line.split(' ')
        roll.append(float(line[2])/math.pi * 180)    # roll
        pitch.append(float(line[3])/math.pi * 180)    # pitch
        yaw.append(float(line[4])/math.pi * 180)    # yaw
finally:
    view_file.close()

for root, dirnames, filenames in os.walk(frame_out_path):
    for filename in filenames:
        if filename.split('.')[-1] != 'bmp':
            continue
        frame_path = os.path.join(frame_out_path, filename)
        i = int(filename[-8:-5]) - 1    # extract the index of the frame

        # re-project the omnidirectional frame to user's corresponding viewport
        frame = spin_angle(pitch[i], yaw[i], roll[i], frame_path)
        print(str(pitch[i]) + ', ' + str(yaw[i]) + ', ' + str(roll[i]))
        # cv.imwrite(frame_path, frame)

        # produce VBM frame
        down_cut_stitch(frame, frame_path)

# use the processed VBM frames and sound file to generate new video
bash_cmd = 'ffmpeg -y -r ' + str(fps) + ' -i ' + os.path.join(frame_out_path, 'image-%04d.bmp') + ' -i ' + os.path.join(frame_out_path, 'sound.mp3') + ' -qscale 10 ' + videop_path   #save sound file
os.system(bash_cmd)

# remove the temporary folder
if os.path.isdir(frame_out_path):
    for root, dirnames, filenames in os.walk(frame_out_path):
        for name in filenames:
            os.remove(os.path.join(root, name))
    os.rmdir(frame_out_path)

