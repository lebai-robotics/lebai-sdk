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

from pylebai import Robot


def main():
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        print(
            f"Usage: {sys.argv[0]} <robot_ip> <task_name> [sim]",
            file=sys.stderr,
        )
        return 1

    if len(sys.argv) == 4 and sys.argv[3] != "sim":
        print("The optional third argument must be 'sim'", file=sys.stderr)
        return 1

    robot = Robot(sys.argv[1], len(sys.argv) == 4)
    robot.start_sys()

    task_id = robot.start_task(sys.argv[2])
    print(f"Started task {task_id}")

    print_result = robot.wait_task(task_id)
    print("Task print result:")
    print(print_result, end="" if print_result.endswith("\n") else "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
