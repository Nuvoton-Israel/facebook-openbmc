#include <cstdio>
#include <cstring>
#include <syslog.h>
#include <gpiod.h>
#include <openbmc/obmc-i2c.h>
#include <openbmc/pal.h>
#include <openbmc/cpld.h>
#include <openbmc/kv.h>
#include "fw-util.h"

using namespace std;

// According to QSYS setting in FPGA project

// on-chip Flash IP
#define ON_CHIP_FLASH_IP_CSR_BASE        (0x00200020)
#define ON_CHIP_FLASH_IP_CSR_STATUS_REG  (ON_CHIP_FLASH_IP_CSR_BASE + 0x0)
#define ON_CHIP_FLASH_IP_CSR_CTRL_REG    (ON_CHIP_FLASH_IP_CSR_BASE + 0x4)

#define ON_CHIP_FLASH_IP_DATA_REG        (0x00000000)
// Dual-boot IP
#define DUAL_BOOT_IP_BASE                (0x00100000)

#define CFM0_10M16_START_ADDR            (0x0004A000)
#define CFM0_10M16_END_ADDR              (0x0008BFFF)
#define CFM1_10M16_START_ADDR            (0x00008000)
#define CFM1_10M16_END_ADDR              (0x00049FFF)

enum {
  CFM_IMAGE_NONE = 0,
  CFM_IMAGE_1,
  CFM_IMAGE_2,
};

class CpldComponent : public Component {
  private:
    uint8_t pld_type;
    void* attr;
    int _update(const char* path);

  public:
    CpldComponent(
      const std::string& fru, const std::string& comp, uint8_t type, void* i2c_attr)
      : Component(fru, comp), pld_type(type), attr(i2c_attr) {}
  int update(const std::string& image) override;
  int fupdate(const std::string& image) override;
  int get_version(json& j) override;
};

int CpldComponent::_update(const char* path) {
  int ret = -1;

  if (cpld_intf_open(pld_type, INTF_I2C, attr)) {
    cerr << "Cannot open i2c!" << endl;
    goto error_exit;
  }

  ret = cpld_program((char *)path, NULL, false);
  cpld_intf_close();
  if (ret) {
    cerr << "Error Occur at updating CPLD FW!" << endl;
    goto error_exit;
  }

error_exit:
  return ret;
}

int CpldComponent::update(const string& image) {
  return _update(image.c_str());
}

int CpldComponent::fupdate(const string& image) {
  return _update(image.c_str());
}

int CpldComponent::get_version(json& j) {
  int ret = -1;
  uint8_t ver[4];
  char strbuf[MAX_VALUE_LEN];
  string comp = this->component();
  string fru  = this->fru();
  transform(comp.begin(), comp.end(),comp.begin(), ::toupper);
  transform(fru.begin(), fru.end(),fru.begin(), ::toupper);
  j["PRETTY_COMPONENT"] = comp;

  if (!cpld_intf_open(pld_type, INTF_I2C, attr)) {
    ret = cpld_get_ver((uint32_t *)ver);
    cpld_intf_close();
  }

  if (ret) {
    sprintf(strbuf, "NA");
  } else {
    sprintf(strbuf, "%02X%02X%02X%02X", ver[0], ver[1], ver[2], ver[3]);
    kv_set(_component.c_str(), strbuf, 0, 0);
  }
  j["VERSION"] = string(strbuf);
  return FW_STATUS_SUCCESS;
}

class GpioControlCpld : public CpldComponent {
public:
  GpioControlCpld(const std::string& fru, const std::string& comp, uint8_t type, void* i2c_attr, const std::string& linename, bool high_active)
    : CpldComponent(fru, comp, type, i2c_attr), line(nullptr), is_high_active(high_active) {
    line = gpiod_line_find(linename.c_str());
    if (!line) {
      std::cerr << "Failed to find GPIO line: " << linename << std::endl;
      throw std::runtime_error("GPIO line not found");
    }

    if (gpiod_line_request_output(line, "fw-util", is_high_active ? 1 : 0) != 0) {
      std::cerr << "Failed to request GPIO line as output" << std::endl;
      gpiod_line_close_chip(line);
      throw std::runtime_error("Failed to request GPIO line as output");
    }
    msleep(100);
  }

  ~GpioControlCpld() {
    if (line) {
      gpiod_line_set_value(line, is_high_active ? 0 : 1);
      gpiod_line_release(line);
      gpiod_line_close_chip(line);
    }
  }

private:
  gpiod_line* line;
  bool is_high_active;
};

static altera_max10_attr_t scm_cpld_attr = {
  7, 0x31, CFM_IMAGE_1, CFM0_10M16_START_ADDR, CFM0_10M16_END_ADDR,
  ON_CHIP_FLASH_IP_CSR_BASE, ON_CHIP_FLASH_IP_DATA_REG, DUAL_BOOT_IP_BASE,
  I2C_BIG_ENDIAN
};

//CPLD Component
GpioControlCpld scm_cpld("scm", "cpld", MAX10_10M16, &scm_cpld_attr, "USBDBG_IPMI_EN_L", true);
