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
sudo pip3 install build/python/dist/lebai-1.0-cp35-cp35m-linux_x86_64.whl
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




