

#  Raspberry Pi Master for Arduino Slave
#  i2c_master_pi.py
#  Connects to Arduino via I2C
  
#  DroneBot Workshop 2019
#  https://dronebotworkshop.com
import time, random, subprocess


#from smbus import SMBus

#addr = 0x8 # bus address
#bus = SMBus(1) # indicates /dev/ic2-1

numb = 1

print ("Enter 1 for ON or 0 for OFF")
ledstate = input(">>>>   ")

def camera_process():
    #turn on camera
    #for now, output random integer from 1-4 (1=left, 2=right, 3=both, 4=no threat)
    #subprocess.Popen(['bash', 'model_run.sh'])
    with open("/home/admin/Projects/Python/tflite/modified_obj_detection/examples/lite/examples/object_detection/raspberry_pi/detect.py") as f:
        exec(f.read())
    
    print(test_print)
    return random.randint(1,4)

camera_process()


    


