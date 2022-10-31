# 从源代码构建安装
## Windows平台

- [MinGW-w64 (prefer x86_64-posix-seh)](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/)
- [cmake >= 3.18](https://cmake.org)
- [boost](https://www.boost.org/users/download/)
- python3 >= 3.5 (可选,如果需要生成python的接口库)

根据需求手动安装
> 安装完成后需将以下目录添加到环境变量PATH  
> python目录下的根目录、Scripts、Library\bin  
> mingw64目录下的根目录、bin  
> cmake目录下的bin
 
> 将boost目录添加到BOOST_ROOT变量

修改CMakeLists.txt：
TODO

```bash
#安装python依赖
pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple -r requirements.txt
# 生成编译配置
cmake -S. -Bbuild -DBUILD_PYTHON=ON -DBUILD_DEB=ON -DBUILD_TESTING=OFF 
# 编译
cmake --build build
```

## Ubuntu&&Debian平台
### 依赖
- gcc or clang support c++14
- cmake >= 3.18
- swig >= 4.0
- python3 >= 3.5 (可选,如果需要生成python的接口库)
- doxygen (可选,如果需要C++文档)

安装依赖的方法：
相对旧的发行版系统cmake和swig版本过低，不能使用，可以通过pip安装较新的版本:
```bash
sudo apt install build-essential python3-pip
sudo pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple -r requirements.txt
# 如果需要生成文档，还需要安装doxygen。
sudo apt install doxygen
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
编译选项
 - BUILD_PYTHON: 是否编译python接口 默认为OFF
 - BUILD_TESTING: 是否编译C++测试程序 默认为ON 
 - BUILD_EXAMPLES: 是否编译示例程序 默认为ON
 - BUILD_DEB: 是否生成DEB包的构建 默认为OFF



## 使用
您可以通过docs目录下的文档了解更多各语言的信息。

TODO
### Python
build目录下会生成python的whl包，可以直接使用。
```python
cd build/python/dist
## 安装
sudo pip3 install lebai-1.0-cpXX-cpXXm-linux_x86_64.whl
### XX取决于您的python版本
## 卸载
sudo pip3 uninstall lebai
```


# 后续工作
- 功能完成（目前为框架结构示例）
- Windows平台支持和相对应的跨平台的编译所需修改代码
- 代码优化
- 文档完善
- 开源后放入对应语言的包管理器，简化安装过程

# 第三方库
lebai-sdk使用如下第三方软件

| 软件名      | 协议 | 官方网站 | 
| ----------- | ----------- |----------- |
| Asio      | Boost  | https://think-async.com/Asio/ |
| rapidjson      | MIT  | https://rapidjson.org/ |
| websocketpp      | BSD  | https://www.zaphoyd.com/websocketpp |
| mdns | public domain | https://github.com/mjansson/mdns |