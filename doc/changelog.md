# ChangeLog

## 1.2.2
添加`c#`语言和`java`语言捕获异常的处理。

## 1.2.1
添加`win32`流水线编译构建。

##
1.2.0
添加存储项的接口。

## 1.1.32
添加wait_task接口

## 1.1.31
python镜像升级

## 1.1.30
修正ci中的错误

## 1.1.29
修正move_pvat不能正常工作的问题。

## 1.1.28
修正c++安装包CMake Config的错误，添加使用文档。

## 1.1.27
调整构建。

## 1.1.25
优化代码格式和构建。

## 1.1.24
暂时移除windows上python3.12生成。

## 1.1.23
修正pypi生成的问题。

## 1.1.22

调整python构建token。

## 1.1.19

调整python构建token。

## 1.1.19

调整python构建token。

## 1.1.18

尝试调整python包名字。

## 1.1.17

修正python不能捕获异常的问题。

## 1.1.16

添加接口`get_claw_data`返回数据结构而不是tuple。

## 1.1.15

完善文档，python支持更多版本，python生成aarch64的包。

## 1.1.14

修正IK的bug

## 1.1.13

独立的FAQ文件

## 1.1.12

修正pypi持续集成的若干问题。

## 1.1.10

添加设置串口波特率和校验位的接口

## 1.1.9

添加接口连接抛出异常的处理

## 1.1.8

speedl添加参考坐标系

修正.net包描述问题

修正注释和文档的一些bug

## 1.1.7

README添加多语言支持

## 1.1.1

添加speedj和speedl的实现和文档

## 1.1.0

添加LuaRobot对象，可以向机械臂发送lua指令

## 1.0.21

添加is_network_connected方法。

## 1.0.20

修正windows平台编译WebSocket的问题。

## 1.0.17

添加python2的构建选项
修正旋转部分数据表示x和z反了的问题
完善文档

## 1.0.14

文档更新，持续集成更新，修正程序的错误

## 1.0.11

文档更新，持续集成更新

## 1.0.10

支持dotnet的编译生成

## 1.0.9

修正文档错误

修正rpc的逻辑错误，该逻辑错误导致rpc调用可能卡死的问题。

## 1.0.8

添加关节跟随运动的接口towardj

添加圆弧运动的接口movec

调整位置数据结构，关节位置修改为数组，坐标位置修改为键值



修正多线程访问的bug.

## 1.0.3 -- 2022-11-17

修正多线程访问的bug.

## 1.0.2 -- 2022-11-06

修改字典数据在python中的的print显示。

修改ik计算的传入数据结构，关节位置数据初值修改为字典。

添加编译选项 `TEST_ROBOT_IP` ，该设置用来设置单元测试连接的测试机器人的IP地址，默认为 `127.0.0.1`。

## 1.0.1 -- 2022-10-28

修改get_xxx_joint_positions返回值为字典类型数据。

添加和完善若干数据类型的 `__repr__` 函数，使得这些数据类型可以直接打印。

添加 `set_do` , `set_flange_do`, `set_extra_do` 值参数为int类型的接口。


## 1.0.2 -- 2022-11-06

修正字典类型数据打印的格式问题。

添加测试框架，采用l-master容器进行测试。








