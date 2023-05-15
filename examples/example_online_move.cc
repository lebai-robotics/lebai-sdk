/**
 * Copyright 2022 lebai.ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <lebai/robot.hh>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    lebai::l_master::Robot robot("172.17.0.4", true);

    robot.start_sys();
    auto jp = robot.get_actual_joint_positions();
    //假设有一个大臂的姿态以四元数表示
    //我们先测试简单的例子，利用大臂的四元数数据来更新机械臂第一关节和第二关节的位置    
    while(1)
    {
        //获取传感器四元数数据
        double quat[4] = {0,0,0,0};
        // 请在这里获取四元数数据
        // quat[0] = ?;
        // quat[1] = ?;
        // quat[2] = ?;
        // quat[3] = ?;    
        // 利用传感器的四元数旋转计算出新的jp[0] jp[1]
        // 请在这里计算
        // jp[0] = ?;
        // jp[1] = ?;
        // 将新的机械臂位置发送给机械臂控制器端
        robot.towardj(jp, 3.0, 3, 0.0, 0.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
