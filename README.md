# 编译安装
## Windows平台
TODO
## Ubuntu&&Debian平台
### 依赖
 - python3 >= 3.5
 - cmake >= 3.18
 - gcc >= 5.4
 - swig >= 4.0
 - doxygen (可选)

安装依赖的方法：
相对早的发行版系统cmake和swig版本过低，不能使用，可以通过pip安装较新的版本:
```bash
sudo apt install python3-pip
sudo pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple -r requirements.txt
```
### 编译
```bash
mkdir build
cd build
cmake ..
## cmake .. -DBUILD_PYTHON=ON
make
```
编译选项
 - BUILD_PYTHON: 是否编译python接口
 - BUILD_TESTING: 是否编译测试程序
 - BUILD_EXAMPLES: 是否编译示例程序

# 后续工作
- 功能完成（目前为框架结构示例）
- Windows平台支持和相对应的跨平台的编译所需修改代码
- 代码优化
- 文档完善
- 开源后放入对应语言的包管理器，简化安装过程



