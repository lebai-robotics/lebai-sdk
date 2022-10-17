from lebai import l_master
from lebai import zeroconf
import time
## Try to find the l_master
d = zeroconf.Discovery()
ip_list = d.resolve()
## Robot instance
robot = l_master.Robot(ip_list[0], True)
time.sleep(2)
robot.start_sys()
robot.movej({"j1": 1.0,"j2": -1.0,"j3": 1.,"j4": -0.0,"j5": -1.0,"j6": 0.0},1.0,1.0,0.0,0.0)
robot.movej([1,2,3,4,5,6],1.0,1.0,0.0,0.0)