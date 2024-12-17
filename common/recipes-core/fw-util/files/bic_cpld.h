#ifndef BIC_CPLD_H_
#define BIC_CPLD_H_
#include "fw-util.h"
#include "server.h"


class CpldComponent : public Component {
  uint8_t slot_id = 0;
  Server server;
  public:
    CpldComponent(const std::string& fru, const std::string& comp, uint8_t _slot_id)
      : Component(fru, comp), slot_id(_slot_id), server(_slot_id, fru) {}
    int update(const std::string& image) override;
    int get_version(json& j) override;
};

#endif
