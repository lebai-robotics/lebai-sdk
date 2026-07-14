#pragma once

#include <cstdint>

namespace lebai {
namespace l_master {
namespace gripper_protocol {

inline constexpr std::uint16_t kAutoCalibrationRegister = 40090;
inline constexpr std::uint16_t kAutoCalibrationEnableValue = 3;
inline constexpr std::uint16_t kAutoCalibrationDisableValue = 2;

}  // namespace gripper_protocol
}  // namespace l_master
}  // namespace lebai
