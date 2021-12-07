from random import randrange

num_points = 100
maxx = 10
minn = 0
px = [randrange(minn, maxx) for _ in range(num_points)]
py = [randrange(minn, maxx) for _ in range(num_points)]
pz = [randrange(minn, maxx) for _ in range(num_points)]

num_cameras = 10
cx = [randrange(minn, maxx) for _ in range(num_cameras)]
cy = [randrange(minn, maxx) for _ in range(num_cameras)]
cz = [randrange(minn, maxx) for _ in range(num_cameras)]

file_name = "./data_fake.txt"
with open(file_name, 'w') as f:
    #f.write('<first line is: "num_points num_cameras">\n')
    f.write('%s %s\n' % (num_points, num_cameras))
    for x, y, z in zip(px, py, pz):
        f.write('%s %s %s\n' % (x, y, z))
    for x, y, z in zip(cx, cy, cz):
        f.write('%s %s %s\n' % (x, y, z))