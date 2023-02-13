#include "swb_common.hpp"

SwbBicFwComponent bic("swb", "bic", 3, 0x0A, BIC_COMP);
SwbBicFwRecoveryComponent bic_recovery("swb", "bic_recovery", 3, 0x0A, BIC_COMP);
SwbPexFwComponent swb_pex0("swb", "pex0", 3, 0x0A, PEX0_COMP, 0);
SwbPexFwComponent swb_pex1("swb", "pex1", 3, 0x0A, PEX1_COMP, 1);
SwbPexFwComponent swb_pex2("swb", "pex2", 3, 0x0A, PEX2_COMP, 2);
SwbPexFwComponent swb_pex3("swb", "pex3", 3, 0x0A, PEX3_COMP, 3);

PLDMNicComponent swb_nic0("swb", "swb_nic0", "SWB_NIC0", 0x10, 3);
PLDMNicComponent swb_nic1("swb", "swb_nic1", "SWB_NIC1", 0x11, 3);
PLDMNicComponent swb_nic2("swb", "swb_nic2", "SWB_NIC2", 0x12, 3);
PLDMNicComponent swb_nic3("swb", "swb_nic3", "SWB_NIC3", 0x13, 3);
PLDMNicComponent swb_nic4("swb", "swb_nic4", "SWB_NIC4", 0x14, 3);
PLDMNicComponent swb_nic5("swb", "swb_nic5", "SWB_NIC5", 0x15, 3);
PLDMNicComponent swb_nic6("swb", "swb_nic6", "SWB_NIC6", 0x16, 3);
PLDMNicComponent swb_nic7("swb", "swb_nic7", "SWB_NIC7", 0x17, 3);