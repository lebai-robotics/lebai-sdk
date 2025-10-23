/**
 * Copyright 2022-2025 lebai.ltd
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

namespace lebai {
	namespace l_master {

		/**
		 *  @brief 独立的夹爪的主要接口对象，通过本对象的方法与夹爪进行数据交互.
		 *
		 *
		 */
		class Gripper {
		public:
			/**
			 * @brief 内部实现.
			 * @note 用户无需使用.
			 *
			 */
			class GripperImpl;

			/**
			 * @brief 构造Gripper对象.
			 */
			explicit Gripper(const std::string& port_name);
			/**
			 * @brief 析构Robot对象.
			 *
			 */
			virtual ~Gripper();
			/**
			 * @brief 设置夹爪幅度. 范围为0-100.
			 * @note 请求的寄存器地址为40000,0x9C40。
			 *
			 * @param Position 位置.
			 */
			void SetPosition(unsigned int position);
			/**
			 * @brief 获取夹爪当前位置.
			 * @note 请求的寄存器地址为40005,0x9C45。
			 *
			 * @return 当前位置.
			 */
			unsigned int GetCurrentPosition() const;
			/**
			 * @brief 设置夹爪夹持力道. 范围为0-100.
			 * @note 请求的寄存器地址为40001,0x9C41。
			 *
			 * @param force 力道.
			 */
			void SetForce(unsigned int force);
			/**
			 * @brief 获取夹爪当前力道.
			 * @note 请求的寄存器地址为40006,0x9C46。
			 *
			 * @return 当前力道.
			 */
			unsigned int GetCurrentForce() const;
			/**
			 * @brief 设置夹爪开合速度. 范围为0-100.
			 * @note 当 persistent=false 时，请求寄存器地址为40010,0x9C4A；当 persistent=true 时，请求寄存器地址为40011,0x9C4B。
			 *
			 * @param velocity 速度.
			 * @param persistent 是否写入掉电保持寄存器。
			 */
			void SetVelocity(unsigned int velocity, bool persistent);
			/**
			 * @brief 获取夹爪当前开合速度.
			 * @note 当 persistent=false 时，请求寄存器地址为40010,0x9C4A；当 persistent=true 时，请求寄存器地址为40011,0x9C4B。
			 *
			 * @return 当前速度.
			 */
			unsigned int GetCurrentVelocity(bool persistent) const;
			/**
			 * @brief 执行夹爪校准动作.
			 * @note 请求的寄存器地址为40007,0x9C47。
			 *
			 */
			void DoCalibration();
			/**
			 * @brief 获取夹爪是否已校准完成状态.
			 * @note 请求的寄存器地址为40008,0x9C48。
			 *
			 * @return true 已校准完成，false 未校准.
			 */
			bool IsCalibrated() const;
			/**
			 * @brief 打开自动校准功能.
			 * @note 请求的寄存器地址为40009,0x9C49。
			 *
			 */
			void TurnOnAutoCalibration();
			/**
			 * @brief 关闭自动校准功能.
			 * @note 请求的寄存器地址为40009,0x9C49。
			 *
			 */
			void TurnOffAutoCalibration();



		protected:
			std::unique_ptr<GripperImpl> impl_; /*!< 内部实现数据结构，用户无需关注. */
		};

	}  // namespace l_master

}  // namespace lebai
