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



# 10 - 100
all_images_file = "./all_out_feature_points.txt-solved.txt"
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

# px, py, pz, cx, cy, cz = read_data(all_images_file)
# plot_scattered_3d(px, py, pz, cx, cy, cz)
# import sys
# sys.exit(0)

all_files = [file_name_10, file_name_20, file_name_30, file_name_40, file_name_50, file_name_60, file_name_70, file_name_80, file_name_90, file_name_100]
titles = ["10 Images", "20 Images", "30 Images", "40 Images", "50 Images", "60 Images", "70 Images", "80 Images", "90 Images", "100 Images"]

fig = plt.figure(figsize=(12,5))
for i, file in enumerate(all_files):
    ax = fig.add_subplot(2, 5, i+1, projection='3d')
    point_args = {
        's': 4,
        'c': 'r',
        'marker': 'o',
    }
    camera_args = {
        'c': 'black',
        'marker': 's',
    }

    px, py, pz, cx, cy, cz = read_data(file)
    # ax.scatter(px, py, pz, **point_args)
    ax.scatter(cx, cy, cz, **camera_args)
    ax.set_yticklabels([])
    ax.set_xticklabels([])
    ax.set_zticklabels([])
    ax.set_title(titles[i])
    ax.view_init(elev=50., azim=-20.)

plt.show()

# fig, axs = plt.subplots(5,2,figsize=(8,4))#, projection='3d')
# axs = np.ravel(axs)
# for i, ax in enumerate(axs):
#     lmin = -1
#     lmax = 5
#     ax.set_xlim(lmin, 3)
#     ax.set_ylim(lmin, 3)
#     # ax.set_zlim(0, lmax)
#     point_args = {
#         's': 4,
#         'c': 'r',
#         'marker': 'o',
#     }
#     camera_args = {
#         'c': 'black',
#         'marker': 's',
#     }
#     px, py, pz, cx, cy, cz = read_data(all_files[i])

#     # Create handles
#     ax.scatter(px, py, pz, **point_args)
#     ax.scatter(cx, cy, cz, **camera_args)

# plt.show()
