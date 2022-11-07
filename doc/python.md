
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

### API

[启动停止](#启动停止)  
[运动](#运动)  
[状态数据](#状态数据)  
[控制箱IO](#控制箱io)  
[法兰盘IO](#法兰盘io)  
[拓展IO](#拓展io)  
[夹爪](#夹爪)  
[灯板](#灯板)  
[信号量](#信号量)  
[程序控制](#程序控制)  
[机器人学](#机器人学)  

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
#a  主轴的关节加速度(rad/s^2^)
#v  主轴的关节速度(rad/s)
#t  运动时间(s).当t>0时，速度参数v和加速度a无效
#r  交融半径(m).0表示关闭平滑运动;1表示启用平滑运动(最大交融半径为1m)
movej(p,a,v,t,r)

#p  坐标位置，或者关节位置(将通过运动学正解转为坐标位置)
#a  工具空间加速度 (m/s^2^)
#v  工具空间速度 (m/s).注意这里的速度和加速度指的是在工具空间上的描述。
#其他参数同movej
movel(p, a, v, t, r)

#p  关节位置，或者坐标位置(将通过运动学反解转为关节位置)
#v  每个关节的速度 (rad/s).如该值为数字，则表示所有关节速度相同。
#a  每个关节的加速度 (rad/s2).如该值为数字，则表示所有关节加速度相同。
#t  运动时间 (s)
move_pvat(p, v, a, t)
```

#### 状态数据

#### 控制箱IO

#### 法兰盘IO

#### 拓展IO

#### 夹爪

#### 灯板

#### 信号量

#### 程序控制

#### 机器人学
