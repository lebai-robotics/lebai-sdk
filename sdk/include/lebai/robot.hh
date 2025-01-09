/**
 * Copyright 2022 lebai.ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <map>

namespace lebai {
namespace l_master {
/**
 * @brief 获取当前SDK版本号
 *
 * @return 返回版本号字符串
 */
std::string version();
/**
 * @brief CartesianPose是用来表示空间位姿的数据结构.
 * 数据为字典数据，其中应当包括键为x,y,z,rz,ry,rx的数据.
 *
 */
using CartesianPose = std::map<std::string, double>;
using DoubleVector = std::vector<double>;
/**
 * @brief 运动学正解的返回值数据结构.
 *
 */
struct KinematicsForwardResp {
  CartesianPose
      pose;        /*!<
                      笛卡尔坐标位置，map数据，应当包括'x','y','z','rx','ry','rz'的键值.
                    */
  bool ok = false; /*!< 计算是否成功. */
};
/**
 * @brief 运动学逆解的返回值数据结构
 *
 */
struct KinematicsInverseResp {
  DoubleVector
      joint_positions; /*!<
                          机械臂关节位置的map数据，应当包括'j1','j2','j3','j4','j5','j6'六个关节的角度值.
                        */
  bool ok = false;     /*!< 计算是否成功 */
};
/***
 * @brief 夹爪数据结构.
 *
 */
struct ClawData {
  double force;     /*!< 夹爪的力矩值. */
  double amplitude; /*!< 夹爪的开合角度值. */
  bool hold_on;     /*!< 夹爪是否夹住. */
};

/**
 *  @brief 机械臂的主要接口对象，通过本对象的方法与机械臂进行数据交互.
 *  @note 使用该数据结构和机械臂交互要求机械臂软件版本>=3.1.5。
 *  @note 接口的每一个函数都可能抛出异常std::runtime_error,
 * 这是因为网络连接丢失或者调用的逻辑错误等。
 *
 *
 */
class Robot {
 public:
  /**
   * @brief 内部实现.
   * @note 用户无需使用.
   *
   */
  class RobotImpl;

  /**
   * @brief 构造Robot对象.
   * @note
   * 当你尝试创建Robot对象时，会根据传入的ip参数尝试去连接机械臂，但是可能会连接失败，当连接不成功时，对象依然会创建。
   *
   *
   * @param ip: 机械臂IP地址.
   * @param simulator:
   * 用于表示机械臂是否为仿真机械臂(docker仿真或控制器运行在仿真模式下)的macs标志.True表示仿真模式，False表示实物机械臂.
   */
  explicit Robot(std::string ip, bool simulator = false);
  /**
   * @brief 析构Robot对象.
   *
   */
  virtual ~Robot();

  // clang-format off
  /**
   * 示例代码:
   *
   *     std::string movej_req = "{\"param\":{\"v\":0.1},\"pose\":{\"joint\":{\"delta\":"{\"joint\":[-1.0,0.0,0.0,0.0,0.0,0.0]}}}}";
   *     resp = robot.call("movej", movej_req);
   *     std::cout << "resp: " << std::get<0>(resp) << ", " << std::get<1>(resp) << std::endl;
   *
   *
   * @brief 用JSON格式字符串调用机械臂的接口.
   * @note SDK采用JSONRPC协议和机械臂控制进行通讯.
   * 您可以自行生成JSON数据，并通过本接口调用相应的JSONRPC，并获取返回的结果和JSON数据.
   * @param[in] method 方法名字
   * @param[in] params 请求JSONRPC的JSON格式字符串.
   * @return 返回一个元组，第一个元素是返回码，第二个元素是数据字符串.
   * 如果返回码为0，表示调用成功，第二个元素是JSONRPC的返回数据.
   * 如果返回码为非0，表示调用失败，第二个元素是错误信息.
   */
  // clang-format on
  std::tuple<int, std::string> call(const std::string &method,
                                    const std::string &params);

  /**
   * @brief
   * 返回是否和机械臂的网络连接正常，如果网络连接异常，调用和机械臂交互的接口会抛出异常std::runtime_error。
   * @note 不建议使用，直接catch接口调用获取网络异常。
   * @return true 表示网络连接正常，false表示网络连接异常.
   */
  bool is_network_connected();

  /** \defgroup STARTSTOP 启动停止.
   *  \brief    启动停止相关的接口.
   */

  /** \defgroup CONFIG 机器人配置.
   *  \brief    机器人配置参数相关的接口.
   */

  /** \defgroup MOTION 机械臂运动.
   *  \brief    机械臂运动相关的接口.
   */

  /** \defgroup STATUS 机械臂状态.
   *  \brief    机械臂状态相关的接口.
   */

  /** \addtogroup IO 通用输入输出
   *  \brief      通用输入输出相关的接口
   */

  /** \defgroup CLAW 夹爪.
   *  \brief    夹爪相关的接口.
   */

  /** \defgroup LED 灯板.
   *  \brief    灯板相关的接口.
   */

  /** \defgroup SIGNAL 信号量.
   *  \brief    信号量相关的接口.
   */

  /** \defgroup PROGRAM 程序控制.
   *  \brief    程序控制相关的接口.
   */

  /** \defgroup ROBOTICS 机器人.
   *  \brief    机器人计算相关的接口.
   */

  /** \defgroup FILE 文件系统.
   *  \brief    文件系统相关的接口.
   */

  /** \defgroup MODBUS modbus.
   *  \brief    modbus相关的接口.
   */

  /** \defgroup SCENE 场景.
   *  \brief    场景相关的接口.
   */

  /** \defgroup SERIAL serial.
   *  \brief    串口相关的接口.
   */

  /** \addtogroup STARTSTOP
   *  @{
   */

  /**
   * @brief 启动机械臂（机械臂上使能）.
   *
   */
  void start_sys();
  /**
   * @brief 停止机械臂（机械臂下使能）.
   *
   */

  void stop_sys();
  /**
   * @brief 关闭机器人电源（关机）.
   *
   */
  void powerdown();
  /**
   * @brief 停止运动（但不下电）.
   *
   */
  void stop();
  /**
   * @brief 紧急停止（急停）.
   *
   */
  void estop();
  /**
   * @brief 进入示教模式.
   *
   */
  void teach_mode();
  /**
   * @brief 退出示教模式.
   *
   */
  void end_teach_mode();
  /**
   * @brief 暂停运动.
   *
   */
  void pause();
  /**
   * @brief 恢复运动.
   *
   */
  void resume();
  /**
   * @brief 重新启动机箱
   */
  void reboot();
  /** @}*/

  /** \addtogroup MOTION
   *  @{
   */

  /**
   * 示例代码:
   *
   *     std::vector<double> joint_positions = {0.0, -60.0 / 180.0 * M_PI, 80.0
   * / 180.0 * M_PI, -10.0 / 180.0 * M_PI, -60.0 / 180.0 * M_PI, 0.0};
   *     robot.movej(joint_positions, 3.0, 1.0, 0.0, 0.0);
   *
   *
   * @brief   通过关节位置发送机械臂关节移动
   * @note
   * 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束.
   * @param[in] joint_positions 目标位置的关节数据,为关节的角度值构成的数组.
   * @param[in] a 加速度
   * @param[in] v 速度
   * @param[in] t
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r 交融半径，设置为0，则无交融半径.
   * @return  >0 发送成功
   * @return  <=0 发送失败
   *
   */
  int movej(const std::vector<double> &joint_positions, double a, double v,
            double t, double r);
  /**
   * 示例代码:
   *
   *     robot.movej({”x",-0.296},{"y",-0.295},{"z",0.285},{"rz",60.0 / 180.0 *
   * M_PI},{"ry",-5.0 / 180.0 * M_PI},{"rx", 81.0 / 180.0 * M_PI}}, 3.0, 1.0,
   * 0.0, 0.0);
   *
   *
   * @brief 通过坐标位置发送机械臂关节移动
   * @note
   * 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束.
   * @param[in] cart_pose
   * 目标位置在机器人基座标系下的坐标数据(目前不支持在其它坐标系下的坐标数据)，CartesianPose
   * = std::map<std::string,double>，应当包括键为x,y,z,rz,ry,rx的值.
   * @param[in] a 加速度.
   * @param[in] v 速度.
   * @param[in] t
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r 交融半径，设置为0，则无交融半径.
   * @return >0 发送成功
   * @return <=0 发送失败
   */
  int movej(const CartesianPose &cart_pose, double a, double v, double t,
            double r);
  /**
   * 示例代码:
   *
   *     std::vector<double> joint_positions = {0.0, -60.0 / 180.0 * M_PI, 80.0
   * / 180.0 * M_PI, -10.0 / 180.0 * M_PI, -60.0 / 180.0 * M_PI, 0.0};
   *     robot.movel(joint_positions, 3.0, 1.0, 0.0, 0.0);
   *
   *
   * @brief 通过关节位置发送机械臂直线移动
   * @note
   * 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束.
   * @param[in] joint_positions: 目标位置的关节数据,为关节的角度值构成的数组.
   * @param[in] a 加速度.
   * @param[in] v 速度.
   * @param[in] t
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r 交融半径，设置为0，则无交融半径.
   * @return >0 发送成功
   * @return <=0 发送失败
   */
  int movel(const std::vector<double> &joint_positions, double a, double v,
            double t, double r);
  /**
   * 示例代码:
   *
   *     robot.movel({”x",-0.296},{"y",-0.295},{"z",0.285},{"rz",60.0 / 180.0 *
   * M_PI},{"ry",-5.0 / 180.0 * M_PI},{"rx", 81.0 / 180.0 * M_PI}}, 3.0, 1.0,
   * 0.0, 0.0);
   *
   *
   * @brief 通过坐标位置发送机械臂直线移动
   * @note
   * 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束.
   * @param cart_pose
   * 目标位置在机器人基座标系下的坐标数据(目前不支持在其它坐标系下的坐标数据)，CartesianPose
   * = std::map<std::string,double>，应当包括键为x,y,z,rz,ry,rx的值.
   * @param a 加速度.
   * @param v 速度.
   * @param t
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param r 交融半径，设置为0，则无交融半径.
   * @return >0 发送成功.
   * @return <=0 发送失败.
   */
  int movel(const CartesianPose &cart_pose, double a, double v, double t,
            double r);
  /**
   * 示例代码
   *
   *     robot.movec({3.0/ 180.0 * M_PI, -48.0/ 180.0 * M_PI, 78.0/ 180.0 *
   * M_PI, 9.0/ 180.0 * M_PI, -67.0/ 180.0 * M_PI, -3.0/ 180.0 * M_PI},
   *     {-28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/
   * 180.0 * M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI}, 0.0, 1.0, 0.5, 0.0,
   * 0.0);
   *
   * @brief 通过关节位置发送机械臂圆弧运动.
   * @param[in] joint_via
   * 圆弧上途径点关节位置，为关节的角度值构成的数组.为圆上三点中的一点.
   * @param[in] joint
   * 圆弧目标点关节位置，为关节的角度值构成的数组.如果编程rad不为零，则为圆上三点中的一点.
   * @param[in] rad
   * 圆弧角度值，单位为弧度，如果为零，则走到目标点，正负值用来确定圆弧方向.
   * @param[in] a 加速度.
   * @param[in] v 速度.
   * @param[in] t:
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r: 交融半径，设置为0，则无交融半径.
   * @return >0 发送成功.
   * @return <=0 发送失败.
   */
  int movec(const std::vector<double> &joint_via,
            const std::vector<double> &joint, double rad, double a, double v,
            double t, double r);
  /**
   *
   * @brief 通过坐标位置发送机械臂圆弧运动
   * @param[in] cart_via
   * 圆弧上途径点坐标位置，应当包括键为x,y,z,rz,ry,rx的值.为圆上三点中的一点.
   * @param[in] cart
   * 圆弧目标点坐标位置，应当包括键为x,y,z,rz,ry,rx的值.如果编程rad不为零，则为圆上三点中的一点.
   * @param[in] rad
   * 圆弧角度值，单位为弧度，如果为零，则走到目标点，正负值用来确定圆弧方向.
   * @param[in] a 加速度.
   * @param[in] v 速度.
   * @param[in] t:
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r: 交融半径，设置为0，则无交融半径.
   * @return >0 发送成功.
   * @return <=0 发送失败.
   */
  int movec(const CartesianPose &cart_via, const CartesianPose &cart,
            double rad, double a, double v, double t, double r);
  /**
   * 示例代码:
   *
   *     robot.speedj( 1.0, {0.5,0.0,0.0,0.0,0.0,0.0}, 0.0);
   *
   * @brief 通过关节速度矢量发送机械臂关节匀速运动
   * @param[in] a 加速度.
   * @param[in] v 速度矢量
   * @param[in] t: 运动时间，默认t = 0，一直运动到限位.
   * @return  >0 发送成功.返回运动号
   * @return  <=0 发送失败.
   */
  int speedj(double a, const std::vector<double> &v, double t = 0.0);
  /**
   *
   * 示例代码:
   *
   *     robot.speedl( 1.0, {{"x", 0.0}, {"y", 0.0}, {"z", 0.1}, {"rx", 0.0},
   * {"ry", 0.0}, {"rz", 0.0}}, 0.0);
   *
   * @brief 通过坐标速度矢量发送机械臂关节匀速运动
   * @param[in] a 加速度.
   * @param[in] v 速度矢量
   * @param[in] t: 运动时间，默认t = 0，一直运动到限位.
   * @param[in] reference: 参考坐标系，默认为零.
   * @return  >0 发送成功.返回运动号
   * @return  <=0 发送失败.
   */
  int speedl(double a, const CartesianPose &v, double t = 0.0,
             const CartesianPose &reference = {{"x", 0.0},
                                               {"y", 0.0},
                                               {"z", 0.0},
                                               {"rx", 0.0},
                                               {"ry", 0.0},
                                               {"rz", 0.0}});
  /**
   * 示例代码:
   *
   *     std::vector<double> joint_positions = {0.0, -60.0 / 180.0 * M_PI, 80.0
   * / 180.0 * M_PI, -10.0 / 180.0 * M_PI, -60.0 / 180.0 * M_PI, 0.0};
   *     robot.toawrdj(joint_positions, 3.0, 1.0, 0.0, 0.0);
   *
   *
   * @brief   通过关节位置发送机械臂关节自由移动.
   * @note
   * 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节自由移动即返回，不会等待运动结束.
   * @param[in] joint_positions: 目标位置的关节数据,为关节的角度值构成的数组.
   * @param[in] a: 加速度.
   * @param[in] v: 速度.
   * @param[in] t:
   * 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数.
   * @param[in] r: 交融半径，设置为0，则无交融半径.
   * @return  >0 发送成功.返回运动号
   * @return  <=0 发送失败.
   *
   */
  int towardj(const std::vector<double> &joint_positions, double a, double v,
              double t, double r);
  /**
   * @brief 伺服运动PVAT
   *
   * @param p 关节位置，或者坐标位置（将通过运动学反解转为关节位置）.
   * @param v 每个关节的速度 (rad/s)。如该值为数字，则表示所有关节速度相同.
   * @param a 每个关节的加速度 (rad/s2)。如该值为数字，则表示所有关节加速度相同.
   * @param t 运动时间 (s)
   */
  void move_pvat(std::vector<double> p, std::vector<double> v,
                 std::vector<double> a, double t);
  /**
   * @brief 等待运动完成.
   *
   * @param id 指定运动的id(0为等待全部任务).
   */
  void wait_move(unsigned int id);
  /**
   * @brief 等待所有运动完成.
   *
   */
  void wait_move();
  /**
   * @brief 查询当前正在运动的MotionId(无运动时返回上次MotionId).
   */
  unsigned int get_running_motion();
  /**
   * @brief 查询指定MotionId的运动状态.
   *
   * @return id 指定的运动id.
   */
  std::string get_motion_state(unsigned int id);
  /**
   * @brief 停止所有运动.
   */
  void stop_move();
  /** @}*/

  /** \addtogroup STATUS
   *  @{
   */

  /**
   * @brief 获取机器人状态码
   *
   * @return int 返回状态码.
   *
   *  状态码 |      状态	     |       说明
   *  ------ | -------------  | -------------
   *    -1	 |   控制系统故障  |  机器人软件控制系统异常
   *    0	   |   硬件通讯故障  |  机器人硬件通讯故障
   *    1	   |   已急停	       |  机器人处于急停状态，请确认安全性
   *    2	   |   初始化中	     |  机器人初始化中
   *    4	   |   初始化完成	   |  机器人电源已开启
   *    5	   |   空闲	         |  机器人处于空闲状态
   *    6	   |   暂停	         |  机器人处于暂停中状态
   *    7	   |   运行中	       |  机器人运行中
   *    8	   |   更新中	       |  机器人系统更新中
   *    9	   |   启动中	       |  机器人初始化完成到空闲的启动过程中
   *    10	 |   正在停止	     |  机器人空闲状态转到停止状态
   *    11	 |   示教中	       |  机器人处于示教模式中
   *    12	 |   已停止	       |  机器人处于停止状态，非急停状态
   *
   * @note 查看 <a
   * href="https://help.lebai.ltd/guide/basic.html#%E6%9C%BA%E5%99%A8%E4%BA%BA%E7%8A%B6%E6%80%81">具体信息</a>.
   */
  int get_robot_mode();
  /**
   *  @brief 查看急停原因
   *
   *  @return 急停原因
   */
  int get_estop_reason();
  /**
   * @brief 是否已与手臂断开连接
   *
   * @return 是否已断开连接
   */
  bool is_disconnected();
  /**
   * @brief 手臂是否已下电
   *
   * @return 是否已下电
   */
  bool is_down();
  /**
   * @brief 获取机械臂关节当前反馈位置
   *
   * @return 关节反馈位置数组，包括所有关节的角度值.
   */
  std::vector<double> get_actual_joint_positions();
  /**
   * @brief 获取机械臂关节当前控制位置
   *
   * @return 关节控制位置数组，包括所有关节的角度值.
   */
  std::vector<double> get_target_joint_positions();
  /**
   * @brief 获取机械臂关节当前反馈速度
   *
   * @return 关节反馈速度数据
   */
  std::vector<double> get_actual_joint_speed();
  // get_target_joint_speed
  /**
   * @brief 获取机械臂关节当前控制力矩
   *
   * @return 关节控制速度数据
   */
  std::vector<double> get_target_joint_speed();
  /**
   * @brief 获取机械臂末端在机械臂基坐标系下的实际位姿，CartesianPose =
   * std::map<std::string,double>，应当包括键为x,y,z,rz,ry,rx的值.
   *
   * @return CartesianPose
   */
  CartesianPose get_actual_tcp_pose();
  /**
   * @brief 获取机械臂末端在机械臂基坐标系下的控制位姿，CartesianPose =
   * std::map<std::string,double>，应当包括键为x,y,z,rz,ry,rx的值.
   *
   * @return CartesianPose
   */
  CartesianPose get_target_tcp_pose();

  /**
   * @brief 获取单个关节温度
   *
   * @param joint_index 关节索引
   * @return double 关节当前温度
   */
  double get_joint_temp(unsigned int joint_index);

  /**
   * @brief 获取机械臂关节当前反馈力矩
   *
   * @return 关节当前反馈力矩
   */
  std::vector<double> get_actual_joint_torques();
  /**
   * @brief 获取机械臂关节当前控制力矩
   *
   * @return 关节当前控制力矩
   */
  std::vector<double> get_target_joint_torques();

  /** @}*/
  /** \addtogroup IO
   *  @{
   */
  /**
   * @brief 设置数字输出
   * @anchor DEVICENAME
   * @param device 设备名字，
   *   ID    |      设备名字	 |       说明
   *  ------ | -------------  | -------------
   *    0	   |   ROBOT         |  机箱IO
   *    1	   |   FLANGE	       |  法兰IO
   *    2	   |   EXTRA	       |  拓展IO
   *    11	 |   SHOULDER	     |  肩部按钮DI
   *    12	 |   FLANGE_BTN	   |  法兰按钮DI
   *
   * 查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   *
   * @param pin 端口，从 0 开始
   * @param value 待设置的值
   */
  void set_do(std::string device, unsigned int pin, unsigned int value);
  /**
   * @brief 获取数字输出
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 端口，从 0 开始
   * @return 返回数字输出数值
   */
  unsigned int get_do(std::string device, unsigned int pin);
  /**
   * @brief 获取多个数字输出
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 起始数字输出端口，从 0 开始
   * @param num 连续的数字输出个数
   * @return 返回多个数字输出数值
   */
  std::vector<unsigned int> get_dos(std::string device, unsigned int pin,
                                    unsigned int num);
  /**
   * @brief 获取数字输入
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 端口，从 0 开始
   * @return 返回输入数值
   */
  unsigned int get_di(std::string device, unsigned int pin);
  /**
   * @brief 获取多个数字输入
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 起始数字输入端口，从 0 开始
   * @param num 连续的数字输入个数
   * @return 返回多个数字输入
   */
  std::vector<unsigned int> get_dis(std::string device, unsigned int pin,
                                    unsigned int num);

  /**
   * @brief 设置模拟输出
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin: 模拟输出端口，从 0 开始
   * @param value: 待设置的模拟输出值
   */
  void set_ao(std::string device, unsigned int pin, double value);
  /**
   * @brief 获取模拟输出
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin: 端口，从 0 开始
   * @return 返回模拟输入数值
   */
  double get_ao(std::string device, unsigned int pin);
  /**
   * @brief 获取多个模拟输出
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin: 起始模拟输出端口，从 0 开始
   * @param num 连续的模拟输出个数
   * @return 返回模拟输出数值
   */
  std::vector<double> get_aos(std::string device, unsigned int pin,
                              unsigned int num);
  /**
   * @brief 获取模拟输入
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin: 端口，从 0 开始
   * @return 返回模拟输入数值
   */
  double get_ai(std::string device, unsigned int pin);
  /**
   * @brief 获取多个模拟输入
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin: 起始模拟输入端口，从 0 开始
   * @param num 连续的模拟输入个数
   * @return 返回多个模拟输入数值
   */
  std::vector<double> get_ais(std::string device, unsigned int pin,
                              unsigned int num);
  /**
   * @brief 设置数字端口模式
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 端口号，从 0 开始
   * @param value 设置的值，false为输入模式，true为输出模式
   * @return 返回是否成功
   */
  void set_dio_mode(std::string device, unsigned int pin, bool value);
  /**
   * @brief 获取数字端口模式
   * @param device 设备名字，查看 @ref DEVICENAME ，可以进一步查看 <a
   * href="https://help.lebai.ltd/sdk/io.html#io-%E8%AE%BE%E5%A4%87%E7%B1%BB%E5%9E%8B">详细信息</a>.
   * @param pin 端口号，从 0 开始
   * @param count 查询的连续端口数
   * @return 从pin开始的连续count个端口的当前模式
   */
  std::vector<bool> get_dios_mode(std::string device, unsigned int pin,
                                  unsigned int count);
  /** @}*/

  /** \addtogroup CLAW
   *  @{
   */
  /**
   * @brief
   * 设置夹爪力度（力控）和幅度（位控）.如果在闭合过程中抓取到物体，则不再继续闭合以避免夹坏物体，判断的准则为这里设置的力的大小.
   *
   * @param force 力度（0-100）
   * @param amplitude 张合幅度（0-100）
   */
  void set_claw(double force, double amplitude);
  /**
   * @brief 获取夹爪当前数据
   *
   * @return std::tuple<double, double ,bool>
   * 第一个数据为夹爪力度，第二个数据为幅度，第三个数据为开度是否稳定
   */
  std::tuple<double, double, bool> get_claw();
  /**
   * @brief 获取夹爪当前数据
   *
   * @return ClawData 数据
   */
  ClawData get_claw_data();
  /** @}*/

  /** \addtogroup LED
   *  @{
   */

  /**
   * @brief 设置LED灯状态.
   *
   * @param mode:
   * 亮灯模式.0：不变；1：关闭；2：常亮；3：呼吸；4：均分旋转；5：同色旋转；6：闪烁
   * @param speed: 速度.1：快速；2：正常；3：慢速
   * @param color: 最多包含 4 个 0 ~ 15 之间的整数
   */
  void set_led(unsigned int mode, unsigned int speed,
               const std::vector<unsigned int> &color);
  /**
   * @brief 设置声音
   *
   * @param voice: 声音列表
   * @param volume: 音量.0：静音；1：低；2：正常；3：高
   */
  void set_voice(unsigned int voice, unsigned int volume);
  /**
   * @brief 开关风扇
   *
   * @param status: 状态.1：关闭；2：开启
   */
  void set_fan(unsigned int status);
  /** @}*/

  /** \addtogroup SIGNAL
   *  @{
   */

  /**
   * @brief 设置信号量
   *
   * @param index: 信号量下标（取值范围0~255)
   * @param value: 待设置的信号量(32位有符号整数)
   */
  void set_signal(unsigned int index, int value);
  /**
   * @brief 获取信号量
   *
   * @param index: 信号量下标（取值范围0~255)
   * @return 返回对应的信号量
   */
  int get_signal(unsigned int index);
  /**
   * @brief 增加指定下标的信号量值，该操作是原子的.
   *
   * @param index: 信号量下标（取值范围0~255）
   * @param value: 待增加的信号量值
   */
  void add_signal(unsigned int index, int value);
  /** @}*/

  /** \addtogroup SCENE
   *  @{
   */

  /**
   * @brief 调用场景
   *
   * @param name 调用场景的名字
   * @param params 其他参数
   * @param dir 调用场景所在的文件夹名
   * @param is_parallel 是否并行
   * @param loop_to 循环次数（默认0永久循环）
   *
   *
   */
  unsigned int start_task(const std::string &name,
                          const std::vector<std::string> &params,
                          const std::string &dir, bool is_parallel,
                          unsigned int loop_to);
  /**
   * @brief 调用场景
   *
   * @param name: 调用场景的名字
   */
  unsigned int start_task(const std::string &name);
  /**
   * @brief 查询任务列表
   */
  std::vector<unsigned int> load_task_list();
  /**
   * @brief 等待任务完成
   *
   * @param id: 任务的ID
   * @return 返回任务是否成功
   */
  bool wait_task(unsigned int id);
  /**
   * @brief 暂停任务与运动
   *
   * @param id: 任务的ID
   * @param time: 暂停的时间
   * @param wait: 是否等待
   */
  void pause_task(unsigned int id, unsigned long time, bool wait);
  /**
   * @brief 暂停任务与运动
   * @param id: 任务的ID
   */
  void pause_task(unsigned int id);
  /**
   * @brief 恢复任务与运动
   *
   * @param id 任务的ID
   */
  void resume_task(unsigned int id);
  /**
   * @brief 取消任务与运动.
   *
   * @param id 任务的ID.
   */
  void cancel_task(unsigned int id);
  /**
   * @brief 根据已设置的Hook执行对应场景
   *
   * @param id 任务的ID.
   */
  unsigned int exec_hook(unsigned int id);
  /**
   * @brief 获取任务状态.
   */
  std::string get_task_state();
  /**
   * @brief 获取任务状态.
   *
   * @param id 任务的ID.
   */
  std::string get_task_state(unsigned int id);
  /** @}*/

  /** \addtogroup ROBOTICS
   *  @{
   */
  /**
   * @brief 根据机械臂关节位置计算机器人末端位姿（位置的运动学正解）.
   * @param joint_positions: 机械臂关节位置的数组.
   * @return 返回计算结果 \ref KinematicsForwardResp "KinematicsForwardResp".
   *
   */
  KinematicsForwardResp kinematics_forward(
      const std::vector<double> &joint_positions);

  /**
   * @brief 根据机械臂的末端位姿计算关节位置（位置的运动学逆解）.
   * @param pose: 机械臂末端位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @param joint_init_positions: 机械臂关节初始位置, 以数组形式传入.
   * @return 返回计算结果 \ref KinematicsInverseResp "KinematicsInverseResp".
   */
  KinematicsInverseResp kinematics_inverse(
      const CartesianPose &pose,
      const std::vector<double> &joint_init_positions = {});

  /**
   * @brief 位姿变换乘法（等价于对应的齐次坐标矩阵乘法）
   *
   * @param[in] a: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @param[in] b: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @return CartesianPose 返回的位姿，应当包括键为x,y,z,rz,ry,rx的值.
   */
  CartesianPose pose_times(const CartesianPose &a, const CartesianPose &b);

  /**
   * @brief 位姿变换的逆（等价于对应的齐次坐标矩的逆）
   *
   * @param in: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @return CartesianPose 返回位姿变换的逆，应当包括键为x,y,z,rz,ry,rx的值.
   */
  CartesianPose pose_inverse(const CartesianPose &in);
  /** @}*/

  /** \addtogroup FILE
   * @{
   */
  /**
   * @brief 保存文件（以字节形式）.
   *
   * @param dir: 保存的文件路径.
   * @param name: 保存的文件名.
   * @param is_dir: 要保存的文件是否为文件夹.
   * @param data: 文件字节.
   */
  void save_file(const std::string &dir, const std::string &name, bool is_dir,
                 const std::string &data);
  /**
   * @brief 重命名文件
   *
   * @param from_dir: 源文件所在的文件夹.
   * @param from_name: 源文件名称.
   * @param to_dir: 目标文件文件夹.
   * @param to_name: 目标文件文件名.
   */
  void rename_file(const std::string &from_dir, const std::string &from_name,
                   const std::string &to_dir, const std::string &to_name);

  /**
   * @brief 查询文件
   *
   * @param dir: 文件的目录
   * @param name: 文件名
   *
   * @return 文件的具体内容
   */
  std::tuple<bool, std::string> load_file(const std::string &dir,
                                          const std::string &name);

  /**
   * @brief 查询文件列表.
   *
   * @param dir: 文件的目录.
   * @param prefix: 前缀.
   * @param suffix: 后缀.
   *
   * @return 文件列表.
   */
  std::vector<std::tuple<bool, std::string>> load_file_list(
      const std::string &dir, const std::string &prefix,
      const std::string &suffix);
  /**
   * @brief 将文件从文件系统中压缩到zip文件.
   *
   * @param from_dir 源文件的目录.
   * @param files 源文件的文件名.
   * @param to_dir  压缩后文件的路径.
   * @param name 压缩后文件的名称.
   */
  // void zip(const std::string &from_dir, std::vector<std::string> files, const
  // std::string &to_dir, const std::string &name);
  // /**
  //  *  @brief 将zip文件解压到文件系统.
  //  *
  //  * @param from_dir zip文件的路径.
  //  * @param name zip文件的名称.
  //  * @param files zip文件内的文件名.
  //  * @param to_dir  解压到的路径.
  //  */
  // void unzip(const std::string &from_dir, const std::string &name,
  // std::vector<std::string> files, const std::string &to_dir);
  // /**
  //  * @brief 查询文件列表.
  //  *
  //  * @brief 目标zip文件名.
  //  * @param dir 文件的目录.
  //  * @param prefix 前缀.
  //  * @param suffix 后缀.
  //  *
  //  * @return 文件列表.
  //  */
  // //std::vector<std::tuple<bool,string>> load_zip_list(const std::string
  // &zip,const std::string &dir,const std::string &prefix,const std::string
  // &suffix);

  /** @}*/

  /** \addtogroup CONFIG
   *  @{
   */
  /**
   *  @brief 设置工具中心点(TCP)坐标，坐标值相对于工具坐标系.
   *
   *  @param tcp 参数为六元组，表示一个空间位置变换.
   */
  void set_tcp(std::array<double, 6> tcp);
  /**
   *  @brief 获取当前机器人工具中心点设置.
   *
   *  @return 当前机器人的工具中心点参数，为六元组.
   */
  std::array<double, 6> get_tcp();
  /**
   *  @brief 设置速度因子.
   *
   *  @param factor 速度因子百分比，范围0-100.
   */
  void set_velocity_factor(int factor);
  /**
   *  @brief 获取当前的速度因子.
   *
   *  @return 速度因子百分比.
   */
  int get_velocity_factor();
  /**
   *  @brief 设置机器人末端负载.
   *
   *  @param mass 末端负载的质量(kg).
   *  @param cog 质心相对于TCP坐标系的偏移.
   */
  void set_payload(double mass, std::map<std::string, double> cog);
  /**
   *  @brief 设置机器人末端负载质量.
   *
   *  @param mass 末端负载的质量(kg).
   */
  void set_payload_mass(double mass);
  /**
   *  @brief 设置机器人末端负载重心.
   *
   *  @param cog 质心相对于TCP坐标系的偏移.
   */
  void set_payload_cog(std::map<std::string, double> cog);
  /**
   *  @brief 获取末端负载设置.
   *
   *  @return 由负载质量mass和负载偏移组成的元组.
   */
  std::map<std::string, double> get_payload();
  /**
   *  @brief 设置机器人重力加速度方向.
   *
   *  @param gravity 相对于机器人基座标的重力方向.
   */
  void set_gravity(std::map<std::string, double> gravity);
  /**
   *  @brief 获取机器人重力加速度的方向.
   *
   *  @return 相对于机器人基座标的重力方向.
   */
  std::map<std::string, double> get_gravity();
  /**
   * @brief 从资源库加载tcp.
   *
   * @param name 点位名称.
   * @param dir 点位目录.
   */
  CartesianPose load_tcp(std::string name, std::string dir = "");
  /** @}*/

  /** \addtogroup MODBUS
   *  @{
   */

  /**
   * @brief 写单个线圈.
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param value 待设置的值.
   */
  void write_single_coil(std::string device, std::string addr, bool value);

  /**
   * @brief 写多个线圈
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param values 待设置的值.
   */
  void wirte_multiple_coils(std::string device, std::string addr,
                            std::vector<bool> values);

  /**
   * @brief 读线圈
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param num 连续数量.
   */
  std::vector<bool> read_coils(std::string device, std::string addr,
                               unsigned int num);
  /**
   * @brief 读离散输入
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param num 连续数量.
   */
  std::vector<bool> read_discrete_inputs(std::string device, std::string addr,
                                         unsigned int num);

  /**
   * @brief 写单个寄存器
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param value 待设置的值.
   */
  void write_single_register(std::string device, std::string addr,
                             unsigned int value);
  /**
   * @brief 写多个寄存器
   *
   * @param device 设备名称.
   * @param addr 寄存器地址.
   * @param values 待设置的值.
   */
  void write_multiple_registers(std::string device, std::string addr,
                                std::vector<unsigned int> values);

  /**
   * @brief 读保持寄存器
   *
   * @param device 设备名称
   * @param addr 寄存器地址
   * @param num 连续数量
   */
  std::vector<unsigned int> read_holding_registers(std::string device,
                                                   std::string addr,
                                                   unsigned int num);
  /**
   * @brief 读输入寄存器
   *
   * @param device 设备名称
   * @param addr 寄存器地址
   * @param num 连续数量
   */
  std::vector<unsigned int> read_input_registers(std::string device,
                                                 std::string addr,
                                                 unsigned int num);
  /** @}*/

  /** \addtogroup SERIAL
   *  @{
   */

  /**
   * @brief 设置串口波特率.
   *
   * @param device 设备名称.
   * @param baud_rate 波特率.
   */
  void set_serial_baud_rate(std::string device, unsigned int baud_rate);
  /**
   * @brief 设置串口校验位.
   *
   * @param device 设备名称.
   * @param parity 校验位. 0:无校验; 1:奇校验; 2:偶校验.
   *
   */
  void set_serial_parity(std::string device, unsigned int parity);
  /** @}*/
 protected:
  std::unique_ptr<RobotImpl> impl_; /*!< 内部实现数据结构，用户无需关注. */
};

}  // namespace l_master

}  // namespace lebai
