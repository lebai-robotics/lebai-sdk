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
#include <cstdint>
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

/**
 * @brief 夹爪数据结构.
 *
 */
struct ClawData {
  double force;     /*!< 夹爪的力矩值. */
  double amplitude; /*!< 夹爪的开合角度值. */
  bool hold_on;     /*!< 夹爪是否夹住. */
};

/**
 * @brief 存储项数据结构.
 *
 */
struct StorageItem {
  std::string key;   /*!< 存储项名称. */
  std::string value; /*!< 存储项值. */
};

/**
 * @brief 机械臂物理数据结构.
 *
 */
struct PhysicalData {
  DoubleVector joint_temperature; /*!< 关节温度. */
  DoubleVector joint_voltage;     /*!< 关节电压. */
  double flange_voltage;          /*!< 法兰电压. */
};

struct DhParamData {
  double a = 0.0;     /*!< DH参数a. */
  double alpha = 0.0; /*!< DH参数alpha. */
  double d = 0.0;     /*!< DH参数d. */
  double theta = 0.0; /*!< DH参数theta. */
};

/**
 * @brief 控制器系统信息数据结构.
 *
 */
struct SystemInfoData {
  std::string name;            /*!< 操作系统名字. */
  std::string kernel_version;  /*!< 内核版本. */
  std::string os_version;      /*!< 系统版本. */
  std::string host_name;       /*!< 主机名. */
  uint64_t used_memory = 0;    /*!< 已使用内存. */
  uint64_t total_memory = 0;   /*!< 总内存. */
};

/**
 * @brief 机器人设备信息.
 *
 */
struct RobotInfoData {
  std::string name;      /*!< 设备名称. */
  std::string mac;       /*!< 设备物理唯一标识. */
  std::string box_model; /*!< 控制箱型号. */
  std::string box_sn;    /*!< 控制箱序列号. */
  std::string arm_model; /*!< 手臂型号. */
  std::string arm_sn;    /*!< 手臂序列号. */
};

struct DiscoveredRobotData {
  std::string name; /*!< 设备名称. */
  std::string mac;  /*!< 设备物理唯一标识. */
  std::string ip;   /*!< 设备IP地址. */
  bool online = false; /*!< 设备是否在线. */
};

/**
 * @brief 控制器硬件设备信息.
 *
 */
struct DeviceInfoData {
  bool invalid = false;      /*!< 设备信息是否无效. */
  std::string sn;            /*!< 序列号. */
  std::string version;       /*!< 固件版本. */
  std::string partition;     /*!< 固件分区. */
  uint32_t di_num = 0;       /*!< DI数量. */
  uint32_t do_num = 0;       /*!< DO数量. */
  uint32_t dio_num = 0;      /*!< DIO数量. */
  uint32_t ai_num = 0;       /*!< AI数量. */
  uint32_t ao_num = 0;       /*!< AO数量. */
};

struct HardwareInfoData {
  DeviceInfoData comboard;             /*!< 通信板信息. */
  std::vector<DeviceInfoData> joints;  /*!< 关节信息. */
  DeviceInfoData flange;               /*!< 法兰信息. */
  DeviceInfoData led;                  /*!< 灯板信息. */
  DeviceInfoData extra_io;             /*!< 扩展IO信息. */
};

struct SoftwareItemInfoData {
  std::string version; /*!< 软件版本. */
  std::string branch;  /*!< 软件分支. */
};

struct SoftwareInfoData {
  std::map<std::string, SoftwareItemInfoData> software; /*!< 软件信息. */
};

struct BackupOptionsData {
  bool tmp = false;
  bool syslog = false;
  bool arm = false;
  bool config = false;
  bool data = false;
  bool file = false;
  bool docker = false;
  bool ds = false;
};

struct BackupInfoData {
  SystemInfoData system;
  RobotInfoData robot;
  HardwareInfoData hardware;
  SoftwareInfoData software;
  BackupOptionsData option;
};

struct HttpRequestData {
  std::string method;                         /*!< HTTP方法. */
  std::string url;                            /*!< 请求URL. */
  std::map<std::string, std::string> headers; /*!< 请求头. */
  std::string body;                           /*!< 请求体. */
};

struct HttpResponseData {
  unsigned int status = 0;                    /*!< HTTP状态码. */
  std::map<std::string, std::string> headers; /*!< 响应头. */
  std::string body;                           /*!< 响应体. */
};

struct MessageData {
  std::string level;  /*!< 消息等级. */
  std::string kind;   /*!< 消息类型. */
  std::string detail; /*!< 详细信息. */
  std::string time;   /*!< 消息产生时间. */
};

struct OtaStateData {
  std::string step;     /*!< 升级步骤. */
  uint32_t progress = 0; /*!< 升级进度. */
};

struct CheckUpgradeData {
  bool need_upgrade = false;  /*!< 是否需要升级. */
  std::string introduction;   /*!< 升级介绍. */
};

struct CommandStdoutData {
  bool done = false;        /*!< 命令是否完成. */
  std::string stdout_text;  /*!< 标准输出. */
  std::string stderr_text;  /*!< 标准错误. */
  int code = 0;             /*!< 返回码. */
};

struct DirData {
  std::string name; /*!< 目录名. */
  uint32_t id = 0;  /*!< 目录内部ID. */
};

struct ShortcutData {
  uint32_t id = 0;  /*!< 快捷方式ID. */
  std::string dir;  /*!< 目标目录. */
  std::string name; /*!< 目标名称. */
};

struct ButtonIndexData {
  std::string device; /*!< 按键设备. */
  uint32_t pin = 0;   /*!< 按键编号. */
};

struct ButtonStatusData {
  std::string state; /*!< 按键状态. */
  uint32_t time = 0; /*!< 持续时间. */
};

struct TriggerConditionData {
  std::vector<ButtonIndexData> pressed; /*!< 同时按下的按键. */
  ButtonIndexData button;               /*!< 条件按键. */
  ButtonStatusData status;              /*!< 条件状态. */
};

struct TriggerData {
  TriggerConditionData condition; /*!< 触发条件. */
  std::string function;           /*!< 触发功能. */
};

struct LedStyleData {
  std::string mode;                /*!< LED工作模式. */
  std::string speed;               /*!< LED运行速度. */
  std::vector<std::string> colors; /*!< LED颜色. */
  std::string voice;               /*!< 语音ID. */
  std::string volume;              /*!< 语音音量. */
};

struct ServoParamData {
  double position_kp = 0.0;       /*!< 位置环KP. */
  double speed_kp = 0.0;          /*!< 速度环KP. */
  double speed_it = 0.0;          /*!< 速度环IT. */
  double torque_cmd_filter = 0.0; /*!< 力矩命令滤波. */
};

struct ExtraServoParamData {
  double acc_position_kp = 0.0;
  double acc_speed_kp = 0.0;
  double acc_speed_it = 0.0;
  double uni_position_kp = 0.0;
  double uni_speed_kp = 0.0;
  double uni_speed_it = 0.0;
  double dec_position_kp = 0.0;
  double dec_speed_kp = 0.0;
  double dec_speed_it = 0.0;
};

struct WrenchData {
  DoubleVector force;  /*!< 受力大小[x,y,z]. */
  DoubleVector torque; /*!< 扭矩大小[x,y,z]. */
};

struct PluginInfoData {
  std::string name;        /*!< 插件名称. */
  std::string description; /*!< 插件描述. */
  std::string homepage;    /*!< 项目主页. */
  bool auto_restart = false; /*!< daemon异常退出后是否自动启动. */
  bool web = false;          /*!< 是否具备Web功能. */
  bool daemon = false;       /*!< 是否具备Daemon功能. */
  bool cmd = false;          /*!< 是否具备Cmd功能. */
  bool enable = false;       /*!< 是否启用. */
};

struct PluginStoreInfoData {
  std::string name; /*!< 插件名称. */
  std::string url;  /*!< 插件下载地址. */
};

struct TaskData {
  unsigned int id = 0;         /*!< 任务ID. */
  std::string state;           /*!< 任务状态. */
  unsigned int loop_count = 0; /*!< 已完成循环次数. */
  unsigned int loop_to = 0;    /*!< 目标循环次数. */
  bool is_parallel = false;    /*!< 是否并行运行. */
  bool is_simu = false;        /*!< 是否为仿真模式. */
  std::string stdout_text;     /*!< 任务输出. */
  std::string kind;            /*!< 任务类型. */
  std::string dir;             /*!< 任务目录. */
  std::string name;            /*!< 任务名称. */
  std::vector<std::string> params; /*!< 任务参数. */
};

struct TaskStdoutData {
  unsigned int id = 0;     /*!< 任务ID. */
  bool done = false;       /*!< 任务是否完成. */
  std::string stdout_text; /*!< 标准输出. */
};

struct ModbusData {
  std::string kind;    /*!< Modbus设备类型. */
  std::string address; /*!< IP地址或串口地址. */
  unsigned int port = 0;     /*!< TCP端口号. */
  unsigned int slave_id = 0; /*!< Modbus从机地址. */
};

struct ModbusRegisterData {
  std::string kind;        /*!< Modbus寄存器类型. */
  unsigned int address = 0; /*!< Modbus内存地址. */
};

struct StructureData {
  bool active = false;      /*!< 是否激活. */
  std::string name;         /*!< 结构名称. */
  std::string kind;         /*!< 结构型号. */
  unsigned int dof = 0;     /*!< 自由度. */
  std::string dh;           /*!< DH参数资源名. */
  std::string dyn;          /*!< 动力学参数资源名. */
  std::string servo;        /*!< 伺服参数资源名. */
};

struct PoseData {
  std::string kind;                 /*!< 位姿类型. */
  CartesianPose cart;               /*!< 笛卡尔位姿. */
  std::vector<double> joint;        /*!< 关节位姿. */
};

struct JointMoveData {
  double pose = 0.0;     /*!< 关节位置. */
  double velocity = 0.0; /*!< 关节速度. */
  double acc = 0.0;      /*!< 关节加速度. */
};

struct PvatPointData {
  double duration = 0.0;            /*!< 执行时间. */
  std::vector<JointMoveData> joints; /*!< 关节运动数据. */
};

struct TrajectoryData {
  std::string kind;                /*!< 轨迹类型. */
  std::vector<PvatPointData> data; /*!< 轨迹数据. */
};

struct FrameData {
  std::string position_kind; /*!< 位置参考系类型. */
  CartesianPose position;    /*!< 自定义位置，使用x/y/z键. */
  std::string rotation_kind; /*!< 姿态参考系类型. */
  CartesianPose rotation;    /*!< 自定义姿态，使用rx/ry/rz键. */
};

/**
 * @brief 机械臂关节运动数据结构.
 *
 */
struct JointMotionData {
  DoubleVector actual_joint_pose;   /*!< 关节实际位置. */
  DoubleVector actual_joint_speed;  /*!< 关节实际速度. */
  DoubleVector actual_joint_acc;    /*!< 关节实际加速度. */
  DoubleVector actual_joint_torque; /*!< 关节实际力矩. */
  DoubleVector target_joint_pose;   /*!< 关节目标位置. */
  DoubleVector target_joint_speed;  /*!< 关节目标速度. */
  DoubleVector target_joint_acc;    /*!< 关节目标加速度. */
  DoubleVector target_joint_torque; /*!< 关节目标力矩. */
  CartesianPose actual_tcp_pose;    /*!< 法兰实际位置. */
  CartesianPose target_tcp_pose;    /*!< 法兰目标位置. */
  CartesianPose actual_flange_pose; /*!< 法兰实际位置. */
};

/**
 * @brief 碰撞检测配置.
 *
 */
struct CollisionDetectorConfig {
  unsigned int action = 99; /*!< 0:急停; 1:暂停; 2:停止运动; 99:关闭碰撞检测 */
  unsigned int pause_time = 0;  /*!< action为1时的暂停时间，单位秒 */
  unsigned int sensitivity = 0; /*!< 灵敏度[0-100] */
};

/**
 * @brief 关节限位配置.
 *
 */
struct JointLimitConfig {
  double min_position = 0.0; /*!< 关节最小位置 */
  double max_position = 0.0; /*!< 关节最大位置 */
  double max_a = 0.0;        /*!< 关节最大加速度 */
  double max_v = 0.0;        /*!< 关节最大速度 */
};

/**
 * @brief 笛卡尔空间限位配置.
 *
 */
struct CartesianLimitConfig {
  double max_a = 0.0;     /*!< 笛卡尔空间最大加速度 */
  double max_v = 0.0;     /*!< 笛卡尔空间最大速度 */
  double eq_radius = 0.0; /*!< 等效半径 */
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
   * @brief 测试 JSON-RPC 连接.
   */
  std::string hello(const std::string &data);

  /**
   * @brief 设置自动配置项，name 取值与 SDK2 保持一致: 1, 2, 3.
   */
  void set_auto(int name, bool value);

  /**
   * @brief 获取自动配置项，name 取值与 SDK2 保持一致: 1, 2, 3.
   */
  bool get_auto(int name);

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
  void start_teach_mode();
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
  void pause_move();
  void pause();
  /**
   * @brief 恢复运动.
   *
   */
  void resume_move();
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
  int move_joint(const std::vector<double> &joint_positions, double a, double v,
                 double t, double r);
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
  int move_joint(const CartesianPose &cart_pose, double a, double v, double t,
                 double r);
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
  int move_linear(const std::vector<double> &joint_positions, double a,
                  double v, double t, double r);
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
  int move_linear(const CartesianPose &cart_pose, double a, double v, double t,
                  double r);
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
  int move_circular(const std::vector<double> &joint_via,
                    const std::vector<double> &joint, double rad, double a,
                    double v, double t, double r);
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
  int move_circular(const CartesianPose &cart_via, const CartesianPose &cart,
                    double rad, double a, double v, double t, double r);
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
  int speed_joint(double a, const std::vector<double> &v, double t = 0.0);
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
  int speed_linear(double a, const CartesianPose &v, double t = 0.0,
                   const CartesianPose &reference = {{"x", 0.0},
                                                     {"y", 0.0},
                                                     {"z", 0.0},
                                                     {"rx", 0.0},
                                                     {"ry", 0.0},
                                                     {"rz", 0.0}});
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
  int toward_joint(const std::vector<double> &joint_positions, double a,
                   double v, double t, double r);
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
  /**
   * @brief 跳过当前运动.
   */
  void skip_move();
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
  int get_robot_state();
  /**
   *  @brief 查看急停原因
   *
   *  @return 急停原因
   */
  int get_estop_reason();
  /**
   * @brief 获取控制器系统信息
   *
   * @return 控制器系统信息
   */
  SystemInfoData get_system_info();
  /**
   * @brief 获取机器人设备信息
   *
   * @return 机器人设备信息
   */
  RobotInfoData get_robot_info();
  /**
   * @brief 获取控制器硬件信息
   *
   * @return 控制器硬件信息
   */
  HardwareInfoData get_hardware_info();
  /**
   * @brief 获取控制器软件信息
   *
   * @return 控制器软件信息
   */
  SoftwareInfoData get_software_info();
  /**
   * @brief 通过控制器发起HTTP请求.
   *
   * @param request HTTP请求.
   * @return HTTP响应.
   */
  HttpResponseData http(HttpRequestData request);
  void clean(const BackupOptionsData &option);
  void backup(const std::string &file, const BackupOptionsData &option);
  BackupInfoData get_backup_info(const std::string &file);
  void restore(const std::string &file, const BackupOptionsData &option);
  void set_virtual_ip(const std::string &ifname, const std::string &ip);
  /**
   * @brief 获取控制箱dev设备列表
   *
   * @param prefix 设备名前缀过滤条件.
   * @return 设备名列表
   */
  std::vector<std::string> get_box_devices(const std::string &prefix);
  /**
   * @brief 获取数据库目录列表
   *
   * @return 目录列表
   */
  std::vector<DirData> get_dirs();
  /**
   * @brief 创建数据库目录.
   *
   * @param dir 目录数据.
   */
  void create_dir(DirData dir);
  /**
   * @brief 重命名数据库目录.
   *
   * @param from 原目录名.
   * @param to 新目录名.
   */
  void update_dir(std::string from, std::string to);
  /**
   * @brief 获取所有快捷路点
   *
   * @return 快捷路点列表
   */
  std::vector<ShortcutData> get_short_poses();
  /**
   * @brief 设置快捷路点
   */
  void set_short_pose(ShortcutData shortcut_data);
  /**
   * @brief 获取单个快捷路点
   */
  ShortcutData get_short_pose(unsigned int id);
  /**
   * @brief 获取所有快捷任务
   *
   * @return 快捷任务列表
   */
  std::vector<ShortcutData> get_short_tasks();
  /**
   * @brief 设置快捷任务
   */
  void set_short_task(ShortcutData shortcut_data);
  /**
   * @brief 获取单个快捷任务
   */
  ShortcutData get_short_task(unsigned int id);
  /**
   * @brief 获取条件任务触发器列表
   *
   * @return 触发器列表
   */
  std::vector<TriggerData> get_triggers();
  /**
   * @brief 设置条件任务触发器
   */
  void set_trigger(TriggerData trigger);
  /**
   * @brief 获取声光交互样式集
   *
   * @return 状态到声光样式的映射
   */
  std::map<std::string, LedStyleData> get_led_styles();
  /**
   * @brief 设置声光交互样式集
   *
   * @param styles 状态到声光样式的映射
   */
  void set_led_styles(std::map<std::string, LedStyleData> styles);
  /**
   * @brief 查询声光样式.
   *
   * @param name 样式名称.
   * @param dir 样式目录.
   */
  LedStyleData load_led_style(std::string name, std::string dir = "");
  /**
   * @brief 保存声光样式.
   *
   * @param name 样式名称.
   * @param style 样式数据.
   * @param dir 样式目录.
   */
  void save_led_style(std::string name, LedStyleData style,
                      std::string dir = "");
  /**
   * @brief 设置机器人状态对应的声光交互样式.
   *
   * @param state 机器人状态名称.
   * @param style 样式数据.
   */
  void set_led_style(std::string state, LedStyleData style);
  /**
   * @brief 查询声光样式列表.
   *
   * @param dir 声光样式目录.
   */
  std::vector<std::string> load_led_style_list(std::string dir = "");
  /**
   * @brief 获取伺服参数
   *
   * @return 伺服参数列表
   */
  std::vector<ServoParamData> get_servo_params();
  void set_servo_params(const std::vector<ServoParamData> &params);
  void find_zero();
  void set_zero(const std::vector<double> &pose,
                const std::vector<bool> &valids);
  void set_extra_servo_params(const std::vector<ExtraServoParamData> &params,
                              const std::vector<bool> &valids);
  void reset_extra_servo_params(const std::vector<bool> &valids);
  void set_flange_baud_rate(unsigned int baud_rate);
  /**
   * @brief 获取末端受力
   *
   * @return 末端受力和力矩
   */
  WrenchData get_tcp_force();
  /**
   * @brief 设置末端受力
   *
   * @param wrench 末端受力和力矩
   */
  void set_tcp_force(const WrenchData &wrench);
  /**
   * @brief 设置力控传感器.
   *
   * @param sensor 传感器类型.
   * @param address 传感器地址.
   */
  void set_force_mode_sensor(const std::string &sensor, unsigned int address);
  /**
   * @brief 设置力控参数.
   *
   * @param damping 阻尼.
   * @param gain 增益.
   * @param max_vel 最大速度，长度为6.
   */
  void set_force_mode_param(double damping, double gain,
                            const std::vector<double> &max_vel);
  /**
   * @brief 开始力控模式.
   *
   * @param limit 末端限速.
   * @param wrench 控制力.
   */
  void start_force_mode(const CartesianPose &limit, const WrenchData &wrench);
  /**
   * @brief 结束力控模式.
   */
  void end_force_mode();
  /**
   * @brief 查询已安装插件列表
   *
   * @return 插件信息列表
   */
  std::vector<PluginInfoData> load_plugins();
  /**
   * @brief 查询插件商店列表.
   *
   * @return 插件商店信息列表.
   */
  std::vector<PluginStoreInfoData> get_plugin_store();
  /**
   * @brief 查询已安装插件
   *
   * @param name 插件名称.
   */
  PluginInfoData load_plugin(const std::string &name);
  /**
   * @brief 执行插件命令.
   *
   * @param name 插件名称.
   * @param params 命令参数.
   */
  CommandStdoutData run_plugin_cmd(
      const std::string &name,
      const std::vector<std::string> &params = std::vector<std::string>{});
  /**
   * @brief 启用插件.
   *
   * @param name 插件名称.
   */
  CommandStdoutData enable_plugin(const std::string &name);
  /**
   * @brief 禁用插件.
   *
   * @param name 插件名称.
   */
  CommandStdoutData disable_plugin(const std::string &name);
  /**
   * @brief 重启插件守护进程.
   *
   * @param name 插件名称.
   */
  void restart_plugin_daemon(const std::string &name);
  /**
   * @brief 通过控制器RPC发现机器人设备.
   *
   * @return 设备信息列表.
   */
  std::vector<DiscoveredRobotData> discover_robots();
  /**
   * @brief 获取插件守护进程输出.
   *
   * @param name 插件名称.
   */
  CommandStdoutData get_plugin_daemon_stdout(const std::string &name);
  /**
   * @brief 获取控制器消息列表
   *
   * @return 消息列表
   */
  std::vector<MessageData> get_messages();
  /**
   * @brief 获取OTA升级状态
   *
   * @return OTA升级状态
   */
  OtaStateData get_ota_state();
  /**
   * @brief 查询是否需要系统升级
   *
   * @return 升级检查结果
   */
  CheckUpgradeData check_upgrade();
  /**
   * @brief 获取系统升级输出
   *
   * @return 升级输出
   */
  CommandStdoutData get_upgrade_stdout();
  void sub_buttons_status(uint64_t interval_min, uint64_t interval_max);
  void sub_kin_data(uint64_t interval_min, uint64_t interval_max);
  void sub_message(uint64_t interval_min, uint64_t interval_max);
  void sub_robot_state(uint64_t interval_min, uint64_t interval_max);
  void sub_phy_data(uint64_t interval_min, uint64_t interval_max);
  void sub_task_stdout(uint64_t interval_min, uint64_t interval_max);
  void start_ota(const std::string &address, const std::string &partition,
                 const std::string &file);
  void switch_partition(const std::string &address,
                        const std::string &partition);
  void start_upgrade();
  int box_test();
  std::string init_robot(const std::string &time, const std::string &auth,
                         const RobotInfoData &info);
  /**
   * @brief 获取机械臂物理数据
   *
   * @return 机械臂物理数据
   */
  PhysicalData get_phy_data();
  /**
   * @brief 获取机械臂关节运动数据
   *
   * @return 机械臂关节运动数据
   */
  JointMotionData get_kin_data();

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
   * @brief 设置多个数字输出
   */
  void set_dos(std::string device, unsigned int pin,
               std::vector<unsigned int> values);
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
   * @brief 设置多个模拟输出
   */
  void set_aos(std::string device, unsigned int pin, std::vector<double> values);
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
   * @brief 获取单个数字端口模式
   * @param device 设备名字，查看 @ref DEVICENAME.
   * @param pin 端口号，从 0 开始
   * @return false为输入模式，true为输出模式
   */
  bool get_dio_mode(std::string device, unsigned int pin);
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
  /**
   * @brief 启用按钮输入.
   *
   * @param device 按钮设备名称.
   * @param pin 按钮编号.
   */
  void enable_button(std::string device, unsigned int pin);
  /**
   * @brief 禁用按钮输入.
   *
   * @param device 按钮设备名称.
   * @param pin 按钮编号.
   */
  void disable_button(std::string device, unsigned int pin);
  /** @}*/

  /** \addtogroup CLAW
   *  @{
   */
  /**
   * @brief 初始化夹爪
   *
   * @param force_initilization 是否强制初始化
   */
  void init_claw(bool force_initilization);
  /**
   * @brief
   * 设置夹爪力度（力控）和幅度（位控）.如果在闭合过程中抓取到物体，则不再继续闭合以避免夹坏物体，判断的准则为这里设置的力的大小.
   *
   * @param force 力度（0-100）
   * @param amplitude 张合幅度（0-100）
   */
  void set_claw(double force, double amplitude);
  /**
   * @brief 设置夹爪指定模拟输出数据.
   *
   * @param address 夹爪地址.
   * @param value 输出值.
   */
  void set_claw_ao(unsigned int address, double value);

  /**
   * @brief 获取夹爪当前数据
   *
   * @return ClawData 数据
   */
  ClawData get_claw();
  ClawData get_claw_data();
  /**
   * @brief 读取夹爪指定模拟输入数据.
   *
   * @param address 夹爪地址.
   */
  double get_claw_ai(unsigned int address);
  /**
   * @brief 等待夹爪模拟输入满足指定关系.
   *
   * @param address 夹爪地址.
   * @param value 待比较的模拟输入值.
   * @param relation 比较关系，如 GTE.
   */
  void wait_claw_ai(unsigned int address, double value,
                    std::string relation = "EQ");
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
   * @brief 设置多个连续信号量
   */
  void set_signals(unsigned int index, std::vector<int> values);
  /**
   * @brief 获取信号量
   *
   * @param index: 信号量下标（取值范围0~255)
   * @return 返回对应的信号量
   */
  int get_signal(unsigned int index);
  /**
   * @brief 获取多个连续信号量
   */
  std::vector<int> get_signals(unsigned int index, unsigned int len);
  /**
   * @brief 等待信号量满足指定关系.
   *
   * @param index 信号量下标（取值范围0~255）.
   * @param value 待比较的信号量值.
   * @param relation 比较关系，如 EQ.
   */
  void wait_signal(unsigned int index, int value, std::string relation = "EQ");
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
  std::vector<unsigned int> get_task_list();
  /**
   * @brief 查询运行中的任务列表.
   */
  std::vector<TaskData> load_running_tasks();
  /**
   * @brief 获取任务输出.
   *
   * @param id 任务ID.
   */
  TaskStdoutData get_task_stdout(unsigned int id);
  /**
   * @brief 等待任务完成
   *
   * @param id: 任务的ID
   * @return 返回任务是否成功
   */
  std::string wait_task(unsigned int id);
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
  std::string load_task();
  std::string get_task_state();
  /**
   * @brief 获取任务状态.
   *
   * @param id 任务的ID.
   */
  std::string load_task(unsigned int id);
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
  KinematicsForwardResp get_forward_kin(
      const std::vector<double> &joint_positions);
  KinematicsForwardResp kinematics_forward(
      const std::vector<double> &joint_positions);

  /**
   * @brief 根据机械臂的末端位姿计算关节位置（位置的运动学逆解）.
   * @param pose: 机械臂末端位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @param joint_init_positions: 机械臂关节初始位置, 以数组形式传入.
   * @return 返回计算结果 \ref KinematicsInverseResp "KinematicsInverseResp".
   */
  KinematicsInverseResp get_inverse_kin(
      const CartesianPose &pose,
      const std::vector<double> &joint_init_positions = {});
  KinematicsInverseResp kinematics_inverse(
      const CartesianPose &pose,
      const std::vector<double> &joint_init_positions = {});
  /**
   * @brief 估算关节位置的灵活性.
   *
   * @param joint_positions 机械臂关节位置数组.
   * @return 灵活性估算值.
   */
  double measure_manipulation(const std::vector<double> &joint_positions);

  /**
   * @brief 位姿变换乘法（等价于对应的齐次坐标矩阵乘法）
   *
   * @param[in] a: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @param[in] b: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @return CartesianPose 返回的位姿，应当包括键为x,y,z,rz,ry,rx的值.
   */
  CartesianPose get_pose_trans(const CartesianPose &a, const CartesianPose &b);
  CartesianPose pose_times(const CartesianPose &a, const CartesianPose &b);
  /**
   * @brief 位姿加.
   *
   * @param pose 基准位姿.
   * @param delta 增量位姿.
   */
  CartesianPose get_pose_add(const CartesianPose &pose,
                             const CartesianPose &delta);
  /**
   * @brief 三点采样计算原点特征.
   *
   * @param o 原点.
   * @param x X轴点.
   * @param xy XY平面点.
   */
  CartesianPose calc_frame(const CartesianPose &o, const CartesianPose &x,
                           const CartesianPose &xy);
  /**
   * @brief 三点采样计算工具中心点.
   *
   * @param poses 采样位姿列表，至少3个.
   */
  CartesianPose calc_tcp(const std::vector<CartesianPose> &poses);

  /**
   * @brief 位姿变换的逆（等价于对应的齐次坐标矩的逆）
   *
   * @param in: 位姿，应当包括键为x,y,z,rz,ry,rx的值.
   * @return CartesianPose 返回位姿变换的逆，应当包括键为x,y,z,rz,ry,rx的值.
   */
  CartesianPose get_pose_inverse(const CartesianPose &in);
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
   * @brief 通过URL下载文件到控制器.
   *
   * @param dir: 保存的文件路径.
   * @param name: 保存的文件名.
   * @param url: 下载URL.
   */
  void download_file(const std::string &dir, const std::string &name,
                     const std::string &url);

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
  void zip(const std::string &from_dir, std::vector<std::string> files,
           const std::string &to_dir, const std::string &name);
  /**
   *  @brief 将zip文件解压到文件系统.
   *
   * @param from_dir zip文件的路径.
   * @param name zip文件的名称.
   * @param files zip文件内的文件名.
   * @param to_dir  解压到的路径.
   */
  void unzip(const std::string &from_dir, const std::string &name,
             std::vector<std::string> files, const std::string &to_dir);
  /**
   * @brief 查询zip内文件列表.
   */
  std::vector<std::tuple<bool, std::string>> load_zip_list(
      const std::string &zip, const std::string &dir,
      const std::string &prefix, const std::string &suffix);

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
   *  @brief 获取当前DH参数.
   *
   *  @return DH参数列表.
   */
  std::vector<DhParamData> get_dh();
  void set_dh(const std::vector<DhParamData> &params);
  /**
   *  @brief 设置速度因子.
   *
   *  @param factor 速度因子百分比，范围0-100.
   */
  void set_kin_factor(int factor);
  void set_velocity_factor(int factor);
  /**
   *  @brief 获取当前的速度因子.
   *
   *  @return 速度因子百分比.
   */
  int get_kin_factor();
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
   *  @brief 保存末端负载.
   *
   *  @param name 负载名称.
   *  @param payload 负载数据，包含mass,x,y,z.
   *  @param dir 负载目录.
   */
  void save_payload(std::string name, std::map<std::string, double> payload,
                    std::string dir = "");
  /**
   *  @brief 获取末端负载设置.
   *
   *  @return 由负载质量mass和负载偏移组成的元组.
   */
  std::map<std::string, double> get_payload();
  /**
   * @brief 从资源库加载末端负载.
   *
   * @param name 负载名称.
   * @param dir 负载目录.
   */
  std::map<std::string, double> load_payload(std::string name,
                                             std::string dir = "");
  /**
   * @brief 查询负载列表.
   *
   * @param dir 负载目录.
   */
  std::vector<std::string> load_payload_list(std::string dir = "");
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
   * @brief 使能碰撞检测.
   *
   */
  void enable_collision_detector();
  /**
   * @brief 临时禁用碰撞检测.
   *
   */
  void disable_collision_detector();
  /**
   * @brief 设置碰撞监测误差.
   *
   * @param diffs 各关节的碰撞监测误差.
   */
  void set_collision_torque_diff(const std::vector<double> &diffs);
  /**
   * @brief 获取碰撞监测误差.
   *
   * @return 各关节的碰撞监测误差.
   */
  std::vector<double> get_collision_torque_diff();
  /**
   * @brief 设置碰撞检测参数.
   *
   * @param config 碰撞检测配置.
   */
  void set_collision_detector(const CollisionDetectorConfig &config);
  /**
   * @brief 获取碰撞检测参数.
   *
   * @return 碰撞检测配置.
   */
  CollisionDetectorConfig get_collision_detector();
  /**
   * @brief 使能安全限位检测.
   *
   */
  void enable_limit();
  /**
   * @brief 临时禁用安全限位检测.
   *
   */
  void disable_limit();
  /**
   * @brief 设置关节限位.
   *
   * @param joints 各关节的限位配置.
   */
  void set_joints_limit(const std::vector<JointLimitConfig> &joints);
  /**
   * @brief 获取关节限位.
   *
   * @return 各关节的限位配置.
   */
  std::vector<JointLimitConfig> get_joints_limit();
  /**
   * @brief 设置笛卡尔空间限位.
   *
   * @param limit 笛卡尔空间限位配置.
   */
  void set_cart_limit(const CartesianLimitConfig &limit);
  /**
   * @brief 获取笛卡尔空间限位.
   *
   * @return 笛卡尔空间限位配置.
   */
  CartesianLimitConfig get_cart_limit();
  /**
   * @brief 从资源库加载tcp.
   *
   * @param name 点位名称.
   * @param dir 点位目录.
   */
  CartesianPose load_tcp(std::string name, std::string dir = "");
  /**
   * @brief 保存tcp.
   *
   * @param name tcp名称.
   * @param tcp tcp数据，包含x,y,z,rx,ry,rz.
   * @param dir tcp目录.
   */
  void save_tcp(std::string name, CartesianPose tcp, std::string dir = "");
  /**
   * @brief 查询工具中心点列表.
   *
   * @param dir 工具中心点目录.
   */
  std::vector<std::string> load_tcp_list(std::string dir = "");
  /**
   * @brief 查询轨迹列表.
   *
   * @param dir 轨迹目录.
   */
  std::vector<std::string> load_trajectory_list(std::string dir = "");
  /**
   * @brief 查询轨迹.
   *
   * @param name 轨迹名称.
   * @param dir 轨迹目录.
   */
  TrajectoryData load_trajectory(std::string name, std::string dir = "");
  /**
   * @brief 保存轨迹.
   *
   * @param name 轨迹名称.
   * @param trajectory 轨迹数据.
   * @param dir 轨迹目录.
   */
  void save_trajectory(std::string name, TrajectoryData trajectory,
                       std::string dir = "");
  unsigned int move_trajectory(std::string name, std::string dir = "");
  /**
   * @brief 开始记录轨迹.
   *
   * @param kind 轨迹类型.
   * @param duration 采样周期.
   */
  void start_record_trajectory(std::string kind, double duration);
  /**
   * @brief 结束记录轨迹并保存.
   *
   * @param name 轨迹名称.
   * @param dir 轨迹目录.
   */
  void end_record_trajectory(std::string name, std::string dir = "");
  /**
   * @brief 查询路点列表.
   *
   * @param dir 路点目录.
   */
  std::vector<std::string> load_pose_list(std::string dir = "");
  /**
   * @brief 查询路点.
   *
   * @param name 路点名称.
   * @param dir 路点目录.
   */
  PoseData load_pose(std::string name, std::string dir = "");
  /**
   * @brief 保存路点.
   *
   * @param name 路点名称.
   * @param pose 路点数据.
   * @param dir 路点目录.
   */
  void save_pose(std::string name, PoseData pose, std::string dir = "");
  /**
   * @brief 查询特征列表.
   *
   * @param dir 特征目录.
   */
  std::vector<std::string> load_frame_list(std::string dir = "");
  /**
   * @brief 查询特征.
   *
   * @param name 特征名称.
   * @param dir 特征目录.
   */
  FrameData load_frame(std::string name, std::string dir = "");
  /**
   * @brief 保存特征.
   *
   * @param name 特征名称.
   * @param frame 特征数据.
   * @param dir 特征目录.
   */
  void save_frame(std::string name, FrameData frame, std::string dir = "");
  /**
   * @brief 查询机器人结构列表.
   *
   * @param dir 机器人结构目录.
   */
  std::vector<std::string> load_structure_list(std::string dir = "");
  /**
   * @brief 查询机器人结构.
   *
   * @param name 机器人结构名称.
   * @param dir 机器人结构目录.
   */
  StructureData load_structure(std::string name, std::string dir = "");
  /**
   * @brief 保存机器人结构.
   *
   * @param name 机器人结构名称.
   * @param structure 机器人结构数据.
   * @param dir 机器人结构目录.
   */
  void save_structure(std::string name, StructureData structure,
                      std::string dir = "");
  /** @}*/

  /** \addtogroup MODBUS
   *  @{
   */

  /**
   * @brief 查询Modbus配置列表.
   *
   * @param dir Modbus配置目录.
   */
  std::vector<std::string> load_modbus_list(std::string dir = "");
  /**
   * @brief 查询Modbus设备.
   *
   * @param name Modbus设备名称.
   * @param dir Modbus配置目录.
   */
  ModbusData load_modbus(std::string name, std::string dir = "");
  /**
   * @brief 保存Modbus设备.
   *
   * @param name Modbus设备名称.
   * @param modbus Modbus设备数据.
   */
  void save_modbus(std::string name, ModbusData modbus);
  /**
   * @brief 设置Modbus设备超时时间.
   *
   * @param device Modbus设备名称.
   * @param timeout 超时时间，单位毫秒.
   */
  void set_modbus_timeout(std::string device, unsigned int timeout);
  /**
   * @brief 设置Modbus设备重试次数.
   *
   * @param device Modbus设备名称.
   * @param retry 重试次数.
   */
  void set_modbus_retry(std::string device, unsigned int retry);
  /**
   * @brief 断开Modbus设备连接.
   *
   * @param device Modbus设备名称.
   */
  void disconnect_modbus(std::string device);
  /**
   * @brief 查询Modbus寄存器列表.
   *
   * @param device Modbus设备名称.
   */
  std::vector<std::string> load_modbus_register_list(std::string device);
  /**
   * @brief 查询Modbus寄存器.
   *
   * @param device Modbus设备名称.
   * @param name 寄存器名称.
   */
  ModbusRegisterData load_modbus_register(std::string device,
                                          std::string name);
  /**
   * @brief 保存Modbus寄存器.
   *
   * @param device Modbus设备名称.
   * @param name 寄存器名称.
   * @param reg Modbus寄存器数据.
   */
  void save_modbus_register(std::string device, std::string name,
                            ModbusRegisterData reg);

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
  void write_multiple_coils(std::string device, std::string addr,
                             std::vector<bool> values);
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
   * @brief 设置串口超时时间.
   *
   * @param device 设备名称.
   * @param timeout 超时时间，单位毫秒.
   */
  void set_serial_timeout(std::string device, unsigned int timeout);
  /**
   * @brief 设置串口校验位.
   *
   * @param device 设备名称.
   * @param parity 校验位. 0:无校验; 1:奇校验; 2:偶校验.
   *
   */
  void set_serial_parity(std::string device, unsigned int parity);
  /**
   * @brief 串口发送数据.
   *
   * @param device 设备名称.
   * @param data u8数组.
   */
  void write_serial(std::string device, std::vector<unsigned int> data);
  /**
   * @brief 串口读取数据.
   *
   * @param device 设备名称.
   * @param len 单次接收的最大缓冲长度.
   */
  std::vector<unsigned int> read_serial(std::string device, unsigned int len);
  /**
   * @brief 清除串口收发缓存.
   *
   * @param device 设备名称.
   */
  void clear_serial(std::string device);
  /** @}*/

  /** \addtogroup STORAGE
   *  @{
   */
  /**
   * @brief 设置存储项.
   *
   * @param item 存储项数据结构.
   */
  void set_item(StorageItem item);
  /**
   * @brief 获取存储项.
   *
   * @param name 存储项名称.
   * @return 存储项值.
   */
  StorageItem get_item(std::string name);
  /**
   * @brief 获取存储项列表.
   *
   * @param prefix 存储项前缀.
   * @return 存储项列表.
   */
  std::vector<StorageItem> get_items(std::string prefix);
  /** @}*/

 protected:
  std::unique_ptr<RobotImpl> impl_; /*!< 内部实现数据结构，用户无需关注. */
};

}  // namespace l_master

}  // namespace lebai
