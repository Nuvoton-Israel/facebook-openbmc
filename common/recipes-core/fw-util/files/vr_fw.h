#ifndef _VR_FW_H_
#define _VR_FW_H_

#include "fw-util.h"

class VrComponent : public Component {
  std::string dev_name;
  public:
    VrComponent(const std::string& fru, const std::string& comp, const std::string& name)
      : Component(fru, comp), dev_name(name) {}
    int update(const std::string& /*image*/) override;
    int fupdate(const std::string& /*image*/) override;
    int get_version(json& j) override;
    const std::string &get_dev_name(void) const { return dev_name; }
  protected:
    virtual int _update(const std::string& /*image*/, bool /*force*/) final;
};

#endif
