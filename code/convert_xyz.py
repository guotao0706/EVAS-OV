import os
import fnmatch
import math

'''
transfer (q0, q1, q2, q3) to (x, y, z)
'''

old_dataset = 'viewpoints_old\\'
new_dataset = 'viewpoint_xyz\\'

def str_to_float(str_list):
    float_list = []
    for string in str_list:
        float_list.append(float(string))
    return float_list


'''
:param pitch: up -> down 90.0 -> -90.0
:param yaw: left -> right -180 -> 180
:param roll: clockwise 0 -> 180 anticlockwise 0 -> -180
:param file: path of the omnidirectional frame
:return: cv image of the reprojected omnidirectional frame
'''
def unit_to_rotation(unit):
    q0 = unit[0]
    q1 = unit[1]
    q2 = unit[2]
    q3 = unit[3]

    x = 1 - 2 * q2 * q2 - 2 * q3 *q3
    y = 2 * q1 * q2 + 2 * q0 * q3
    z = 2 * q1 * q3 + 2 * q0 * q2
    return x, y, z


if __name__ == '__main__':
    count = 0
    for root, dirnames, filenames in os.walk(old_dataset):
        print('root ' + root)

        for filename in fnmatch.filter(filenames, '*.txt'):
            if filename == 'formAnswers.txt' or filename == 'testInfo.txt':
                continue
            file_path = os.path.join(root, filename)
            # print(file_path)
            uid = root.split('\\')[1]
            print('uid'+uid)

            cooked_dir = os.path.join(new_dataset, uid)
            cooked_file = os.path.join(cooked_dir, filename)
            if not os.path.isdir(cooked_dir):
                os.mkdir(cooked_dir)
            with open(file_path, 'r') as fr, open(cooked_file, 'w') as fw:
                count = -1
                for line in fr:
                    # count += 1
                    if len(line) > 10:
                        parse = line.split()
                        time = str(parse[0])
                        frame = int(parse[1])
                        if frame > count:
                            x, y, z = unit_to_rotation(str_to_float(parse[2:6]))
                            fw.write(time + ' ' + str(frame) + ' ' + str(x) + ' ' + str(y) + ' ' + str(z) + '\n')
                            count = frame

    print(count)