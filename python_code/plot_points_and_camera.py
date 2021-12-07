from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import bz2

def get_n_cam_points(gx, gy, gz, points_3d, camera_indices, point_indices):
    print(f"\n\npoint_indices: {point_indices}")
    print(f"point_indices shape: {point_indices.shape}")
    print(f"camera_indices: {camera_indices}")
    print(f"camera_indices shape: {camera_indices.shape}")

    camera_choice = [48]

    obs_indices = []
    for i, ci in enumerate(camera_indices):
        if ci in camera_choice:  # meaning it is a camera we want to look at
            obs_indices.append(i)  # record which observations are associated with the camera we are looking at

    points_with_cams = []
    for oi in obs_indices:
        points_with_cams.append(point_indices[oi]) # grabs point associated with the observation

    print(f"points with cams:\n{points_with_cams}")
    print(f"length of points with cams: {len(points_with_cams)}")

    tx, ty, tz = [], [], []
    for point in points_with_cams:
        tx.append(gx[point])
        ty.append(gy[point])
        tz.append(gz[point])
    
    gx, gy, gz = tx, ty, tz

    return gx, gy, gz

def read_bal_data(file_name):
    # with bz2.open(file_name, "rt") as file:
    with open(file_name, "rt") as file:
        n_cameras, n_points, n_observations = map(
            int, file.readline().split())

        camera_indices = np.empty(n_observations, dtype=int)
        point_indices = np.empty(n_observations, dtype=int)
        points_2d = np.empty((n_observations, 2))

        for i in range(n_observations):
            camera_index, point_index, x, y = file.readline().split()
            camera_indices[i] = int(camera_index)
            point_indices[i] = int(point_index)
            points_2d[i] = [float(x), float(y)]

        camera_params = np.empty(n_cameras * 9)
        for i in range(n_cameras * 9):
            camera_params[i] = float(file.readline())
        camera_params = camera_params.reshape((n_cameras, -1))

        points_3d = np.empty(n_points * 3)
        for i in range(n_points * 3):
            points_3d[i] = float(file.readline())
        points_3d = points_3d.reshape((n_points, -1))

    return camera_params, points_3d, camera_indices, point_indices, points_2d

def read_data(file_name):
    xyz_points, xyz_cameras = [], []  # will create list of lists
    with open(file_name, 'r') as f:
        n_points, n_cameras = map(int, f.readline().split())

        for _ in range(n_points):
            xyz_points.append(f.readline().split())
        for _ in range(n_cameras):
            xyz_cameras.append(f.readline().split())

    factor = 1#=.0001
    px = [factor * float(p_lst[0]) for p_lst in xyz_points]
    py = [factor * float(p_lst[1]) for p_lst in xyz_points]
    pz = [factor * float(p_lst[2]) for p_lst in xyz_points]

    cx = [factor * float(c_lst[0]) for c_lst in xyz_cameras]
    cy = [factor * float(c_lst[1]) for c_lst in xyz_cameras]
    cz = [factor * float(c_lst[2]) for c_lst in xyz_cameras]
    return px, py, pz, cx, cy, cz

# file_name = "./data_fake.txt"
# file_name = "./solved-49-7776.txt"
file_name = "./problem-49-7776-pre.txt-solved.txt"

px, py, pz, cx, cy, cz = read_data(file_name)
print(f"px:\n{max(px)}")
print(f"py:\n{max(py)}")
print(f"px:\n{max(pz)}")

print(f"cx:\n{max(cx)}")
print(f"cy:\n{max(cy)}")
print(f"cx:\n{max(cz)}")

print(f"px:\n{min(px)}")
print(f"py:\n{min(py)}")
print(f"px:\n{min(pz)}")

print(f"cx:\n{min(cx)}")
print(f"cy:\n{min(cy)}")
print(f"cx:\n{min(cz)}")

# LADY
# lady_path = './data_ladybug'
lady_path = './data_ladybug.txt'
camera_params, points_3d, camera_indices, point_indices, points_2d = read_bal_data(lady_path)

print(f"camera_params shape: {camera_params.shape}")
num_cams = 1
reduced_cam = camera_params[0:num_cams,:]
rot_start, rot_end = 0, 3
tra_start, tra_end = 3, 6
rot_vec = reduced_cam[:, rot_start:rot_end]
tra_vec = reduced_cam[:, tra_start:tra_end]
print(f"reduced_cam shape: {reduced_cam.shape}")
print(f"rot_vec shape: {rot_vec.shape}")
print(f"tra_vec shape: {tra_vec.shape}")
tx, ty, tz = tra_vec[:,0].tolist(), tra_vec[:,1].tolist(), tra_vec[:,2].tolist()

print(f"points_3d shape: {points_3d.shape}")
gx, gy, gz = points_3d[:,0].tolist(), points_3d[:,1].tolist(), points_3d[:,2].tolist()

# get grab first n camera corresponding 3D points
gx, gy, gz = get_n_cam_points(gx, gy, gz, points_3d, camera_indices, point_indices)


print(f"gx:\n{max(gx)}")
print(f"gy:\n{max(gy)}")
print(f"gx:\n{max(gz)}")

print(f"gx:\n{min(gx)}")
print(f"gy:\n{min(gy)}")
print(f"gx:\n{min(gz)}")
# LADY

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

cr = (0, 1)  # camera range

#ax.scatter(px, py, pz, c='r', marker='o')
#ax.scatter(cx, cy, cz, c='black', marker='s')
ax.scatter(cx[cr[0]:cr[1]], cy[cr[0]:cr[1]], cz[cr[0]:cr[1]], c='black', marker='s')

# ground truth Data
ax.scatter(gx, gy, gz, c='blue', marker='o')
ax.scatter(tx, ty, tz, c='green', marker='s')

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

lmin = -2
lmax = 2
ax.set_xlim(lmin, lmax)
ax.set_ylim(lmin, lmax)
ax.set_zlim(lmin, lmax)

plt.show()