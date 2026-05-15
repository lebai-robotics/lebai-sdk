# lebai-sdk

[![zh](https://img.shields.io/badge/lang-zh-red.svg)](https://github.com/lebai-robotics/lebai-sdk/blob/master/README.md)
[![en](https://img.shields.io/badge/lang-en-green.svg)](https://github.com/lebai-robotics/lebai-sdk/blob/master/README.en.md)

乐白机器人 SDK，用于发现、连接和控制乐白机械臂。当前重构版本为
`2.0.0`，核心 C++ SDK 已迁移到 `json-rpc-cxx` + `nlohmann/json` 的
JSON-RPC 实现，并通过 SWIG 生成 Python、.NET 和 Java 绑定。

## 状态

| 模块 | CI |
| --- | --- |
| 总发布 | [![Release][release_svg]][release_link] |
| C++ Linux x64 | [![C++ Linux][cpp_linux_svg]][cpp_linux_link] |
| C++ Linux arm64 | [![C++ Linux arm64][cpp_linux_arm64_svg]][cpp_linux_arm64_link] |
| C++ Windows | [![C++ Windows][cpp_windows_svg]][cpp_windows_link] |
| Python 发布 | [![Python release][python_release_svg]][python_release_link] |
| .NET 发布 | [![.NET release][dotnet_release_svg]][dotnet_release_link] |
| Java 发布 | [![Java release][java_release_svg]][java_release_link] |

[release_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/release.yml/badge.svg
[release_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/release.yml
[cpp_linux_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/linux_cpp_release.yml/badge.svg
[cpp_linux_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/linux_cpp_release.yml
[cpp_linux_arm64_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/linux_cpp_arm64_release.yml/badge.svg
[cpp_linux_arm64_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/linux_cpp_arm64_release.yml
[cpp_windows_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/windows_cpp_release.yml/badge.svg
[cpp_windows_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/windows_cpp_release.yml
[python_release_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/python_release.yml/badge.svg
[python_release_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/python_release.yml
[dotnet_release_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/dotnet_release.yml/badge.svg
[dotnet_release_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/dotnet_release.yml
[java_release_svg]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/java_release.yml/badge.svg
[java_release_link]: https://github.com/lebai-robotics/lebai-sdk/actions/workflows/java_release.yml

## SDK 组成

SDK 公共接口主要分为四部分：

- `Discovery`: 发现局域网内的乐白控制器和设备。
- `Robot`: 机器人主控制接口，包含运动、IO、任务、配置、系统等 RPC。
- `LuaRobot`: 面向 Lua 风格控制器通信的兼容接口。
- `Gripper`: 直接 RS485 夹爪接口，独立于主 Robot JSON-RPC 通道。

## 安装

### Python

Python 包名为 `pylebai`：

```bash
pip install pylebai
```

更多说明见 [docs/python.md](docs/python.md)。

### .NET / C#

NuGet 包名为 `lebai`：

```bash
dotnet add package lebai
```

`lebai` NuGet 包包含 Linux x64、Linux arm64 和 Windows x64 native runtime
assets，C# 用户不需要额外安装平台 runtime 包。更多说明见
[docs/dotnet.md](docs/dotnet.md)。

### Java

Java Maven 坐标为：

```xml
<dependency>
  <groupId>io.github.liufang-robot</groupId>
  <artifactId>lebai-java</artifactId>
  <version>2.0.0</version>
</dependency>
```

Java 源码包名仍为 `org.sdk.lebai`，以保持现有 SWIG API 兼容。更多说明见
[docs/java.md](docs/java.md)。

## 从源码构建

### Linux

基础依赖：

- C++14 编译器，例如 GCC 或 Clang
- CMake 3.15+
- SWIG 4.0+
- Python 3 和 venv，构建 Python 包时需要
- .NET SDK，构建 C# 包时需要
- JDK 11 和 Maven，构建 Java 包时需要
- Doxygen 和 Graphviz，构建文档时需要

Ubuntu / Debian 示例：

```bash
sudo apt install build-essential cmake python3-dev python3-venv \
  doxygen graphviz openjdk-11-jdk maven
```

如果系统 SWIG 版本过旧，可以使用用户级安装：

```bash
uv tool install swig
swig -version
```

常用构建命令：

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

构建语言绑定：

```bash
cmake -S . -B build-python -DBUILD_PYTHON=ON -DBUILD_TESTING=OFF
cmake --build build-python --target python_package

cmake -S . -B build-dotnet -DBUILD_DOTNET=ON -DBUILD_TESTING=OFF
cmake --build build-dotnet --target dotnet_package

cmake -S . -B build-java -DBUILD_JAVA=ON -DBUILD_TESTING=OFF -DSKIP_GPG=ON
cmake --build build-java --target java_package
```

### Windows

推荐使用 Visual Studio 2022 + CMake。构建 C#、Python 或 Java 绑定时，还需要
对应语言的 SDK、SWIG 和 Maven/JDK 等工具。Windows native 包使用静态 MSVC
runtime，以减少 C# 用户对额外 Visual C++ Redistributable 的依赖。

## 常用 CMake 选项

| 选项 | 默认值 | 说明 |
| --- | --- | --- |
| `BUILD_CXX` | `ON` | 构建 C++ SDK |
| `BUILD_PYTHON` | `OFF` | 构建 Python 绑定 |
| `BUILD_DOTNET` | `OFF` | 构建 .NET / C# 绑定 |
| `BUILD_JAVA` | `OFF` | 构建 Java 绑定 |
| `BUILD_TESTING` | `ON` | 构建测试程序 |
| `BUILD_EXAMPLES` | `ON` | 构建示例程序 |
| `BUILD_DEB` | `OFF` | 生成 Debian 包 |
| `BUILD_DOCUMENTATION` | `OFF` | 生成 Doxygen 文档 |
| `TEST_ROBOT_IP` | `127.0.0.1` | 测试目标控制器 IP |
| `ENABLE_TSAN` | `OFF` | 启用 ThreadSanitizer |

本地模拟 L master controller 测试目标通常使用 `127.0.0.1:3030`。

## 文档

- [docs/architecture.md](docs/architecture.md): SDK 分层和请求流。
- [docs/rpc-protocol.md](docs/rpc-protocol.md): RPC 协议、wire name 映射和迁移策略。
- [docs/develop.md](docs/develop.md): 添加新 RPC/API 的开发流程。
- [docs/bindings.md](docs/bindings.md): SWIG 绑定和语言包结构。
- [docs/python.md](docs/python.md): Python 使用说明。
- [docs/dotnet.md](docs/dotnet.md): .NET / C# 使用说明。
- [docs/java.md](docs/java.md): Java/Maven 使用和发布说明。
- [docs/faq.md](docs/faq.md): 常见问题。

## 第三方依赖

| 软件 | 协议 | 说明 |
| --- | --- | --- |
| Asio | MPL-2.0 | 串口/异步通信基础设施 |
| mdns | Public Domain | 设备发现 |
| nlohmann/json | MIT | JSON DTO 和序列化 |
| cpp-httplib | MIT | HTTP transport |
| json-rpc-cxx | MIT | JSON-RPC request/response |
| SWIG | GPL-compatible | Python/.NET/Java 绑定生成 |

## 协议和支持

SDK 使用需要乐白机械臂控制器软件版本大于等于 `3.1.5`。控制器版本可以在机械臂
Web 界面左上方查看。

Lua 接口文档可参考 [SDK 在线文档](http://help.lebai.ltd/sdk/)。本 SDK 的
Robot API 正在按 `lebai-proto` RPC 协议逐步迁移和校准。
