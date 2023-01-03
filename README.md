lebai-sdk的源代码仓库，可以用于控制乐白机械臂。

[SDK在线文档](http://help.lebai.ltd/sdk/)

# 包管理直接安装

python开发可以直接从[PyPI](https://pypi.org/project/pylebai/)安装。
```
pip install pylebai
```

# 从源代码构建安装

## Ubuntu&&Debian平台
### 依赖
- gcc or clang support c++14
- cmake >= 3.18
- swig >= 4.0
- python3 >= 3.5 (可选,如果需要生成python的接口库)
- doxygen (可选,如果需要C++文档)
- jdk, maven(可选,如果需要生成java的接口库)

相对旧的发行版系统cmake和swig版本过低，不能使用，可以通过pip安装较新的版本:
```bash
sudo apt install build-essential python3-pip dpkg-dev
sudo pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple -r requirements.txt
# 如果需要生成文档，还需要安装doxygen。
sudo apt install doxygen graphviz
# 如果需要生成python的wrapper包。
sudo apt install python3-dev
```


### 编译
```bash
# 生成编译配置
cmake -S. -Bbuild -DBUILD_PYTHON=ON -DBUILD_DEB=ON -DBUILD_TESTING=OFF 
# 编译
cmake --build build
# 运行单元测试
cmake --build build --target test
## 生成DEB包
cd build
cpack
```

## Windows平台 Visual Studio
### 依赖
- Visual Studio(C++ Support)
- CMake >= 3.18
- python3 >= 3.5 (可选,如果需要生成python的接口库)

安装python库时，选择 `Download debug binaries`

在Visual Studio中载入CMake工程，即可以生成构建运行测试等。

目前Windows平台下python的binding部分还存在问题无法工作。



## 编译选项

 - BUILD_PYTHON: 是否编译python接口 默认为OFF
 - PYTHONPATH: PYTHON的安装目录
 - BUILD_DOTNET: 是否编译.NET接口 默认为OFF（请先安装好.NET的开发环境）
 - BUILD_JAVA: 是否编译JAVA接口 默认为OFF（请先安装好JDK和Maven）
 - BUILD_TESTING: 是否编译C++测试程序 默认为ON 
 - TEST_ROBOT_IP: 测试程序的机器人IP地址，正确的设置该值用于单元测试 默认为127.0.0.1
 - BUILD_EXAMPLES: 是否编译示例程序 默认为ON
 - BUILD_DEB: 是否生成DEB包的构建 默认为OFF


## 使用
您可以通过docs目录下的文档了解更多各语言的信息。

### Python
build目录下会生成python的whl包，可以直接使用:
```python
cd build/python/dist
## 安装
pip3 install pylebai-xxx.whl
### XXX取决于您的sdk版本，python版本，操作系统平台。
## 卸载
pip3 uninstall pylebai
```

# 第三方库
lebai-sdk使用如下第三方软件:

| 软件名      | 协议 | 官方网站 | 
| ----------- | ----------- |----------- |
| Asio      | Boost  | https://think-async.com/Asio/ |
| rapidjson      | MIT  | https://rapidjson.org/ |
| websocketpp      | BSD  | https://www.zaphoyd.com/websocketpp |
| mdns | public domain | https://github.com/mjansson/mdns |