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

import sys
import time

from pylebai import Robot


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <robot_ip> [sim] [--move]", file=sys.stderr)
        return 1

    ip = sys.argv[1]
    simulator = "sim" in sys.argv[2:]
    run_motion = "--move" in sys.argv[2:]

    robot = Robot(ip, simulator)
    print("system:", robot.get_system_info().name)
    print("state:", robot.get_robot_state())
    print("estop reason:", robot.get_estop_reason())

    if run_motion:
        robot.start_sys()
        time.sleep(1)
        robot.movej([0.0, 0.0, 0.0, 0.0, 0.0, 0.5], 1.0, 0.5, 0.0, 0.0)
        robot.movej([0.0, 0.0, 0.0, 0.0, 0.0, 0.0], 1.0, 0.5, 0.0, 0.0)
        robot.wait_move()
    else:
        print("Motion skipped. Pass --move to run the motion demo.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
