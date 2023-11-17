#include "platform.hpp"

namespace pldm
{
namespace responder
{
namespace platform
{

std::string get_sensor_name(uint16_t id)
{
  static const std::map<uint16_t, const std::string> sensor_map = {
    {0x01, "CB_SENSOR_ACCL_1"},
    {0x02, "CB_SENSOR_ACCL_2"},
    {0x03, "CB_SENSOR_ACCL_3"},
    {0x04, "CB_SENSOR_ACCL_4"},
    {0x05, "CB_SENSOR_ACCL_5"},
    {0x06, "CB_SENSOR_ACCL_6"},
    {0x07, "CB_SENSOR_ACCL_7"},
    {0x08, "CB_SENSOR_ACCL_8"},
    {0x09, "CB_SENSOR_ACCL_9"},
    {0x0A, "CB_SENSOR_ACCL_10"},
    {0x0B, "CB_SENSOR_ACCL_11"},
    {0x0C, "CB_SENSOR_ACCL_12"},
    {0x0D, "CB_SENSOR_ACCL_POWER_CABLE_1"},
    {0x0E, "CB_SENSOR_ACCL_POWER_CABLE_2"},
    {0x0F, "CB_SENSOR_ACCL_POWER_CABLE_3"},
    {0x10, "CB_SENSOR_ACCL_POWER_CABLE_4"},
    {0x11, "CB_SENSOR_ACCL_POWER_CABLE_5"},
    {0x12, "CB_SENSOR_ACCL_POWER_CABLE_6"},
    {0x13, "CB_SENSOR_ACCL_POWER_CABLE_7"},
    {0x14, "CB_SENSOR_ACCL_POWER_CABLE_8"},
    {0x15, "CB_SENSOR_ACCL_POWER_CABLE_9"},
    {0x16, "CB_SENSOR_ACCL_POWER_CABLE_10"},
    {0x17, "CB_SENSOR_ACCL_POWER_CABLE_11"},
    {0x18, "CB_SENSOR_ACCL_POWER_CABLE_12"},
    {0x19, "CB_SENSOR_FIO"},
  };

  if (sensor_map.find(id) !=sensor_map.end())
    return sensor_map.at(id);
  else
    return "Unknown";
}

std::string get_state_message(uint8_t offset, uint8_t state)
{
  static const std::vector <std::vector<std::string>> eventstate_map = {
    {"unknown", "present", "not present"},                  // sensorOffset = 0: presentence
    {"unknown", "normal",  "alert"},                        // sensorOffset = 1: status
    {"not power on yet", "power good",  "power good fail"}, // sensorOffset = 2: power status
  };

  if (offset >= eventstate_map.size())
    return "offset not supported";
  else if (state >= eventstate_map[offset].size())
    return "state not supported";
  else
    return eventstate_map[offset][state];
}

std::string get_device_type(uint8_t type)
{
  static const std::map<uint8_t, const std::string> device_map = {
    {0x01, "CB_P0V8_VDD_1"},
    {0x02, "CB_P0V8_VDD_2"},
    {0x03, "CB_POWER_BRICK"},
    {0x04, "CB_P1V25_MONITOR"},
    {0x05, "CB_P12V_ACCL1_MONITOR"},
    {0x06, "CB_P12V_ACCL2_MONITOR"},
    {0x07, "CB_P12V_ACCL3_MONITOR"},
    {0x08, "CB_P12V_ACCL4_MONITOR"},
    {0x09, "CB_P12V_ACCL5_MONITOR"},
    {0x0A, "CB_P12V_ACCL6_MONITOR"},
    {0x0B, "CB_P12V_ACCL7_MONITOR"},
    {0x0C, "CB_P12V_ACCL8_MONITOR"},
    {0x0D, "CB_P12V_ACCL9_MONITOR"},
    {0x0E, "CB_P12V_ACCL10_MONITOR"},
    {0x0F, "CB_P12V_ACCL11_MONITOR"},
    {0x10, "CB_P12V_ACCL12_MONITOR"},
    {0x11, "CB_PESW_0"},
    {0x12, "CB_PESW_1"},
  };

  if (device_map.find(type) != device_map.end())
    return device_map.at(type);
  else
    return "UNKNOWN_TYPE";
}

std::string get_board_info(uint8_t id)
{
  static const std::map<uint8_t, const std::string> board_map = {
    {0x00, "MAIN_SOURCE"},
    {0x01, "SECOND_SOURCE"},
  };

  if (board_map.find(id) != board_map.end())
    return board_map.at(id);
  else
    return "UNKNOWN_SOURCE";
}

std::string get_event_type(uint8_t type)
{
  static const std::map<uint8_t, const std::string> event_map = {
    {0x00, "UNUSE"},
    {0x01, "OVER_POWER"},
    {0x02, "OVER_VOLTAGE"},
    {0x03, "OVER_CURRENT"},
    {0x04, "UNDER_VOLTAGE"},
    {0x05, "OVER_TEMPERATURE"},
    // Switch CCR system error
    {0x06, "SYSTEM_ERROR"},
    {0x07, "PEX_FATAL_ERROR"},
    {0x08, "POR_BISR_TIMEOUT"},
    {0x09, "FLASH_SIGNATURE_FAIL"},
    {0x0A, "WATCHDOG_0_TIMEOUT_CPU_CORE_RESET"},
    {0x0B, "WATCHDOG_0_TIMEOUT_SYSTEM_RESET"},
    {0x0C, "WATCHDOG_1_TIMEOUT_CPU_CORE_RESET"},
    {0x0D, "WATCHDOG_1_TIMEOUT_SYSTEM_RESET"},
    {0x0E, "LOCAL_CPU_PARITY_ERROR"},
    {0x0F, "SECURE_BOOT_FAIL"},
    {0x10, "SBR_LOAD_FAIL"},
    {0x11, "STATION_0_FATAL_ERROR"},
    {0x12, "STATION_1_FATAL_ERROR"},
    {0x13, "STATION_2_FATAL_ERROR"},
    {0x14, "STATION_3_FATAL_ERROR"},
    {0x15, "STATION_4_FATAL_ERROR"},
    {0x16, "STATION_5_FATAL_ERROR"},
    {0x17, "STATION_6_FATAL_ERROR"},
    {0x18, "STATION_7_FATAL_ERROR"},
    {0x19, "STATION_8_FATAL_ERROR"},
    {0x1A, "PSB_STATION_FATAL_ERROR"},
  };

  if (event_map.find(type) != event_map.end())
    return event_map.at(type);
  else
    return "UNKNOWN_TYPE";
}


} // namespace platform
} // namespace responder
} // namespace pldm
