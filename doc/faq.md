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
 
$ RotationMatrix = \begin{bmatrix}
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
 \end{bmatrix} $