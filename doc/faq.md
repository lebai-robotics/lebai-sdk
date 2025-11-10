FAQ
---------------------

# 如何在CMake项目中使用lebai c++包开发。

1. 通过deb包或者源码安装。
2. 创建您的CMake项目，在CMakeLists.txt中添加如下内容：

    ```
    find_package(lebai REQUIRED)
    target_link_libraries(your_target_name lebai::lebai-cpp)
    ```


# 机械臂界面上坐标系统中，Rz、Ry、Rx是如何定义的。
乐白机械臂界面的Rx,Ry,Rz表示机械臂姿态，是由EulerZYX确定的。
 
界面显示的Rz，Ry，Rx为角度，将其转换为弧度后，它和旋转矩阵的关系如下：
 
\f[ RotationMatrix = \begin{bmatrix}
 \cos(Rz) & -sin(Rz) & 0 \\ 
 sin(Rz) & cos(Rz) & 0\\ 
 0 & 0 & 1
 \end{bmatrix}\begin{bmatrix}
 cos(Ry) & 0 & sin(Ry)\\ 
 0 & 1 & 0 \\ 
 -sin(Ry) & 0 & cos(Ry)
 \end{bmatrix}\begin{bmatrix}
 1 & 0 & 0 \\ 
 0 & cos(Rx) & -sin(Rx) \\ 
 0 & sin(Rx)  & cos(Rx) 
 \end{bmatrix} \f]

 # 夹爪的轴位置关系

 `gripper`中一共有6个关节，按照左右对称分布。

 - gripper_r_joint1
 - gripper_r_joint2
 - gripper_r_joint_finger
 - gripper_l_joint1
 - gripper_l_joint2
 - gripper_l_joint_finger

 夹爪的张开角度`amplitude`由一个[0,100]的整数表示，0表示完全闭合，100表示完全打开。

 
 夹爪的机械结构为类似于挖掘机的结构，因此关节角度需要满足以下关系：

\f[
angle = \frac{60.0}{180.0} \times \pi \times \frac{amplitude}{100.0} = \frac{\pi \times amplitude}{300}
\f]

\f[
\begin{aligned}
gripper\_r\_joint1 &= angle \\
gripper\_r\_joint2 &= -angle \\
gripper\_r\_joint\_finger &= -angle \\
gripper\_l\_joint1 &= angle \\
gripper\_l\_joint2 &= -angle \\
gripper\_l\_joint\_finger &= -angle
\end{aligned}
\f]
