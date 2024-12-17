#include "fw-util.h"
#include <sstream>
#include <syslog.h>
#include <unistd.h>
#include <openbmc/pal.h>
#include <openbmc/aries_common.h>
#include <openbmc/libgpio.h>
#include "vr_fw.h"
#include <openbmc/kv.h>

const char * RST_PESET = "RST_PERST_CPUx_SWB_N";

class RetimerComponent : public Component {
  int _bus = 0;
  int addr = 0x24;

  public:
    RetimerComponent(
      const std::string& fru,
      const std::string& comp,
      const int bus)
      : Component(fru, comp), _bus(bus) {}

    int update(const std::string& image) {
      int ret = -1, lock = -1;
      const std::string& comp = this->component();
      const std::string& fru  = this->fru();

      if(gpio_get_value_by_shadow(RST_PESET) != GPIO_VALUE_HIGH) {
         return FW_STATUS_FAILURE;
      }

      if ((lock = mb_retimer_lock()) < 0) {
        syslog(LOG_WARNING, "%s: mb_retimer_lock failed", __func__);
        return FW_STATUS_FAILURE;
      }
      ret = AriestFwUpdate(_bus, addr, image.c_str());
      mb_retimer_unlock(lock);

      if (ret) {
        syslog(LOG_CRIT, "%s component %s: upgrade failed", fru.c_str(), comp.c_str());
        return FW_STATUS_FAILURE;
      }
      std::cout << "To active the upgrade of Retimer, please perform 'power-util mb cycle'" << std::endl;
      return FW_STATUS_SUCCESS;
    }

    int get_version(json& j) {
      int ret = -1, lock = -1;
      uint16_t ver[3] = {0};

      if(gpio_get_value_by_shadow(RST_PESET) != GPIO_VALUE_HIGH) {
          j["VERSION"] = "NA";
          return FW_STATUS_FAILURE;
      }

      if ((lock = mb_retimer_lock()) < 0) {
        syslog(LOG_WARNING, "%s: mb_retimer_lock failed", __func__);
        return FW_STATUS_FAILURE;
      }
      ret = AriesGetFwVersion(_bus, addr, ver);
      mb_retimer_unlock(lock);

      if (ret) {
        j["VERSION"] = "NA";
        return FW_STATUS_FAILURE;
      }

      std::stringstream ss;
      ss << +ver[0] << '.'
         << +ver[1] << '.'
         << +ver[2];
      j["VERSION"] = ss.str();

      return FW_STATUS_SUCCESS;
    }
};

// rev_id[2:0] | ID2 | ID1 | ID0 |
// 000         |  0  |  0  |  0  | EVT  (no retimer)
// 001         |  0  |  0  |  1  | DVT1 (8 retimer)
// 010         |  0  |  1  |  0  | DVT2 (2 retimer)
// 011         |  0  |  1  |  1  | PVT  (TBD)

class RetimerSysConfig {
  char value[MAX_VALUE_LEN] = {0};
  uint8_t mb_rev = 0;
  public:
    RetimerSysConfig() {

      if (kv_get("mb_rev", value, NULL, 0) == 0) {
        mb_rev = atoi(value);
      } else {
        std::cout << "Get MB REV ID Failed" << std:: endl;
        return;
      }

      switch (mb_rev) {
        case 0: { // No Retimer
          break;
        }
        case 2: { // 2 Retimer
          static RetimerComponent rt0_comp("mb", "retimer0", I2C_BUS_60);
          static RetimerComponent rt4_comp("mb", "retimer4", I2C_BUS_64);
          break;
        }
        default: { // 8 Retimer
          static RetimerComponent rt0_comp("mb", "retimer0", I2C_BUS_60);
          static RetimerComponent rt1_comp("mb", "retimer1", I2C_BUS_61);
          static RetimerComponent rt2_comp("mb", "retimer2", I2C_BUS_62);
          static RetimerComponent rt3_comp("mb", "retimer3", I2C_BUS_63);
          static RetimerComponent rt4_comp("mb", "retimer4", I2C_BUS_64);
          static RetimerComponent rt5_comp("mb", "retimer5", I2C_BUS_65);
          static RetimerComponent rt6_comp("mb", "retimer6", I2C_BUS_66);
          static RetimerComponent rt7_comp("mb", "retimer7", I2C_BUS_67);
          static VrComponent rt_vr0("mb", "cpu0_rt_p0v9", "VR_CPU0_RETIMER_P0V9");
          static VrComponent rt_vr1("mb", "cpu1_rt_p0v9", "VR_CPU1_RETIMER_P0V9");
          break;
        }
      }
    }
};

RetimerSysConfig rtConfig;
