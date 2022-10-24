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

namespace lebai
{
  namespace l_master
  {
  /**
   * @brief 运动学正解的返回值数据结构
   * 
   */
  struct KinematicsForwardResp
  {
    std::array<double, 6> pose; /*!< 笛卡尔坐标位置，依次为 x, y, z, rz, ry, rx */
    bool ok = false;  /*!< 计算是否成功 */
  };
  /**
   * @brief 运动学逆解的返回值数据结构
   * 
   */
  struct KinematicsInverseResp
  {
    std::map<std::string, double> joint_positions;  /*!< 机械臂关节位置的map数据，应当包括"j1","j2","j3","j4","j5","j6"六个关节的角度值。  */
    bool ok = false;  /*!< 计算是否成功 */
  };  
  /**
   *  @brief 机械臂的主要接口对象，通过本对象的方法与机械臂进行数据交互。
   *
   */
  class Robot
  {
  public:
    /**
     * @brief 内部实现.
     * @note 用户无需使用.
     */
    class RobotImpl;



    /**
     * @brief 构造Robot对象.
     * 
     * @param ip 机械臂IP地址.
     * @param simulator 用于表示机械臂是否为仿真机械臂(docker仿真或控制器运行在仿真模式下)的标志.True表示仿真模式，False表示实物机械臂.
     */
    explicit Robot(std::string ip, bool simulator = false);
    /**
     * @brief 析构Robot对象.
     * 
     */
    virtual ~Robot();
    /**
     * @brief CartesianPose是用来表示空间位姿的数据结构.
     * 数据为一个包含六个数的数组，其数据排布为: x,y,z,rz,ry,rx.
     * 
     */
    using CartesianPose = std::array<double, 6>;
    /**
     * 示例代码: 
     * 
     *     std::string movej_req = "{\"param\":{\"v\":0.1},\"pose\":{\"joint\":{\"delta\":"{\"joint\":[-1.0,0.0,0.0,0.0,0.0,0.0]}}}}";
     *     resp = robot.call("movej", movej_req);  
     *     std::cout << "resp: " << std::get<0>(resp) << ", " << std::get<1>(resp) << std::endl;
     * 
     *  
     * @brief 用JSON格式字符串调用机械臂的接口.
     * @note SDK采用JSONRPC协议和机械臂控制进行通讯. 您可以自行生成JSON数据，并通过本接口调用相应的JSONRPC，并获取返回的结果和JSON数据。
     * @param[in] method 方法名字
     * @param[in] params 请求JSONRPC的JSON格式字符串。
     * @return 返回一个元组，第一个元素是返回码，第二个元素是数据字符串。
     * 如果返回码为0，表示调用成功，第二个元素是JSONRPC的返回数据。
     * 如果返回码为非0，表示调用失败，第二个元素是错误信息。
     */
    std::tuple<int, std::string> call(const std::string & method, const std::string & params);
    
    /** \defgroup STARTSTOP 启动停止.
     *  \brief    启动停止相关的接口.
     */

    /** \defgroup MOTION 机械臂运动.
     *  \brief    机械臂运动相关的接口.
     */

    /** \defgroup STATUS 机械臂状态.
     *  \brief    机械臂状态相关的接口.
     */
    
    /** \defgroup CABINETIO 控制箱IO.
     *  \brief    控制箱IO相关的接口.
     */

    /** \defgroup FLANGEIO 法兰IO.
     *  \brief    法兰IO相关的接口.
     */

    /** \defgroup EXTRAIO 扩展IO.
     *  \brief    扩展IO相关的接口.
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

    /** \defgroup CONTROL 程序控制.
     *  \brief    程序控制相关的接口.
    */

    /** \defgroup ROBOTICS 机器人.
     *  \brief    机器人计算相关的接口.
     */       

    /** \addtogroup STARTSTOP
     *  @{
     */
    
    /**
     * @brief 启动机械臂（机械臂上使能）。
     * 
     */
    void start_sys();
    /**
     * @brief 停止机械臂（机械臂上使能）。
     * 
     */

    void stop_sys();
    /**
     * @brief 关闭机器人电源（关机）。
     * 
     */
    void powerdown();
    /**
     * @brief 停止运动（但不下电）。
     * 
     */
    void stop();
    /**
     * @brief 紧急停止（急停）。
     * 
     */
    void estop();
    /**
     * @brief 进入示教模式。
     * 
     */
    void teach_mode();
    /**
     * @brief 退出示教模式。
     * 
     */
    void end_teach_mode();
    /**
     * @brief 暂停运动。
     * 
     */
    void pause();
    /**
     * @brief 恢复运动。
     * 
     */
    void resume();
    /** @}*/


    /** \addtogroup MOTION
     *  @{
     */

    /**
     * 示例代码: 
     * 
     *     std::map<std::string, double> joint_positions;
     *     joint_positions["j1"] = 0.0;
     *     joint_positions["j2"] = -60.0 / 180.0 * M_PI;
     *     joint_positions["j3"] = 80.0 / 180.0 * M_PI;
     *     joint_positions["j4"] = -10.0 / 180.0 * M_PI;
     *     joint_positions["j5"] = -60.0 / 180.0 * M_PI;
     *     joint_positions["j6"] = 0.0;
     *     robot.movej(joint_positions, 3.0, 1.0, 0.0, 0.0);
     * 
     * 
     * @brief 通过关节位置发送机械臂关节移动
     * @note 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束。
     * @param[in] joint_positions 目标位置的关节map数据,应当包括"j1","j2","j3","j4","j5","j6"六个关节的角度值.
     * @param[in] a 加速度
     * @param[in] v 速度
     * @param[in] t 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数。
     * @param[in] r 交融半径，设置为0，则无交融半径。
     * @return true 发送成功
     * @return false 发送失败
     * 
     */
    bool movej(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r);
    /**
     * 示例代码: 
     * 
     *     robot.movej({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 3.0, 1.0, 0.0, 0.0);
     * 
     * 
     * @brief 通过坐标位置发送机械臂关节移动
     * @note 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束。
     * @param[in] cart_pose 目标位置在机器人基座标系下的坐标数据(目前不支持在其它坐标系下的坐标数据)，CartesianPose = std::array<double,6>，数组大小为6，前三个数据为空间中的[x,y,z]点位，后三个数据是以欧拉ZYX形式的姿态数据[rz,ry,rx]。
     * @param[in] a 加速度
     * @param[in] v 速度
     * @param[in] t 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数。
     * @param[in] r 交融半径，设置为0，则无交融半径。
     * @return true 发送成功
     * @return false 发送失败
     */    
    bool movej(const CartesianPose & cart_pose, double a, double v, double t, double r);
    /**
     * 示例代码: 
     * 
     *     std::map<std::string, double> joint_positions;
     *     joint_positions["j1"] = 0.0;
     *     joint_positions["j2"] = -60.0 / 180.0 * M_PI;
     *     joint_positions["j3"] = 80.0 / 180.0 * M_PI;
     *     joint_positions["j4"] = -10.0 / 180.0 * M_PI;
     *     joint_positions["j5"] = -60.0 / 180.0 * M_PI;
     *     joint_positions["j6"] = 0.0;
     *     robot.movel(joint_positions, 3.0, 1.0, 0.0, 0.0);
     * 
     * 
     * @brief 通过关节位置发送机械臂直线移动
     * @note 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束。
     * @param[in] joint_positions 目标位置的关节map数据，应当包括"j1","j2","j3","j4","j5","j6"六个关节的角度值。
     * @param[in] a 加速度
     * @param[in] v 速度
     * @param[in] t 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数。
     * @param[in] r
     *  交融半径，设置为0，则无交融半径。
     * @return true 发送成功
     * @return false 发送失败
     */
    bool movel(const std::map<std::string, double> & joint_positions, double a, double v, double t, double r);
    /**
     * 示例代码: 
     * 
     *     robot.movel({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 3.0, 1.0, 0.0, 0.0);
     * 
     * 
     * @brief 通过坐标位置发送机械臂直线移动
     * @note 该接口为异步接口，仅向控制器内部的运动缓冲区写入一个关节移动即返回，不会等待运动结束。
     * @param cart_pose 目标位置在机器人基座标系下的坐标数据(目前不支持在其它坐标系下的坐标数据)，CartesianPose = std::array<double,6>，数组大小为6，前三个数据为空间中的[x,y,z]点位，后三个数据是以欧拉ZYX形式的姿态数据[rz,ry,rx]。
     * @param a 加速度
     * @param v 速度
     * @param t 时间参数，如果设置时间不为零，则按照时间计算出速度，而不使用速度参数。
     * @param r 交融半径，设置为0，则无交融半径。
     * @return true 发送成功
     * @return false 发送失败
     */    
    bool movel(const CartesianPose & cart_pose, double a, double v, double t, double r);
    /**
     * @brief 等待运动完成
    */
    void wait_move();
    /** @}*/
    /** \addtogroup STATUS
     *  @{
     */
    
    /**
     * @brief 获取机器人状态码
     * 
     * @return int 返回状态码。
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
     *     10	 |   正在停止	     |  机器人空闲状态转到停止状态
     *     11	 |   示教中	       |  机器人处于示教模式中
     *     12	 |   已停止	       |  机器人处于停止状态，非急停状态
     *  
     * @note See state <a href="https://help.lebai.ltd/guide/basic.html#%E6%9C%BA%E5%99%A8%E4%BA%BA%E7%8A%B6%E6%80%81">here</a>.
     */
    int get_robot_mode();
    /**
     * @brief 获取机械臂关节当前反馈位置 
     * 
     * @return 关节反馈位置数据
     */
    std::vector<double> get_actual_joint_positions();
    /**
     * @brief 获取机械臂关节当前控制位置 
     * 
     * @return 关节控制位置数据
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
     * @brief 获取机械臂末端在机械臂基坐标系下的实际位姿
     * 
     * @return CartesianPose 
     */
    CartesianPose get_actual_tcp_pose();
    /**
     * @brief 获取机械臂末端在机械臂基坐标系下的控制位姿
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
     * @return * std::vector<double> 
     */
    std::vector<double> get_actual_joint_torques();
    /**
     * @brief 获取机械臂关节当前控制力矩
     * 
     * @return * std::vector<double> 
     */
    std::vector<double> get_target_joint_torques();

    /** @}*/

  
    

    /** \addtogroup CABINETIO
     *  @{
     */
    
    /**
     * @brief 获取控制箱数字输入
     * 
     * @param pin 端口，从 0 开始
     * @return 返回输入数值
     */
    bool get_di(unsigned int pin);

    /**
     * @brief 设置控制箱数字输出
     * 
     * @param pin 端口，从 0 开始
     * @param value 待设置的值，可以指定 1/0，也可以是 true/false
     */
    void set_do(unsigned int pin, bool value);

    /**
     * @brief 获取控制箱模拟输入
     * 
     * @param pin 端口，从 0 开始
     * @return 返回输入数值
     */
    double get_ai(unsigned int pin);

    /**
     * @brief 设置控制箱模拟输出
     * 
     * @param pin 端口，从 0 开始
     * @param value 待设置的值
     */
    void set_ao(unsigned int pin, double value);
    /** @}*/

    /** \addtogroup FLANGEIO
     *  @{
     */
    
    /**
     * @brief 获取法兰数字输入
     * 
     * @param pin 端口，从 0 开始
     * @return 返回输入数值
     */
    bool get_flange_di(unsigned int pin);

    /**
     * @brief 设置法兰数字输出
     * 
     * @param pin 端口，从 0 开始
     * @param value 待设置的值，可以指定 1/0，也可以是 true/false
     */
    void set_flange_do(unsigned int pin, bool value);
    /** @}*/

    /** \addtogroup EXTRAIO
     *  @{
     */
    
    /**
     * @brief 获取扩展数字输入
     * 
     * @param pin 端口，从 0 开始
     * @return 返回输入数值
     */
    bool get_extra_di(unsigned int pin);

    /**
     * @brief 设置扩展数字输出
     * 
     * @param pin 端口，从 0 开始
     * @param value 待设置的值，可以指定 1/0，也可以是 true/false
     */
    void set_extra_do(unsigned int pin, bool value);

    /**
     * @brief 获取扩展模拟输入
     * 
     * @param pin 端口，从 0 开始
     * @return 返回输入数值
     */
    double get_extra_ai(unsigned int pin);

    /**
     * @brief 设置扩展模拟输出
     * 
     * @param pin 端口，从 0 开始
     * @param value 待设置的值
     */
    void set_extra_ao(unsigned int pin, double value);    
    /** @}*/ 

    /** \addtogroup CLAW
     *  @{
     */   
    /**
     * @brief 设置夹爪力度（力控）和幅度（位控）。如果在闭合过程中抓取到物体，则不再继续闭合以避免夹坏物体，判断的准则为这里设置的力的大小。
     * 
     * @param force 力度（0-100）
     * @param amplitude 张合幅度（0-100）
    */
    void set_claw(double force, double amplitude);
    /**
     * @brief 获取夹爪当前数据
     * 
     * @return std::tuple<double, double ,bool> 第一个数据为夹爪力度，第二个数据为幅度，第三个数据为开度是否稳定
     */
    std::tuple<double, double ,bool> get_claw();
    /** @}*/

    /** \addtogroup LED
     *  @{
     */

    /**
     * @brief 设置LED灯状态。
     * 
     * @param mode 亮灯模式。0：不变；1：关闭；2：常亮；3：呼吸；4：均分旋转；5：同色旋转；6：闪烁
     * @param speed 速度。1：快速；2：正常；3：慢速
     * @param color 最多包含 4 个 0 ~ 15 之间的整数
     */
    void set_led(unsigned int mode,unsigned int speed,const std::vector<unsigned int> & color);
    /**
     * @brief 设置声音
     * 
     * @param voice 声音列表
     * @param volume 音量。0：静音；1：低；2：正常；3：高
     */
    void set_voice(unsigned int voice,unsigned int volume);
    /**
     * @brief 开关风扇
     * 
     * @param status 状态。1：关闭；2：开启
     */
    void set_fan(unsigned int status);
    /** @}*/
    

    /** \addtogroup SIGNAL
     *  @{
    */

    /**
     * @brief 设置信号量
     * 
     * @param index 信号量下标（取值范围0~255)
     * @param value 待设置的信号量(32位有符号整数)
    */
    void set_signal(unsigned int index,int value);
    /**
     * @brief 获取信号量
     * 
     * @param index 信号量下标（取值范围0~255)
     * @return 返回对应的信号量
    */
    int get_signal(unsigned int index);
    /**
     * @brief 增加指定下标的信号量值，该操作是原子的。
     * 
     * @param index 信号量下标（取值范围0~255）
     * @param value 待增加的信号量值
    */
    void add_signal(unsigned int index,int value);
    /** @}*/

    /** \addtogroup CONTROL
     *  @{
    */

    /**
      * @brief 调用场景
      * 
      * @param name 调用场景的名字
      * @param is_main 是否以主任务方式运行（主任务会排队执行，子任务会并发执行）
      * @param loop_to 循环次数（默认0永久循环）
      * @param dir 调用场景所在的文件夹名
      * @param params 其他参数
    */
    unsigned int scene(std::string name,bool is_main,unsigned int loop_to,std::string dir,const std::vector<std::string> & params);
    /**
      * @brief 调用场景
      * 
      * @param name 调用场景的名字
      * @param is_main 是否以主任务方式运行（主任务会排队执行，子任务会并发执行）
      * @param loop_to 循环次数（默认0永久循环）
      * @param dir 调用场景所在的文件夹名
    */
    unsigned int scene(std::string name,bool is_main,unsigned int loop_to,std::string dir);
    /**
      * @brief 调用场景
      * 
      * @param name 调用场景的名字
    */
    unsigned int scene(std::string name);
    /**
     * @brief 查询任务列表
    */
    std::vector<unsigned int> load_task_list();
    /**
     * @brief 暂停任务与运动
     * 
     * @param id 任务的ID
     * @param time 暂停的时间
     * @param wait 是否等待
    */
    void pause_task(unsigned int id,unsigned long time,bool wait);
    /**
     * @brief 恢复任务与运动
     * 
     * @param id 任务的ID
    */
    void resume_task(unsigned int id);
    /**
     * @brief 取消任务与运动
     * 
     * @param id 任务的ID
    */
    void cancel_task(unsigned int id);
    /** @}*/

    /** \addtogroup ROBOTICS
     *  @{
     */
    /**
     * @brief 根据机械臂关节位置计算机器人末端位姿（位置的运动学正解）
     * @param joint_positions 机械臂关节位置的map数据，应当包括"j1","j2","j3","j4","j5","j6"六个关节的角度值。
     * @return 返回计算结果 \ref KinematicsForwardResp "KinematicsForwardResp"。
     *
     */
    KinematicsForwardResp kinematics_forward(const std::map<std::string, double> & joint_positions);
    
    /**
     * @brief 根据机械臂的末端位姿计算关节位置（位置的运动学逆解）
     * @param pose 机械臂末端位姿，依次为 x, y, z, rz, ry, rx
     * @param joint_init_positions 机械臂关节初始位置, 以数组形式传入。
     * @return 返回计算结果 \ref KinematicsInverseResp "KinematicsInverseResp"。
     */
    KinematicsInverseResp kinematics_inverse(const std::array<double, 6> & pose, const std::vector<double> & joint_init_positions = {});
    
    /**
     * @brief 位姿变换乘法（等价于对应的齐次坐标矩阵乘法）
     * 
     * @param a 位姿，依次为 x, y, z, rz, ry, rx
     * @param b 位姿，依次为 x, y, z, rz, ry, rx
     * @return std::array<double, 6> 返回的位姿，依次为 x, y, z, rz, ry, rx
     */
    std::array<double, 6> pose_times(const std::array<double, 6> & a, const std::array<double, 6> & b);

    /**
     * @brief 位姿变换的逆（等价于对应的齐次坐标矩的逆）
     * 
     * @param in 位姿，依次为 x, y, z, rz, ry, rx
     * @return std::array<double, 6> 返回位姿变换的逆，依次为 x, y, z, rz, ry, rx
     */    
    std::array<double, 6> pose_inverse(const std::array<double, 6> & in);
    /** @}*/    

  protected:
    std::unique_ptr<RobotImpl> impl_; /*!< 内部实现数据结构，用户无需关注。 */
  };
  }

} // namespace l_master_sdk
