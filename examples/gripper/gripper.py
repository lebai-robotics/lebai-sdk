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

def main():
    # Check command line arguments
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print(f"Usage: {sys.argv[0]} <serial_port> [position]", file=sys.stderr)
        print(f"Example (Windows): {sys.argv[0]} COM3 50", file=sys.stderr)
        print(f"Example (Linux): {sys.argv[0]} /dev/ttyUSB0 50", file=sys.stderr)
        return 1

    port_name = sys.argv[1]
    position = 50
    if len(sys.argv) == 3:
        try:
            if sys.argv[2].startswith("-"):
                raise ValueError()
            position = int(sys.argv[2], 10)
            if position > 100:
                raise ValueError()
        except ValueError:
            print(f"Invalid position: {sys.argv[2]}", file=sys.stderr)
            return 1

    try:
        from pylebai import gripper

        print(f"Connecting to gripper on port: {port_name}")

        # Create gripper instance
        # This will establish Modbus RTU communication over RS485
        # with 115200 baud rate, 8N1, Modbus address 1
        direct_gripper = gripper.Gripper(port_name)

        print("Gripper connected successfully!")
        print(f"Setting gripper position to: {position}")
        direct_gripper.set_position(position)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0

if __name__ == "__main__":
    sys.exit(main())
