
# python应用文档

## 通过pypi安装

TODO

## 通过源码构建安装

### 编译python包

您可以参考项目的README文档来安装项目的依赖，然后进行构建编译。
如果所有的依赖都满足，您可以用下面的命令来构建python语言的支持：

```bash
cmake -S. -Bbuild -DBUILD_PYTHON=ON
cmake --build build --target python_package
```

### 安装

构建完成后，在`build/python/dist/`目录下会生成python的whl包，可以直接使用pip进行安装。

```bash
sudo pip3 install build/python/dist/lebai-xxx.whl
```

您可以通过如下命令来查看安装的包的信息。

```bash
pip3 show lebai
```

您可以通过如下命令来卸载。

```bash
sudo pip3 uninstall lebai
```

### 运行

安装完成后可以直接使用lebai包。

```python
from lebai import robot
robot = robot.Robot("172.17.0.5",True)
robot.movej({"j1": 1.0,"j2": -1.0471975511965976,"j3": 1.3962634015954636,"j4": -0.17453292519943295,"j5": -1.0471975511965976,"j6": 0.0},1.0,1.0,0.0,0.0)
```

## API

### Robot

- [启动停止](#启动停止)  
- [运动](#运动)  
- [状态数据](#状态数据)  
- [控制箱IO](#控制箱io)  
- [法兰盘IO](#法兰盘io)  
- [拓展IO](#拓展io)  
- [夹爪](#夹爪)  
- [灯板](#灯板)  
- [信号量](#信号量)  
- [程序控制](#程序控制)  
- [机器人学](#机器人学)  

#### 启动停止

```python
#启动机器人(上电)
start_sys()

#停止机器人(下电)
stop_sys()

#关闭机器人电源(关机)
powerdown()

#停止运动(但不下电)
stop()

#紧急停止(急停)
estop()

#进入示教模式
teach_mode()

#退出示教模式
end_teach_mode()

#暂停运动
pause()

#恢复运动
resume()
```

#### 运动

```python
#关节运动
#p  关节位置或坐标位置，类型为dict
#a  主轴的关节加速度(rad/s.pow(2))
#v  主轴的关节速度(rad/s)
#t  运动时间(s).当t>0时，速度参数v和加速度a无效
#r  交融半径(m).0表示关闭平滑运动;1表示启用平滑运动(最大交融半径为1m)
#return 整型数字代表此运动的id,若为0则启动运动失败
movej(p,a,v,t,r)

#p  坐标位置，或者关节位置(将通过运动学正解转为坐标位置)
#a  工具空间加速度 (m/s^2^)
#v  工具空间速度 (m/s).注意这里的速度和加速度指的是在工具空间上的描述。
#其他参数同movej
#return 整型数字代表此运动的id,若为0则启动运动失败
movel(p, a, v, t, r)

#p  关节位置，或者坐标位置(将通过运动学反解转为关节位置)
#v  每个关节的速度 (rad/s).如该值为数字，则表示所有关节速度相同。
#a  每个关节的加速度 (rad/s2).如该值为数字，则表示所有关节加速度相同。
#t  运动时间 (s)
move_pvat(p, v, a, t)

#等待运动结束
#id  指定运动的id.如该值为0，则表示等待所有运动完成
wait_move(id)
```

#### 状态数据

```python
#获取机器人当前状态
#return 整形状态码,代表机器人当前的状态
#  状态码 |      状态     |       说明  
#  ------ | -------------  | -------------
#    -1  |   控制系统故障  |  机器人软件控制系统异常
#    0   |   硬件通讯故障  |  机器人硬件通讯故障
#    1   |   已急停       |  机器人处于急停状态，请确认安全性
#    2   |   初始化中     |  机器人初始化中
#    4   |   初始化完成   |  机器人电源已开启
#    5   |   空闲         |  机器人处于空闲状态
#    6   |   暂停         |  机器人处于暂停中状态
#    7   |   运行中       |  机器人运行中
#    8   |   更新中       |  机器人系统更新中
#    9   |   启动中       |  机器人初始化完成到空闲的启动过程中
#     10 |   正在停止     |  机器人空闲状态转到停止状态
#     11 |   示教中       |  机器人处于示教模式中
#     12 |   已停止       |  机器人处于停止状态，非急停状态
get_robot_mode()

#获取反馈关节位置
#return 包含"j1"-"j6"的字典，代表每个关节的位置
get_actual_joint_positions()

#获取规划关节位置
#return 包含"j1"-"j6"的字典，代表每个关节的位置
get_target_joint_positions()

#获取反馈关节速度
#return 包含6个元素的列表，代表每个关节的速度
get_actual_joint_speed()

#获取规划关节速度
#return 包含6个元素的列表，代表每个关节的速度
get_target_joint_speed()

#获取反馈工具位置
#return 包含6个元素的切片(slice)，代表tcp位置
get_actual_tcp_pose()

#获取规划工具位置
#return 包含6个元素的切片(slice)，代表tcp位置
get_target_tcp_pose()

#获取关节温度
#joint  关节编号
#return 指定关节的温度
get_joint_temp(joint)

#获取关节反馈力矩
#return 一个包含6个元素的列表，代表每个关节的力矩
get_actual_joint_torques()

#获取关节规划力矩
#return 一个包含6个元素的列表，代表每个关节的力矩
get_target_joint_torques()
```

#### 控制箱IO

```python
#设置数字输出
#pin  端口.从0开始
#value  待设置的值，可以是1/0，也可以是True/False
set_do(pin,value)

#获取数字输入
#pin  端口.从0开始
#return True/False
get_di(pin)

#设置模拟输出
#pin  端口库.从0开始
#value  待设置的值，可以是1/0，也可以是True/False
set_ao(pin,value)

#获取模拟输入
#pin  端口.从0开始
#return True/False
get_ai(pin)
```

#### 法兰盘IO

```python
#设置数字输出
#pin  端口.从0开始
#value  待设置的值，可以是1/0，也可以是True/False
set_flange_do(pin,value)

#获取数字输入
#pin  端口.从0开始
#return True/False
get_flange_di(pin)
```

#### 拓展IO

```python
#设置数字输出
#pin  端口.从0开始
#value  待设置的值，可以是1/0，也可以是True/False
set_extra_do(pin,value)

#获取数字输入
#pin  端口.从0开始
#return True/False
get_extra_di(pin)

#设置模拟输出
#pin  端口库.从0开始
#value  待设置的值，可以是1/0，也可以是True/False
set_extra_ao(pin,value)

#获取模拟输入
#pin  端口.从0开始
#return True/False
get_extra_ai(pin)

```

#### 夹爪

```python
#设置夹爪参数
#force  力度(0-100)
#amplitude  张合幅度(0-100)
set_claw(force,amplitude)

#获取夹爪参数
#return 一个元组tuple,第一个数为夹爪力度，第二个数为幅度，第三个数为夹爪是否稳定
get_claw()
```

#### 灯板

```python
#设置led灯状态
#mode  亮灯模式.0：不变；1：关闭；2：常亮；3：呼吸；4：均分旋转；5：同色旋转；6：闪烁
#speed  速度.1：快速；2：正常；3：慢速
#color  最多包含 4 个 0 ~ 15 之间的整数
set_led(mode,speed,color)


#设置声音
#voice 声音列表
#volume 音量.0：静音；1：低；2：正常；3：高
set_voice(voice,volume)

#开关风扇
#status  是否开启风扇.1为关闭，2为开启
set_fan(status)
```

#### 信号量

```python
#设置信号量
#index  信号量下标(0-255)
#value  待设置的信号量值(32位有符号整形)
set_signal(index,value)

#获取信号量
#index  信号量下标(0-255)
#return  设置的信号量值(32位有符号整形)
get_signal(index)

#增加指定下标的信号量值
#index  信号量下标(0-255)
#value  待设置的信号量值(32位有符号整形)
add_signal(index,value)
```

#### 程序控制

```python
#调用指定场景
#id  所调用的场景的id
#return  执行此场景的taskid
scene(id)

#查询任务列表
#return  包含多个taskid的列表
load_task_list()

#暂停任务
#id  指定暂停的任务的taskid
#time  暂停的时间
#wait  是否等待恢复
pause_task(id,time,wait)

#恢复任务
#id  要恢复的暂停任务的taskid
resume_task(id)

#取消任务
#id  要取消的队列中的任务的taskid
cancel_task(id)
```

#### 机器人学

```python
#通过正向运动学将关节角度转成笛卡尔位置和姿态
#joints  关节角度.包含"j1"-"j6"的字典
#return  返回包含一个bool值和string-double键值对的字典，bool表示正解是否成功
kinematics_forward(joints)

#通过逆向运动学反解，将笛卡尔位置和姿态转成关节角度
#vector  包含6个元素的列表，代表笛卡尔坐标
#joints  包含6个元素的列表，代表关节初始位置
kinematics_inverse(vector,joints)

#将a和b转为4x4的齐次矩阵A和B，然后将C=AB转为位姿返回
#a  位姿a
#b  位姿b
#return 返回相乘后的位姿
pose_times(a,b)

#将a转为4x4的齐次矩阵A,返回A的逆的位姿表示
#a  位姿a
#return  逆矩阵的位姿
pose_inverse(a)

example:
#原始关节角度
jpose0={'j1':-0.2, 'j2':2.7, 'j3':-4.0, 'j4':-2.3, 'j5':1.6, 'j6':-1.3}
fk_resp = robot.kinematics_forward(jpose0)
#经正向转化后的笛卡尔位姿，ok=True代表转化成功
#{1=0.36081962328886585,2=-0.19076037719926695,3=0.3537290579173756,4=-0.7527842092971923,5=-1.036487456216255,6=-2.623180719146089,ok=true}
print(fk_resp)
ik_resp  = robot.kinematics_inverse(fk_resp.pose)
#将笛卡尔位姿反向转化为关节角，ok=True代表转化成功
#{j1=-0.20000195232787238,j2=-1.613927211382733,j3=-1.4217944465440524,j4=-3.7058747067503983,j5=-1.6000033762300137,j6=1.841591302572852,ok=true}
print(ik_resp)
#经过两次转化后的关节角与原始关节角是相同的，且精度更高
#{1=0.3608198951635695,2=-0.19076058061714882,3=0.35372925635352953,4=-0.7527992994328961,5=-1.0364868722925928,6=-2.6231651290324502,ok=true}
print(robot.kinematics_forward(ik_resp.joint_positions))
```
### Zeroconf

#### 设备发现

```python
#返回当前局域网内最多16台可发现的设备，并返回控制器信息（包含信息：hostname、ip_address、mac_address、model、ds_version、rc_version、id
resolve()



example:
#从lebai导入设备发现的模块
from lebai import zeroconf
#实例化一个Discovery类用来存储设备发现的信息
d = zeroconf.Discovery()
#搜索局域网内设备，返回一个最多16个元素的列表
controllers = d.resolve()
#根据局域网内发现的设备，建立连接
robot = l_master.Robot(controllers[0].ip_address)
```
