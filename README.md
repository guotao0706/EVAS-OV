# EVAS-OV
This is a demo for Edge-assisted Viewport Adaptive Scheme for real-time Omnidirectional Video. The whole project contains two parts:
+ database
  + 5 omnidirectional videos for testing.
  + 59 users' viewpoint data of 5 test videos extracted at 30 times per second.
+ code:
  + A programme to transform an omnidirectional video into a VBM video according to each frame's viewport.
  + A programme to cluster users into groups using DBSCAN algorithm based on their viewport.

## Omnidirectional video
The 5 omnidirectional videos are stored in `database/videos`. Each video is a `3840 * 2048` 4K omnidirectional video with a duration of 20 seconds. They are from a open-source dataset ["360-degree video head movement dataset"](https://dl.acm.org/doi/abs/10.1145/3083187.3083215) by Kan N, Liu C, Zou J, et al.
## Viewpoint data
The corresponding viewpoint dataset of 59 users extracted at 30 times per second. The original data is stored in `database\results` in which viewpoint is stored as *`(q0, q1, q2, q3)`* 4-dimension data. The data in `database\viewpoint_xyz` contains the viewpoint data stored as *`(x, y, z)`* coordinates of euclidean space. The data in `database\viewpoint_new` contains the viewpoint data stored as *`(roll, pitch, yaw)`* coordinates of Euler pt.

## VBM generator
The VBM generator is written using python. It reads omnidirectional video as BMP frames using FFmpeg and reprojects each frame to its viewpoint. Then these frames are cut and stitched as VBM frames and combine these frames in order into a VBM video.
### Preparation
Before running this programme, you need to download [FFmpeg](http://ffmpeg.org/download.html) on your computer and put the \<bin> folder into your computer's search path so that the `ffmpeg.exe` can be used in command line mode. Moreover, you should make sure that `'.'`(It means allow searching python script in the same folder) is in your python search path.
### Usage
+ Open `code/run.py`.
+ Edit `video_path`, which is the path of the omnidirectional video.
+ Edit `frame_out_path`, which is the temporary folder for frames and sound file and will be removed after running this programme.
+ Edit `videop_path`, which is the path for the output VBM video.
+ Edit `viewport_path`, which is the path for the viewpoint file, noting that it should be one of the files in `database/viewpoints_new` because the `reprojection.py` needs *`(roll, pitch, yaw)`* as one of the parameters.
+ Run the `run.py` using python.

## Clustering algorithm
The clustering algorithm is written using C++. It reads users' viewpoint data at the same moment and clusters them into groups based on the weighted sum of their viewpoints' distance and the difference of their velocity. Then the clustering outcome of the first moment's users' viewpoints will be saved as TXT files for demo and the MSE of all moments' clustering result will be calculated and saved as a TXT file. This clustering algorithm is designed mainly based on DBSCAN algorithm.
### Usage
+ Open `code/clustering/run.cpp`.
+ Edit `viewdata_folder`, which is the path for the viewpoint file, noting that it should be `database/viewpoints_xyz`.
+ Edit `viewdata_file`, which is the name of the viewpoint file.
+ Edit `viewdata_num`, which is the number of viewpoints you want to read for one user.
+ Edit `w`, which is the weight for distance and velocity in calculation.
+ Edit `minPts`, which is the minimum points in neighbourhood in DBSCAN algorithm.
+ Edit `eps`, which is the radius of neighbourhood in DBSCAN algorithm.
+ Edit `cluster_output`, which is the TXT file of clustering results of users' viewpoints at the first moment for demo.
+ Edit `MSE_file`, which is the path of the TXT file that contains the MSE of all moments' clustering result.
+ Compile `run.cpp` and run the generated EXE file.
