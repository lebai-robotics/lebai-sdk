# ChangeLog

## 1.3.8

优化构建系统

## 1.3.7

优化构建系统

调整 gripper 接口命名

## 1.3.5

优化 Windows 平台 python 包的构建支持

## 1.3.4

添加独立的 gripper 接口

## 1.3.3

添加 init_claw 接口
load_task_list -> get_task_list 修正
修正 get_motion_state 接口

## 1.3.2

添加 get_kin_data
升级 dotnet sdk 为 8.0

## 1.3.1

get_robot_mode -> get_robot_state
添加 get_phy_data
wait_task 返回 string

## 1.2.2

添加`c#`语言和`java`语言捕获异常的处理。

## 1.2.1

添加`win32`流水线编译构建。

##

1.2.0
添加存储项的接口。

## 1.1.32

添加 wait_task 接口

## 1.1.31

python 镜像升级

## 1.1.30

修正 ci 中的错误

## 1.1.29

修正 move_pvat 不能正常工作的问题。

## 1.1.28

修正 c++安装包 CMake Config 的错误，添加使用文档。

## 1.1.27

调整构建。

## 1.1.25

优化代码格式和构建。

## 1.1.24

暂时移除 windows 上 python3.12 生成。

## 1.1.23

修正 pypi 生成的问题。

## 1.1.22

调整 python 构建 token。

## 1.1.19

调整 python 构建 token。

## 1.1.19

调整 python 构建 token。

## 1.1.18

尝试调整 python 包名字。

## 1.1.17

修正 python 不能捕获异常的问题。

## 1.1.16

添加接口`get_claw_data`返回数据结构而不是 tuple。

## 1.1.15

完善文档，python 支持更多版本，python 生成 aarch64 的包。

## 1.1.14

修正 IK 的 bug

## 1.1.13

独立的 FAQ 文件

## 1.1.12

修正 pypi 持续集成的若干问题。

## 1.1.10

添加设置串口波特率和校验位的接口

## 1.1.9

添加接口连接抛出异常的处理

## 1.1.8

speedl 添加参考坐标系

修正.net 包描述问题

修正注释和文档的一些 bug

## 1.1.7

README 添加多语言支持

## 1.1.1

添加 speedj 和 speedl 的实现和文档

## 1.1.0

添加 LuaRobot 对象，可以向机械臂发送 lua 指令

## 1.0.21

添加 is_network_connected 方法。

## 1.0.20

修正 windows 平台编译 WebSocket 的问题。

## 1.0.17

添加 python2 的构建选项
修正旋转部分数据表示 x 和 z 反了的问题
完善文档

## 1.0.14

文档更新，持续集成更新，修正程序的错误

## 1.0.11

文档更新，持续集成更新

## 1.0.10

支持 dotnet 的编译生成

## 1.0.9

修正文档错误

修正 rpc 的逻辑错误，该逻辑错误导致 rpc 调用可能卡死的问题。

## 1.0.8

添加关节跟随运动的接口 towardj

添加圆弧运动的接口 movec

调整位置数据结构，关节位置修改为数组，坐标位置修改为键值

修正多线程访问的 bug.

## 1.0.3 -- 2022-11-17

修正多线程访问的 bug.

## 1.0.2 -- 2022-11-06

修改字典数据在 python 中的的 print 显示。

修改 ik 计算的传入数据结构，关节位置数据初值修改为字典。

添加编译选项 `TEST_ROBOT_IP` ，该设置用来设置单元测试连接的测试机器人的 IP 地址，默认为 `127.0.0.1`。

## 1.0.1 -- 2022-10-28

修改 get_xxx_joint_positions 返回值为字典类型数据。

添加和完善若干数据类型的 `__repr__` 函数，使得这些数据类型可以直接打印。

添加 `set_do` , `set_flange_do`, `set_extra_do` 值参数为 int 类型的接口。

## 1.0.2 -- 2022-11-06

修正字典类型数据打印的格式问题。

添加测试框架，采用 l-master 容器进行测试。
