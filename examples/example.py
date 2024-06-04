# Copyright 2022 lebai.ltd
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from pylebai import zeroconf
## Try to find the l_master
d = zeroconf.Discovery()
controllers = d.resolve()
## Robot instance
from pylebai import l_master
import time
robot = l_master.Robot(controllers[0].ip_address)
time.sleep(2)
robot.start_sys()
robot.movej([1.0, 0.0, 0.0, 0.0, -1.0, 0.0],1.0,1.0,0.0,0.0)
#robot.movej([1,2,3,4,5,6],1.0,1.0,0.0,0.0)
# j1=-1.4964941323816858,j2=-2.9646096201874874,j3=1.1310232096679318,j4=0.701029220063738,j5=1.8745245227962535,j6=-1.5241057865636274
jpose0=[-0.2, 2.7, -4.0,-2.3,1.6, -1.3]
robot.movej(jpose0,1.0,1.0,0.0,0.0)
robot.movej({"x":-0.296,"y":-0.295,"z":0.285,"rz":60.0 / 180.0 * 3.14,"ry":-5.0 / 180.0 * 3.14,"rx":81.0 / 180.0 * 3.14},1.0,1.0,0.0,0.0)
fk_resp = robot.kinematics_forward(jpose0)
print(fk_resp)
ik_resp  = robot.kinematics_inverse(fk_resp.pose)
print(ik_resp)
print(robot.kinematics_forward(ik_resp.joint_positions))