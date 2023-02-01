# .net平台应用文档


[Nuget包地址](https://www.nuget.org/packages/lebai/)

## 通过nuget安装'

```bash
dotnet add package lebai
```

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
using lebai.l_master;


# in Main
Robot robot = new Robot("xxx.xxx.xxx.xxx", true);
robot.stop_sys();
Console.WriteLine($"stop...");
robot.start_sys();
Console.WriteLine($"start...");
DoubleVector jp1 = new DoubleVector();
jp1.Add(0.0);
jp1.Add(-60.0 / 180.0 * 3.14);
jp1.Add(80.0 / 180.0 * 3.14);
jp1.Add(-10.0 / 180.0 * 3.14);
jp1.Add(-60.0 / 180.0 * 3.14);
jp1.Add(0.0);
robot.movej(jp1, 1.0, 1.0, 0.0, 0.0);
```

4. 编译运行

```
dotnet build
# run your console application.
```



