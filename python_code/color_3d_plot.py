from os import read
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import animation
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation
animation.writer = animation.writers['ffmpeg']

def read_data(file_name):
    xyz_points, xyz_cameras = [], []  # will create list of lists
    with open(file_name, 'r') as f:
        n_points, n_cameras = map(int, f.readline().split())

        for _ in range(n_points):
            xyz_points.append(f.readline().split())
        for _ in range(n_cameras):
            xyz_cameras.append(f.readline().split())

    factor = 1#.01
    px = [factor * float(p_lst[0]) for p_lst in xyz_points]
    py = [factor * float(p_lst[1]) for p_lst in xyz_points]
    pz = [factor * float(p_lst[2]) for p_lst in xyz_points]

    cx = [factor * float(c_lst[0]) for c_lst in xyz_cameras]
    cy = [factor * float(c_lst[1]) for c_lst in xyz_cameras]
    cz = [factor * float(c_lst[2]) for c_lst in xyz_cameras]
    return px, py, pz, cx, cy, cz

def plot_scattered_3d(px, py, pz, cx, cy, cz):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    ax.scatter(px, py, pz, c='r', marker='o', s=4)
    ax.scatter(cx, cy, cz, c='black', marker='s')

    ax.set_xlabel('X Label')
    ax.set_ylabel('Y Label')
    ax.set_zlabel('Z Label')

    lmin = -1
    lmax = 5
    ax.set_xlim(lmin, 3)
    ax.set_ylim(lmin, 3)
    ax.set_zlim(0, lmax)

    plt.show()

def get_dist(p1, p2):
    sqrd_dist = np.sum((p1 - p2)**2, axis=0)
    return np.sqrt(sqrd_dist)

# 10 - 100
file_name_100 = "./100_out_feature_points.txt-solved.txt"
file_name_90 = "./90_out_feature_points.txt-solved.txt"
file_name_80 = "./80_out_feature_points.txt-solved.txt"
file_name_70 = "./70_out_feature_points.txt-solved.txt"
file_name_60 = "./60_out_feature_points.txt-solved.txt"
file_name_50 = "./50_out_feature_points.txt-solved.txt"
file_name_40 = "./40_out_feature_points.txt-solved.txt"
file_name_30 = "./30_out_feature_points.txt-solved.txt"
file_name_20 = "./20_out_feature_points.txt-solved.txt"
file_name_10 = "./10_out_feature_points.txt-solved.txt"

gt = "/home/tannort5/Documents/prob_rob/slamdunk/datasets/rgbd_dataset_freiburg1_xyz/groundtruth.txt"  # ground truth path

all_files = [file_name_10, file_name_20, file_name_30, file_name_40, file_name_50, file_name_60, file_name_70, file_name_80, file_name_90, file_name_100]

_, _, _, cx, cy, cz = read_data(file_name_100)

c_point = [np.array([x, y, z]) for x,y,z in zip(cx, cy, cz)]

gx, gy, gz = [], [], []
g_point = []
with open(gt, 'r') as file:
    for line in file:
        line_lst = line.split()
        gx.append(float(line_lst[1]))
        gy.append(float(line_lst[2]))
        gz.append(float(line_lst[3]))
        g_point.append( np.array([float(line_lst[1]), float(line_lst[2]), float(line_lst[3])]) )

closest_points = []
for i, cp in enumerate(c_point):
    min_dist = np.inf
    max_dist = -np.inf
    min_point = None
    
    for gp in g_point:
        dist = get_dist(cp, gp)
        if dist < min_dist:
            min_dist = dist
            min_point = gp
        if dist > max_dist:
            max_dist = dist

    print(f"i: {i}\tMin dist: {min_dist}")
    print(f"i: {i}\tMax dist: {max_dist}")
    closest_points.append(min_point)








# file_name = "./data_fake.txt"
# file_name = "./solved-49-7776.txt"
# file_name = "./problem-49-7776-pre.txt-solved.txt"
# file_name = "./out_feature_points.txt-solved.txt"
# file_name = "./five_images_out_feature_points.txt-solved.txt"
# file_name = "./03_out_feature_points.txt-solved.txt"
# file_name = "./02_five_images_out_feature_points.txt-solved.txt"

# print(f"px:\n{max(px)}")
# print(f"py:\n{max(py)}")
# print(f"px:\n{max(pz)}")

# print(f"cx:\n{max(cx)}")
# print(f"cy:\n{max(cy)}")
# print(f"cx:\n{max(cz)}")

# print(f"px:\n{min(px)}")
# print(f"py:\n{min(py)}")
# print(f"px:\n{min(pz)}")

# print(f"cx:\n{min(cx)}")
# print(f"cy:\n{min(cy)}")
# print(f"cx:\n{min(cz)}")