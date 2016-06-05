import sys, getopt

sys.path.append('.')
import RTIMU
import os.path
import time
import math
import csv

# Note: timestamp is in microseconds

timestamp = 0

SETTINGS_FILE = "RTIMULib"

print("Using settings file " + SETTINGS_FILE + ".ini")
if not os.path.exists(SETTINGS_FILE + ".ini"):
  print("Settings file does not exist, will be created")

s = RTIMU.Settings(SETTINGS_FILE)
imu = RTIMU.RTIMU(s)
imu.IMUInit()

print("IMU Name: " + imu.IMUName())

with open('example_data.csv', 'rb') as f:
    reader = csv.reader(f)
    imu_data = list(reader)

# this is how to turn off the magnetometer

imu.setCompassEnable(False)

# everything is now ready

with open('fuse_data.csv', 'wb') as f:
    writer = csv.writer(f)

    label_row = []
    label_row.append('Time');
    label_row.append('roll');
    label_row.append('pitch');
    label_row.append('yaw');

    writer.writerow(label_row);

    for row in imu_data:

        #print(row);
        # time stamp is in milliseconds.
        # convert to microseconds.
        timestamp = int(row[0]) * 1000 
        ax = float(row[1])
        ay = float(row[2])
        az = float(row[3])
        mx = float(row[4])
        my = float(row[5])
        mz = float(row[6])
        gx = float(row[7])
        gy = float(row[8])
        gz = float(row[9])

        imu.setExtIMUData(gx, gy, gz, ax, ay, az, mx, my, mz, timestamp)
        data = imu.getIMUData()
        fusionPose = data["fusionPose"]

        fuse_row = []

        fuse_row.append(timestamp);
        fuse_row.append(math.degrees(fusionPose[0]));
        fuse_row.append(math.degrees(fusionPose[1]));
        fuse_row.append(math.degrees(fusionPose[2]));

        writer.writerow(fuse_row);
 
        #print("r: %f p: %f y: %f" % (math.degrees(fusionPose[0]), 
        #    math.degrees(fusionPose[1]), math.degrees(fusionPose[2])))
        #time.sleep(0.1)


