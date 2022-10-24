from lebai import zeroconf
## Try to find the l_master
d = zeroconf.Discovery()
controllers = d.resolve()
## Robot instance
from lebai import l_master
import time
robot = l_master.Robot(controllers[0].ip_address)
time.sleep(2)
robot.start_sys()
robot.movej({"j1": 1.0,"j2": -1.0,"j3": 1.,"j4": -0.0,"j5": -1.0,"j6": 0.0},1.0,1.0,0.0,0.0)
#robot.movej([1,2,3,4,5,6],1.0,1.0,0.0,0.0)
# j1=-1.4964941323816858,j2=-2.9646096201874874,j3=1.1310232096679318,j4=0.701029220063738,j5=1.8745245227962535,j6=-1.5241057865636274
jpose0={'j1':-0.2, 'j2':2.7, 'j3':-4.0, 'j4':-2.3, 'j5':1.6, 'j6':-1.3}
robot.movej(jpose0,1.0,1.0,0.0,0.0)
fk_resp = robot.kinematics_forward(jpose0)
print(fk_resp)
ik_resp  = robot.kinematics_inverse(fk_resp.pose)
print(ik_resp)