#pragma once

/*! \mainpage lebai
 * [TOC]
 * # 概述 
 * Lebai机械臂All in one开发包。开发包通过C++实现和机械臂的通讯,提供和机械臂控制器交互的接口,并且通过[SWIG](https://www.swig.org/ "SWIG")生成其它各种语言的接口库.
 * 
 * 您可以通过项目的README文档来了解如何构建和安装本开发包.
 * 
 * 您也可以基于本开发包创建使用其它语言的开发包.
 * 
 * # 如何开始
 * 查阅 \ref README.md "README" 文档,了解如何构建和安装本开发包.
 *  
 * # ChangeLog
 *  查阅 \ref doc/changelog.md.
 *    
 * # Python
 * 查看 \ref doc/python.md 来获取使用python开发相关的内容.
 *
 * # .net
 * 查看 \ref doc/dotnet.md 来获取在.net中使用C\#开发相关的内容.
 *
 * # Develop
 * 如果需要开发sdk，添加新的功能，可以参考 \ref doc/develop.md.
 * 
 * # FAQ
 * 
 * ## 机械臂界面上坐标系统中，Rz、Ry、Rx是如何定义的。
 *
 * 乐白机械臂界面的Rz,Rz,Rz表示机械臂姿态，是由EulerZYX确定的。
 * 
 * 界面显示的Rz，Ry，Rx为角度，将其转换为弧度后，它和旋转矩阵的关系如下：
 * 
 * \f$RotationMatrix = \begin{bmatrix}
 * \cos(Rz) & -sin(Rz) & 0 \\ 
 * sin(Rz) & cos(Rz) & 0\\ 
 * 0 & 0 & 1
 * \end{bmatrix}\begin{bmatrix}
 * cos(Ry) & 0 & sin(Ry)\\ 
 * 0 & 1 & 0 \\ 
 * -sin(Ry) & 0 & cos(Ry)
 * \end{bmatrix}\begin{bmatrix}
 * 1 & 0 & 0 \\ 
 * 0 & cos(Rx) & -sin(Rx) \\ 
 * 0 & sin(Rx)  & cos(Rx) 
 * \end{bmatrix}\f$
 */

