# .net平台应用文档

**.NET 包目标支持 Linux x64、Linux arm64 和 Windows x64；具体平台可用性以
CI 产物和发布包内容为准。**

[Nuget包地址](https://www.nuget.org/packages/lebai/)

## 通过 NuGet 安装

```bash
dotnet add package lebai
```

`lebai` 包包含托管 C# 程序集，并在 NuGet 的 `runtimes/<rid>/native`
目录中携带对应平台的原生库。为了兼容历史构建，仓库仍可能生成
`lebai.runtime.<rid>` 这类运行时包，但普通用户不应该需要手动安装所有
运行时包。

Windows 的 C# 原生库按静态 MSVC runtime (`/MT`) 构建，因此用户不应该只
为了加载 `lebai-native.dll` 而额外安装 Visual C++ Redistributable。

## 示例工程运行

您可以自行创建.net工程，并且在CSharp中使用lebai包进行开发。

一个简单的示例步骤如下：

1. 创建工程


```bash
# 创建
mkdir lebai-sdk-dotnet-example
cd lebai-sdk-dotnet-example
dotnet new console
```

2. 添加lebai packages

```bash
dotnet add package lebai
```

3. 编写代码


在程序中添加如下代码

```c#
using System;
using lebai;

# in Main
Robot robot = new Robot("xxx.xxx.xxx.xxx", true);
robot.StopSys();
Console.WriteLine($"stop...");
robot.StartSys();
Console.WriteLine($"start...");

double[] joints = {
    0.0,
    -60.0 / 180.0 * Math.PI,
    80.0 / 180.0 * Math.PI,
    -10.0 / 180.0 * Math.PI,
    -60.0 / 180.0 * Math.PI,
    0.0
};
robot.MoveJ(joints, 1.0, 1.0, 0.0, 0.0);
```

4. 编译运行

```
dotnet build
# run your console application.
```

## API 风格

推荐应用层使用 `lebai.Robot`。它是面向 C# 的轻量 facade，公开
`double[]`、`uint[]`、`int[]`、`string[]` 等普通 C# 类型，并在内部转换到
SWIG 生成的底层类型。

`lebai.l_master` 命名空间下仍包含 SWIG 生成的低层接口，例如 `DoubleVector`
和 `StrVector`。这些类型主要用于兼容和内部桥接，新代码不应优先直接使用。
