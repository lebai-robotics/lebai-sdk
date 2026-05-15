#pragma once

/*! \mainpage lebai
 * [TOC]
 * # 概述
 * Lebai机械臂All in
 * one开发包。开发包通过C++实现和机械臂的通讯,提供和机械臂控制器交互的接口,并且通过[SWIG](https://www.swig.org/
 * "SWIG")生成其它各种语言的接口库.
 *
 * 您可以通过项目的README文档来了解如何构建和安装本开发包.
 *
 * 您也可以基于本开发包创建使用其它语言的开发包.
 *
 * # SDK 组成
 * SDK 的公共接口主要分为四部分:
 *
 * - \ref lebai::zeroconf::Discovery "Discovery": 用于在局域网内发现 Lebai
 *   控制器和设备.
 * - \ref lebai::l_master::Robot "Robot": 主要的机器人控制器接口, 包括运动、
 *   IO、任务、配置和系统 RPC 调用.
 * - \ref lebai::l_master::LuaRobot "LuaRobot": 面向 Lua 风格控制器通信的兼容接口.
 * - \ref lebai::l_master::Gripper "Gripper": 直接 RS485 夹爪接口, 独立于 Robot
 *   的 JSON-RPC 主控制链路.
 *
 * # 如何开始
 * 查阅 \ref README.md "README" 文档,了解如何构建和安装本开发包.
 *
 * # ChangeLog
 *  查阅 \ref docs/changelog.md.
 *
 * # Python
 * 查看 \ref docs/python.md 来获取使用python开发相关的内容.
 *
 * # .net
 * 查看 \ref docs/dotnet.md 来获取在.net中使用C\#开发相关的内容.
 *
 * # Develop
 * 如果需要开发sdk，添加新的功能，可以参考 \ref docs/develop.md.
 *
 * # FAQ *
 * 参考 \ref docs/faq.md.
 *
 *
 */
