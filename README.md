# EVAS-OV
This is a demo for Edge-assisted Viewport Adaptive Scheme for real-time Omnidirectional Video. The whole project contains two parts:
+ database
  + 5 omnidirectional videos for test.
  + 59 users' viewpoint data of 5 test videos extracted at 30 times per second.
+ code:
  + A programme to transform an omnidirectional video into a VBM video according to each frame's viewport.
  + A programme to cluster users into groups using DBSCAN algorithm based on their viewport.

## Omnidirectional video
The 5 omnidirectional videos is stored in `database/videos`. Each video is a `3840 * 2080` 4K omnidirectional video with a duration of 20 second. They are from a open source dataset ["360-degree video head movement dataset"](https://dl.acm.org/doi/abs/10.1145/3083187.3083215) by Kan N, Liu C, Zou J, et al.
## Viewpoint data
The corresponding viewpoint dataset of 59 users extracted at 30 times per second. The original data is stored in `database\results` in which viewpoint is stored as *`(q0, q1, q2, q3)`* 4-dimension data. The data in `database\viewpoint_xyz` contains the viewpoint data stored as *`(x, y, z)`* coordinates of euclidean space. The data in `database\viewpoint_new` contains the viewpoint data stored as *`(roll, pitch, yaw)`* coordinates of Euler pt.

## VBM generator
The VBM generator is written using python 
### Preperation
Before run this programme, you need to download [FFmpeg](http://ffmpeg.org/download.html) on your computer and put the \<bin> folder into your computer's search path so that the `ffmpeg.exe` can be used in command line mode.
### Usage
