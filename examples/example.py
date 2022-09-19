from lebai_sdk import robot
from lebai_sdk import motion
from lebai_sdk import posture
import time
## Robot instance
robot = robot.Robot("172.17.0.3", True)
time.sleep(2)
## Move Request data
move_req = motion.MoveRequest()
move_req.pose().mutable_joint().delta().set_joint([1.5,0,0,0,0,0])
move_req.param().set_v(0.2)
## Call movej
robot.movej(move_req)
## Change joint pose
move_req.pose().joint().delta().set_joint([-1.5,0,0,0,0,0])
## Call movej again
robot.movej(move_req)