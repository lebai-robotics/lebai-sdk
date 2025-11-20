# Copyright 2022-2025 lebai.ltd
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
from pylebai import l_master

def main():
    # Check command line arguments
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <serial_port>", file=sys.stderr)
        print(f"Example (Windows): {sys.argv[0]} COM3", file=sys.stderr)
        print(f"Example (Linux): {sys.argv[0]} /dev/ttyUSB0", file=sys.stderr)
        return 1

    port_name = sys.argv[1]

    try:
        print(f"Connecting to gripper on port: {port_name}")

        # Create gripper instance
        # This will establish Modbus RTU communication over RS485
        # with 115200 baud rate, 8N1, Modbus address 1
        gripper = l_master.Gripper(port_name)

        print("Gripper connected successfully!")
        gripper.set_position(30)
        time.sleep(1)
        gripper.set_position(70)
        time.sleep(5)
        gripper.do_calibration()
        time.sleep(5)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0

if __name__ == "__main__":
    sys.exit(main())

