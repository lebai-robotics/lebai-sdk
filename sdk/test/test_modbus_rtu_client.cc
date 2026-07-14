#include "modbus_rtu_client.hh"
#include "gripper_protocol.hh"

#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace {

using lebai::l_master::ModbusRtuClient;
namespace gripper_protocol = lebai::l_master::gripper_protocol;

TEST(ModbusRtuClientTest, BuildsReadHoldingRegistersRequest) {
  const auto frame =
      ModbusRtuClient::build_read_holding_registers_request(1, 0x0000, 10);

  const std::vector<uint8_t> expected = {0x01, 0x03, 0x00, 0x00,
                                         0x00, 0x0A, 0xC5, 0xCD};
  EXPECT_EQ(frame, expected);
}

TEST(ModbusRtuClientTest, BuildsWriteMultipleRegistersRequest) {
  const auto frame =
      ModbusRtuClient::build_write_multiple_registers_request(1, 0x9C40, {30});

  ASSERT_EQ(frame.size(), 11U);
  EXPECT_EQ(frame[0], 0x01);
  EXPECT_EQ(frame[1], 0x10);
  EXPECT_EQ(frame[2], 0x9C);
  EXPECT_EQ(frame[3], 0x40);
  EXPECT_EQ(frame[4], 0x00);
  EXPECT_EQ(frame[5], 0x01);
  EXPECT_EQ(frame[6], 0x02);
  EXPECT_EQ(frame[7], 0x00);
  EXPECT_EQ(frame[8], 0x1E);
  EXPECT_NO_THROW(ModbusRtuClient::validate_crc(frame));
}

TEST(ModbusRtuClientTest, BuildsDisableAutoCalibrationRequest) {
  EXPECT_EQ(40090, gripper_protocol::kAutoCalibrationRegister);
  EXPECT_EQ(3, gripper_protocol::kAutoCalibrationEnableValue);
  EXPECT_EQ(2, gripper_protocol::kAutoCalibrationDisableValue);
  EXPECT_NE(gripper_protocol::kAutoCalibrationEnableValue,
            gripper_protocol::kAutoCalibrationDisableValue);

  const auto frame = ModbusRtuClient::build_write_single_register_request(
      1, gripper_protocol::kAutoCalibrationRegister,
      gripper_protocol::kAutoCalibrationDisableValue);

  const std::vector<uint8_t> expected = {0x01, 0x06, 0x9C, 0x9A,
                                         0x00, 0x02, 0x06, 0x74};
  EXPECT_EQ(expected, frame);
}

TEST(ModbusRtuClientTest, BuildsEnableAutoCalibrationRequest) {
  const auto frame = ModbusRtuClient::build_write_multiple_registers_request(
      1, gripper_protocol::kAutoCalibrationRegister,
      {gripper_protocol::kAutoCalibrationEnableValue});

  const std::vector<uint8_t> expected = {0x01, 0x10, 0x9C, 0x9A, 0x00, 0x01,
                                         0x02, 0x00, 0x03, 0xA7, 0xA2};
  EXPECT_EQ(expected, frame);
}

TEST(ModbusRtuClientTest, ParsesReadHoldingRegistersResponse) {
  std::vector<uint8_t> frame = {0x01, 0x03, 0x04, 0x00, 0x2A, 0x00, 0x64};
  ModbusRtuClient::append_crc(frame);

  const auto values =
      ModbusRtuClient::parse_read_holding_registers_response(frame, 1, 2);

  ASSERT_EQ(values.size(), 2U);
  EXPECT_EQ(values[0], 42);
  EXPECT_EQ(values[1], 100);
}

TEST(ModbusRtuClientTest, RejectsBadCrc) {
  const std::vector<uint8_t> frame = {0x01, 0x03, 0x02, 0x00, 0x2A, 0x00, 0x00};

  EXPECT_THROW(ModbusRtuClient::validate_crc(frame), std::runtime_error);
}

TEST(ModbusRtuClientTest, ValidatesWriteEchoResponses) {
  std::vector<uint8_t> write_single = {0x01, 0x06, 0x9C, 0x9A, 0x00, 0x03};
  ModbusRtuClient::append_crc(write_single);
  EXPECT_NO_THROW(ModbusRtuClient::validate_write_registers_response(
      write_single, 1, 0x06, 40090, 3));

  std::vector<uint8_t> write_multiple = {0x01, 0x10, 0x9C, 0x40, 0x00, 0x01};
  ModbusRtuClient::append_crc(write_multiple);
  EXPECT_NO_THROW(ModbusRtuClient::validate_write_registers_response(
      write_multiple, 1, 0x10, 0x9C40, 1));
}

TEST(ModbusRtuClientTest, ReportsExceptionResponses) {
  std::vector<uint8_t> frame = {0x01, 0x83, 0x02};
  ModbusRtuClient::append_crc(frame);

  EXPECT_THROW(ModbusRtuClient::throw_if_exception_response(frame),
               std::runtime_error);
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
