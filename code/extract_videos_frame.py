import fnmatch
import os

'''
extract video to frames and sound file
:param video: path of the video
:param dir: save path for frames and sound file
:param fps: frame per second
:return: none
'''


def extract_frame(video, dir, fps):
    out = os.path.join(dir, 'image-%4d.bmp')
    # print(out)
    # ffmpeg -i video.mp4 image-%4d.jpg -r 30
    bash_cmd = 'ffmpeg -i ' + video + ' ' + out + ' -r ' + str(fps)  # save frames
    os.system(bash_cmd)  # 系统执行命令
    bash_cmd = 'ffmpeg -i ' + video + ' -vn -y -f mp2 ' + os.path.join(dir, 'sound.mp3')  # save sound file
    os.system(bash_cmd)


'''
fps = 30    #frame rate
if not os.path.isdir(os.path.join(video_folder, 'extract_frame_result_30')):
    os.mkdir(os.path.join(video_folder, 'extract_frame_result_30'))
for filenames in os.walk(video_folder):
    for filename in fnmatch.filter(filenames[2], '*.mp4'):
        video_name = filename.split('.')[0]
        dir = os.path.join(video_folder, 'extract_frame_result_30/' + video_name)
        if not os.path.isdir(dir):
            os.mkdir(dir)   #创建保存帧的文件夹
        extract_frame(os.path.join(video_folder, filename), dir, fps)
    print('finish')
'''
