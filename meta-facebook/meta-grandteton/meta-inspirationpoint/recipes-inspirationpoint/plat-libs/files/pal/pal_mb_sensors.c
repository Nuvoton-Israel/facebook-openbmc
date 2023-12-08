#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <peci.h>
#include <linux/peci-ioctl.h>
#include <openbmc/kv.h>
#include <openbmc/libgpio.h>
#include <openbmc/obmc-i2c.h>
#include <openbmc/nm.h>
#include <openbmc/ipmb.h>
#include <openbmc/misc-utils.h>
#include <openbmc/obmc-sensors.h>
#include <openbmc/peci_sensors.h>
#include <openbmc/pmbus.h>
#include <esmi_mailbox.h>
#include "pal.h"
#include "pal_common.h"
#include <openbmc/kv.h>
#include <openbmc/aries_common.h>
#include <openbmc/plat.h>
#include <openbmc/dimm.h>
#include <openbmc/i3c_dev.h>
#include <time.h>

//#define DEBUG
#define GPIO_P3V_BAT_SCALED_EN    "BATTERY_DETECT"
#define GPIO_APML_MUX2_SEL        "FM_APML_MUX2_SEL_R"
#define HSC_12V_BUS               (2)

#define IIO_DEV_DIR(device, bus, addr, index) \
  "/sys/bus/i2c/drivers/"#device"/"#bus"-00"#addr"/iio:device"#index"/%s"
#define IIO_AIN_NAME       "in_voltage%d_raw"

#define MAX11617_DIR     IIO_DEV_DIR(max1363, 20, 35, 2)
#define MAX11617_RT_DIR  IIO_DEV_DIR(max1363, 26, 35, 3)

#define SCALING_FACTOR	0.25
#define CHANNEL_OF_DIMM_NUM  6

#define APML_SNR_START_INDEX  MB_SNR_DIMM_CPU0_A0_TEMP
#define DELAY_APML_MUX  100  // 100 ms
#define DIMM_TS0_SNR_DIR "/dev/bus/i3c/%d-e0000001%d"
#define DIMM_TS1_SNR_DIR "/dev/bus/i3c/%d-e0000003%d"
#define DIMM_PWR_SNR_DIR "/dev/bus/i3c/%d-e5010%d000"

uint8_t DIMM_SLOT_CNT = 0;
//static float InletCalibration = 0;

static int read_bat_val(uint8_t fru, uint8_t sensor_num, float *value);
static int read_mb_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu_pkg_pwr(uint8_t fru, uint8_t sensor_num, float *value);
static int read_hsc_iout(uint8_t fru, uint8_t sensor_num, float *value);
static int read_hsc_vin(uint8_t fru, uint8_t sensor_num, float *value);
static int read_hsc_pin(uint8_t fru, uint8_t sensor_num, float *value);
static int read_hsc_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_hsc_peak_pin(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu0_dimm_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu1_dimm_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu0_dimm_power(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu1_dimm_power(uint8_t fru, uint8_t sensor_num, float *value);
static int read_frb3(uint8_t fru, uint8_t sensor_num, float *value);
static int read_e1s_power(uint8_t fru, uint8_t sensor_num, float *value);
static int read_e1s_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_retimer_temp(uint8_t fru, uint8_t sensor_num, float *value);
static int read_cpu_dimm_state(uint8_t fru, uint8_t sensor_num, float *value);
static int read_retimer_health(uint8_t fru, uint8_t sensor_num, float *value);

bool pal_bios_completed(uint8_t fru);

static bool g_has_mux = true;

const uint8_t mb_sensor_list[] = {
  MB_SNR_INLET_TEMP_R,
  MB_SNR_INLET_TEMP_L,
  MB_SNR_OUTLET_TEMP_R,
  MB_SNR_OUTLET_TEMP_L,
  MB_SNR_DIMM_CPU0_A0_TEMP,
  MB_SNR_DIMM_CPU0_A1_TEMP,
  MB_SNR_DIMM_CPU0_A2_TEMP,
  MB_SNR_DIMM_CPU0_A3_TEMP,
  MB_SNR_DIMM_CPU0_A4_TEMP,
  MB_SNR_DIMM_CPU0_A5_TEMP,
  MB_SNR_DIMM_CPU0_A6_TEMP,
  MB_SNR_DIMM_CPU0_A7_TEMP,
  MB_SNR_DIMM_CPU0_A8_TEMP,
  MB_SNR_DIMM_CPU0_A9_TEMP,
  MB_SNR_DIMM_CPU0_A10_TEMP,
  MB_SNR_DIMM_CPU0_A11_TEMP,
  MB_SNR_DIMM_CPU1_B0_TEMP,
  MB_SNR_DIMM_CPU1_B1_TEMP,
  MB_SNR_DIMM_CPU1_B2_TEMP,
  MB_SNR_DIMM_CPU1_B3_TEMP,
  MB_SNR_DIMM_CPU1_B4_TEMP,
  MB_SNR_DIMM_CPU1_B5_TEMP,
  MB_SNR_DIMM_CPU1_B6_TEMP,
  MB_SNR_DIMM_CPU1_B7_TEMP,
  MB_SNR_DIMM_CPU1_B8_TEMP,
  MB_SNR_DIMM_CPU1_B9_TEMP,
  MB_SNR_DIMM_CPU1_B10_TEMP,
  MB_SNR_DIMM_CPU1_B11_TEMP,
  MB_SNR_VR_CPU0_VCCIN_VOLT,
  MB_SNR_VR_CPU0_VCCIN_TEMP,
  MB_SNR_VR_CPU0_VCCIN_CURR,
  MB_SNR_VR_CPU0_VCCIN_POWER,
  MB_SNR_VR_CPU0_VCCFA_FIVRA_VOLT,
  MB_SNR_VR_CPU0_VCCFA_FIVRA_TEMP,
  MB_SNR_VR_CPU0_VCCFA_FIVRA_CURR,
  MB_SNR_VR_CPU0_VCCFA_FIVRA_POWER,
  MB_SNR_VR_CPU0_VCCIN_FAON_VOLT,
  MB_SNR_VR_CPU0_VCCIN_FAON_TEMP,
  MB_SNR_VR_CPU0_VCCIN_FAON_CURR,
  MB_SNR_VR_CPU0_VCCIN_FAON_POWER,
  MB_SNR_VR_CPU0_VCCFA_VOLT,
  MB_SNR_VR_CPU0_VCCFA_TEMP,
  MB_SNR_VR_CPU0_VCCFA_CURR,
  MB_SNR_VR_CPU0_VCCFA_POWER,
  MB_SNR_VR_CPU0_VCCD_HV_VOLT,
  MB_SNR_VR_CPU0_VCCD_HV_TEMP,
  MB_SNR_VR_CPU0_VCCD_HV_CURR,
  MB_SNR_VR_CPU0_VCCD_HV_POWER,
  MB_SNR_VR_CPU1_VCCIN_VOLT,
  MB_SNR_VR_CPU1_VCCIN_TEMP,
  MB_SNR_VR_CPU1_VCCIN_CURR,
  MB_SNR_VR_CPU1_VCCIN_POWER,
  MB_SNR_VR_CPU1_VCCFA_FIVRA_VOLT,
  MB_SNR_VR_CPU1_VCCFA_FIVRA_TEMP,
  MB_SNR_VR_CPU1_VCCFA_FIVRA_CURR,
  MB_SNR_VR_CPU1_VCCFA_FIVRA_POWER,
  MB_SNR_VR_CPU1_VCCIN_FAON_VOLT,
  MB_SNR_VR_CPU1_VCCIN_FAON_TEMP,
  MB_SNR_VR_CPU1_VCCIN_FAON_CURR,
  MB_SNR_VR_CPU1_VCCIN_FAON_POWER,
  MB_SNR_VR_CPU1_VCCFA_VOLT,
  MB_SNR_VR_CPU1_VCCFA_TEMP,
  MB_SNR_VR_CPU1_VCCFA_CURR,
  MB_SNR_VR_CPU1_VCCFA_POWER,
  MB_SNR_VR_CPU1_VCCD_HV_VOLT,
  MB_SNR_VR_CPU1_VCCD_HV_TEMP,
  MB_SNR_VR_CPU1_VCCD_HV_CURR,
  MB_SNR_VR_CPU1_VCCD_HV_POWER,
  MB_SNR_CPU0_TEMP,
  MB_SNR_CPU1_TEMP,
  MB_SNR_CPU0_PKG_POWER,
  MB_SNR_CPU1_PKG_POWER,
  MB_SNR_P3V_BAT,
  MB_SNR_E1S_P3V3_VOUT,
  MB_SNR_E1S_P12V_IOUT,
  MB_SNR_E1S_P12V_POUT,
  MB_SNR_E1S_TEMP,
  MB_SNR_ADC128_P12V_AUX,
  NB_SNR_ADC128_P5V,
  MB_SNR_ADC128_P3V3,
  MB_SNR_ADC128_P3V3_AUX,
  MB_SNR_DIMM_CPU0_A0_POWER,
  MB_SNR_DIMM_CPU0_C0_POWER,
  MB_SNR_DIMM_CPU0_A1_POWER,
  MB_SNR_DIMM_CPU0_C1_POWER,
  MB_SNR_DIMM_CPU0_A2_POWER,
  MB_SNR_DIMM_CPU0_C2_POWER,
  MB_SNR_DIMM_CPU0_A3_POWER,
  MB_SNR_DIMM_CPU0_C3_POWER,
  MB_SNR_DIMM_CPU0_A4_POWER,
  MB_SNR_DIMM_CPU0_C4_POWER,
  MB_SNR_DIMM_CPU0_A5_POWER,
  MB_SNR_DIMM_CPU0_C5_POWER,
  MB_SNR_DIMM_CPU1_B0_POWER,
  MB_SNR_DIMM_CPU1_D0_POWER,
  MB_SNR_DIMM_CPU1_B1_POWER,
  MB_SNR_DIMM_CPU1_D1_POWER,
  MB_SNR_DIMM_CPU1_B2_POWER,
  MB_SNR_DIMM_CPU1_D2_POWER,
  MB_SNR_DIMM_CPU1_B3_POWER,
  MB_SNR_DIMM_CPU1_D3_POWER,
  MB_SNR_DIMM_CPU1_B4_POWER,
  MB_SNR_DIMM_CPU1_D4_POWER,
  MB_SNR_DIMM_CPU1_B5_POWER,
  MB_SNR_DIMM_CPU1_D5_POWER,
  MB_SNR_RETIMER0_TEMP,
  MB_SNR_RETIMER1_TEMP,
  MB_SNR_RETIMER2_TEMP,
  MB_SNR_RETIMER3_TEMP,
  MB_SNR_RETIMER4_TEMP,
  MB_SNR_RETIMER5_TEMP,
  MB_SNR_RETIMER6_TEMP,
  MB_SNR_RETIMER7_TEMP,
  MB_SNR_P5V_AUX_ADC,
  MB_SNR_P1V8_AUX_ADC,
  MB_SNR_P1V2_AUX_ADC,
  MB_SNR_P1V8_CPU0_RT_1D_ADC,
  MB_SNR_P1V8_CPU1_RT_1D_ADC,
  MB_SNR_PVDD_33_S5_ADC,
  MB_SNR_PVDD18_S5_P0_ADC,
  MB_SNR_PVDD18_S5_P1_ADC,
  MB_SNR_VR_RT_P0V9_VDD1_TEMP,
  MB_SNR_VR_RT_P0V9_VDD1_VOUT,
  MB_SNR_VR_RT_P0V9_VDD1_IOUT,
  MB_SNR_VR_RT_P0V9_VDD1_POUT,
  MB_SNR_VR_RT_P0V9_VDD2_TEMP,
  MB_SNR_VR_RT_P0V9_VDD2_VOUT,
  MB_SNR_VR_RT_P0V9_VDD2_IOUT,
  MB_SNR_VR_RT_P0V9_VDD2_POUT,
  MB_SNR_HSC_VIN,
  MB_SNR_HSC_IOUT,
  MB_SNR_HSC_PIN,
  MB_SNR_HSC_TEMP,
};

// List of MB discrete sensors to be monitored
const uint8_t mb_discrete_sensor_list[] = {
//  MB_SENSOR_POWER_FAIL,
//  MB_SENSOR_MEMORY_LOOP_FAIL,
  MB_SNR_PROCESSOR_FAIL,
  MB_SNR_RETIMER0_HEALTHY,
  MB_SNR_RETIMER1_HEALTHY,
  MB_SNR_RETIMER2_HEALTHY,
  MB_SNR_RETIMER3_HEALTHY,
  MB_SNR_RETIMER4_HEALTHY,
  MB_SNR_RETIMER5_HEALTHY,
  MB_SNR_RETIMER6_HEALTHY,
  MB_SNR_RETIMER7_HEALTHY,
  MB_SNR_DIMM_CPU0_A0_STATE,
  MB_SNR_DIMM_CPU0_C0_STATE,
  MB_SNR_DIMM_CPU0_A1_STATE,
  MB_SNR_DIMM_CPU0_C1_STATE,
  MB_SNR_DIMM_CPU0_A2_STATE,
  MB_SNR_DIMM_CPU0_C2_STATE,
  MB_SNR_DIMM_CPU0_A3_STATE,
  MB_SNR_DIMM_CPU0_C3_STATE,
  MB_SNR_DIMM_CPU0_A4_STATE,
  MB_SNR_DIMM_CPU0_C4_STATE,
  MB_SNR_DIMM_CPU0_A5_STATE,
  MB_SNR_DIMM_CPU0_C5_STATE,
  MB_SNR_DIMM_CPU1_B0_STATE,
  MB_SNR_DIMM_CPU1_D0_STATE,
  MB_SNR_DIMM_CPU1_B1_STATE,
  MB_SNR_DIMM_CPU1_D1_STATE,
  MB_SNR_DIMM_CPU1_B2_STATE,
  MB_SNR_DIMM_CPU1_D2_STATE,
  MB_SNR_DIMM_CPU1_B3_STATE,
  MB_SNR_DIMM_CPU1_D3_STATE,
  MB_SNR_DIMM_CPU1_B4_STATE,
  MB_SNR_DIMM_CPU1_D4_STATE,
  MB_SNR_DIMM_CPU1_B5_STATE,
  MB_SNR_DIMM_CPU1_D5_STATE,
};

PAL_DPM_DEV_INFO dpm_info_list[] = {
  {DPM_0, I2C_BUS_34, 0x82, 0.004, 0, 0 },
  {DPM_1, I2C_BUS_34, 0x84, 0.004, 0, 0 },
  {DPM_2, I2C_BUS_34, 0x86, 0.004, 0, 0 },
  {DPM_3, I2C_BUS_34, 0x88, 0.004, 0, 0 },
  {DPM_4, I2C_BUS_34, 0x8A, 0.004, 0, 0 },
};

//E1S
PAL_I2C_BUS_INFO e1s_info_list[] = {
  {E1S_0, I2C_BUS_31, 0xD4},
};

//{SensorName, ID, FUNCTION, PWR_STATUS, {UCR, UNC, UNR, LCR, LNC, LNR, Pos, Neg}
PAL_SENSOR_MAP mb_sensor_map[] = {
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x00
  {"E1S_P3V3_VOLT", DPM_0, read_dpm_vout, false, {3.465, 0, 0, 3.135, 0, 0, 0, 0}, VOLT}, //0x01
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x02
  {"E1S_P12V_CURR", ADC_CH7, read_iic_adc_val, false, {0, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0x03
  {"E1S_P12V_PWR", E1S_0, read_e1s_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x04
  {"E1S_TEMP", E1S_0, read_e1s_temp, false, {70.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x05
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x06
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x07

  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x08
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x09
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x0A
  {"HSC_VOLT", HSC_ID0, read_hsc_vin,  true, {14.33, 0, 0, 10.09, 0, 0, 0, 0}, VOLT}, //0x0B
  {"HSC_CURR", HSC_ID0, read_hsc_iout, true, {240.0, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0x0C
  {"HSC_PWR", HSC_ID0, read_hsc_pin,  true, {3439.0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x0D
  {"HSC_TEMP", HSC_ID0, read_hsc_temp, true, {120.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x0E
  {"HSC_PEAK_PIN", HSC_ID0, read_hsc_peak_pin, true, {3782.0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x0F

  {"OUTLET_L_TEMP", TEMP_OUTLET_L, read_mb_temp, true, {75.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x10
  {"INLET_L_TEMP", TEMP_INLET_L, read_mb_temp, true, {55.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x11
  {"OUTLET_R_TEMP", TEMP_OUTLET_R, read_mb_temp, true, {75.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x12
  {"INLET_R_TEMP", TEMP_INLET_R, read_mb_temp, true, {55.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x13
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x14
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x15
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x16
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x17

  {"CPU0_TEMP", CPU_ID0, read_cpu_temp, false, {90.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x18
  {"CPU1_TEMP", CPU_ID1, read_cpu_temp, false, {90.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x19
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x1A
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x1B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x1C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x1D
  {"CPU0_PKG_PWR", CPU_ID0, read_cpu_pkg_pwr, false, {420.0, 0, 0, 0.0, 0, 0, 0, 0}, POWER}, //0x1E
  {"CPU1_PKG_PWR", CPU_ID1, read_cpu_pkg_pwr, false, {420.0, 0, 0, 0.0, 0, 0, 0, 0}, POWER}, //0x1F

  {"CPU0_DIMM_A0_A1_TEMP", DIMM_CRPA, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x20
  {"CPU0_DIMM_A2_A3_TEMP", DIMM_CRPB, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x21
  {"CPU0_DIMM_A4_A5_TEMP", DIMM_CRPC, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x22
  {"CPU0_DIMM_A6_A7_TEMP", DIMM_CRPD, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x23
  {"CPU0_DIMM_A8_A9_TEMP", DIMM_CRPE, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x24
  {"CPU0_DIMM_A10_A11_TEMP", DIMM_CRPF, read_cpu0_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x25
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x26
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x27

  {"CPU1_DIMM_B0_B1_TEMP", DIMM_CRPA, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x28
  {"CPU1_DIMM_B2_B3_TEMP", DIMM_CRPB, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x29
  {"CPU1_DIMM_B4_B5_TEMP", DIMM_CRPC, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x2A
  {"CPU1_DIMM_B6_B7_TEMP", DIMM_CRPD, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x2B
  {"CPU1_DIMM_B8_B9_TEMP", DIMM_CRPE, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x2C
  {"CPU1_DIMM_B10_B11_TEMP", DIMM_CRPF, read_cpu1_dimm_temp, false, {85.0, 0, 0, 0, 0, 0, 0, 0}, TEMP}, //0x2D
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x2E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x2F

  {"VR_CPU0_VCORE0_VOLT", VR_ID0, read_vr_vout, false, {1.70, 0, 0, 0.33, 0, 0, 0, 0}, VOLT}, //0x30
  {"VR_CPU0_VCORE0_TEMP", VR_ID0, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x31
  {"VR_CPU0_VCORE0_CURR", VR_ID0, read_vr_iout, false, {240, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X32
  {"VR_CPU0_VCORE0_PWR",  VR_ID0, read_vr_pout, false, {410, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x33
  {"VR_CPU0_SOC_VOLT", VR_ID1, read_vr_vout, false, {1.35, 0, 0, 0.59, 0, 0, 0, 0}, VOLT}, //0x34
  {"VR_CPU0_SOC_TEMP", VR_ID1, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x35
  {"VR_CPU0_SOC_CURR", VR_ID1, read_vr_iout, false, {140, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X36
  {"VR_CPU0_SOC_PWR",  VR_ID1, read_vr_pout, false, {192, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x37
  {"VR_CPU0_VCORE1_VOLT",  VR_ID2, read_vr_vout, false, {1.70, 0, 0, 0.33, 0, 0, 0, 0}, VOLT}, //0x38
  {"VR_CPU0_VCORE1_TEMP",  VR_ID2, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x39
  {"VR_CPU0_VCORE1_CURR",  VR_ID2, read_vr_iout, false, {240, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X3A
  {"VR_CPU0_VCORE1_PWR",   VR_ID2, read_vr_pout, false, {410.0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x3B
  {"VR_CPU0_PVDDIO_VOLT", VR_ID3, read_vr_vout, false, {1.28, 0, 0, 0.81, 0, 0, 0, 0}, VOLT}, //0x3C
  {"VR_CPU0_PVDDIO_TEMP", VR_ID3, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x3D
  {"VR_CPU0_PVDDIO_CURR", VR_ID3, read_vr_iout, false, {150, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X3E
  {"VR_CPU0_PVDDIO_PWR",  VR_ID3, read_vr_pout, false, {195, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x3F
  {"VR_CPU0_PVDD11_VOLT", VR_ID4, read_vr_vout, false, {1.17, 0, 0, 1.03, 0, 0, 0, 0}, VOLT}, //0x40
  {"VR_CPU0_PVDD11_TEMP", VR_ID4, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x41
  {"VR_CPU0_PVDD11_CURR", VR_ID4, read_vr_iout, false, {85, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X42
  {"VR_CPU0_PVDD11_PWR", VR_ID4, read_vr_pout, false, {99, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x43
  {"P12V_AUX_IN0_VOLT", ADC_CH0, read_iic_adc_val, true, {13.2, 0, 0, 10.8, 0, 0, 0, 0}, VOLT}, //0x44
  {"P5V_IN3_VOLT",      ADC_CH3, read_iic_adc_val, false, {5.25, 0, 0, 4.75, 0, 0, 0, 0}, VOLT}, //0x45
  {"P3V3_IN4_VOLT",     ADC_CH4, read_iic_adc_val, false, {3.465, 0, 0, 3.135, 0, 0, 0, 0}, VOLT}, //0x46
  {"P3V3_AUX_IN5_VOLT", ADC_CH5, read_iic_adc_val, true, {3.465, 0, 0, 3.135, 0, 0, 0, 0}, VOLT}, //0x47

  {"VR_CPU1_VCORE0_VOLT", VR_ID5, read_vr_vout, false, {1.70, 0, 0, 0.33, 0, 0, 0, 0}, VOLT}, //0x48
  {"VR_CPU1_VCORE0_TEMP", VR_ID5, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x49
  {"VR_CPU1_VCORE0_CURR", VR_ID5, read_vr_iout, false, {240, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X4A
  {"VR_CPU1_VCORE0_PWR", VR_ID5, read_vr_pout, false, {410, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x4B
  {"VR_CPU1_SOC_VOLT", VR_ID6, read_vr_vout, false, {1.35, 0, 0, 0.59, 0, 0, 0, 0}, VOLT}, //0x4C
  {"VR_CPU1_SOC_TEMP", VR_ID6, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x4D
  {"VR_CPU1_SOC_CURR", VR_ID6, read_vr_iout, false, {140, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X4E
  {"VR_CPU1_SOC_PWR", VR_ID6, read_vr_pout, false, {192, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x4F
  {"VR_CPU1_VCORE1_VOLT", VR_ID7, read_vr_vout, false, {1.70, 0, 0, 0.33, 0, 0, 0, 0}, VOLT}, //0x50
  {"VR_CPU1_VCORE1_TEMP", VR_ID7, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x51
  {"VR_CPU1_VCORE1_CURR", VR_ID7, read_vr_iout, false, {240, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X52
  {"VR_CPU1_VCORE1_PWR", VR_ID7, read_vr_pout, false, {410, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x53
  {"VR_CPU1_PVDDIO_VOLT", VR_ID8, read_vr_vout, false, {1.28, 0, 0, 0.81, 0, 0, 0, 0}, VOLT}, //0x54
  {"VR_CPU1_PVDDIO_TEMP", VR_ID8, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x55
  {"VR_CPU1_PVDDIO_CURR", VR_ID8, read_vr_iout, false, {150, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X56
  {"VR_CPU1_PVDDIO_PWR", VR_ID8, read_vr_pout, false, {195, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x57
  {"VR_CPU1_PVDD11_VOLT", VR_ID9, read_vr_vout, false, {1.17, 0, 0, 1.03, 0, 0, 0, 0}, VOLT}, //0x58
  {"VR_CPU1_PVDD11_TEMP", VR_ID9, read_vr_temp, false, {105.0, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x59
  {"VR_CPU1_PVDD11_CURR", VR_ID9, read_vr_iout, false, {85, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0X5A
  {"VR_CPU1_PVDD11_PWR", VR_ID9, read_vr_pout, false, {99, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x5B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x5C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x5D
  {"P3V_BAT_VOLT", ADC7, read_bat_val, true, {3.4, 0, 0, 2.6, 0, 0, 0, 0}, VOLT}, //0x5E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x5F

  {"CPU0_DIMM_A0_PWR", DIMM_ID0,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x60
  {"CPU0_DIMM_A1_PWR", DIMM_ID1,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x61
  {"CPU0_DIMM_A2_PWR", DIMM_ID2,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x62
  {"CPU0_DIMM_A3_PWR", DIMM_ID3,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x63
  {"CPU0_DIMM_A4_PWR", DIMM_ID4,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x64
  {"CPU0_DIMM_A5_PWR", DIMM_ID5,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x65
  {"CPU0_DIMM_A6_PWR", DIMM_ID6,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x66
  {"CPU0_DIMM_A7_PWR", DIMM_ID7,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x67
  {"CPU0_DIMM_A8_PWR", DIMM_ID8,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x68
  {"CPU0_DIMM_A9_PWR", DIMM_ID9,  read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x69
  {"CPU0_DIMM_A10_PWR", DIMM_ID10, read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x6A
  {"CPU0_DIMM_A11_PWR", DIMM_ID11, read_cpu0_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x6B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x6C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x6D
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x6E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x6F

  {"CPU1_DIMM_B0_PWR", DIMM_ID0,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x70
  {"CPU1_DIMM_B1_PWR", DIMM_ID1,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x71
  {"CPU1_DIMM_B2_PWR", DIMM_ID2,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x72
  {"CPU1_DIMM_B3_PWR", DIMM_ID3,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x73
  {"CPU1_DIMM_B4_PWR", DIMM_ID4,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x74
  {"CPU1_DIMM_B5_PWR", DIMM_ID5,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x75
  {"CPU1_DIMM_B6_PWR", DIMM_ID6,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x76
  {"CPU1_DIMM_B7_PWR", DIMM_ID7,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x77
  {"CPU1_DIMM_B8_PWR", DIMM_ID8,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x78
  {"CPU1_DIMM_B9_PWR", DIMM_ID9,  read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x79
  {"CPU1_DIMM_B10_PWR", DIMM_ID10, read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x7A
  {"CPU1_DIMM_B11_PWR", DIMM_ID11, read_cpu1_dimm_power, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0x7B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x7C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x7D
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x7E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x7F

  {"CPU0_DIMM_A0_TEMP", DIMM_ID0, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x80
  {"CPU0_DIMM_A1_TEMP", DIMM_ID1, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x81
  {"CPU0_DIMM_A2_TEMP", DIMM_ID2, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x82
  {"CPU0_DIMM_A3_TEMP", DIMM_ID3, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x83
  {"CPU0_DIMM_A4_TEMP", DIMM_ID4, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x84
  {"CPU0_DIMM_A5_TEMP", DIMM_ID5, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x85
  {"CPU0_DIMM_A6_TEMP", DIMM_ID6, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x86
  {"CPU0_DIMM_A7_TEMP", DIMM_ID7, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x87
  {"CPU0_DIMM_A8_TEMP",  DIMM_ID8, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x88
  {"CPU0_DIMM_A9_TEMP",  DIMM_ID9, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x89
  {"CPU0_DIMM_A10_TEMP", DIMM_ID10, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x8A
  {"CPU0_DIMM_A11_TEMP", DIMM_ID11, read_cpu0_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x8B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x8C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x8D
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x8E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x8F

  {"CPU1_DIMM_B0_TEMP", DIMM_ID0, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x90
  {"CPU1_DIMM_B1_TEMP", DIMM_ID1, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x91
  {"CPU1_DIMM_B2_TEMP", DIMM_ID2, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x92
  {"CPU1_DIMM_B3_TEMP", DIMM_ID3, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x93
  {"CPU1_DIMM_B4_TEMP", DIMM_ID4, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x94
  {"CPU1_DIMM_B5_TEMP", DIMM_ID5, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x95
  {"CPU1_DIMM_B6_TEMP", DIMM_ID6, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x96
  {"CPU1_DIMM_B7_TEMP", DIMM_ID7, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x97
  {"CPU1_DIMM_B8_TEMP",  DIMM_ID8, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x98
  {"CPU1_DIMM_B9_TEMP",  DIMM_ID9, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x99
  {"CPU1_DIMM_B10_TEMP", DIMM_ID10, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x9A
  {"CPU1_DIMM_B11_TEMP", DIMM_ID11, read_cpu1_dimm_temp, false, {85.0, 80.0, 0, 5, 0, 0, 0, 0}, TEMP}, //0x9B
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x9C
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x9D
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x9E
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0x9F

  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA0
  {"PROCESSOR_FAIL", FRU_MB, read_frb3, 0, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xA1
  {"CPLD_HEALTH", 0, read_cpld_health, 0, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xA2
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA3
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA4
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA5
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA6
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA7
  {"RETIMER0_HEALTH", 0, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA8
  {"RETIMER1_HEALTH", 1, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xA9
  {"RETIMER2_HEALTH", 2, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAA
  {"RETIMER3_HEALTH", 3, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAB
  {"RETIMER4_HEALTH", 4, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAC
  {"RETIMER5_HEALTH", 5, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAD
  {"RETIMER6_HEALTH", 6, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAE
  {"RETIMER7_HEALTH", 7, read_retimer_health, false, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xAF

  {"CPU0_DIMM_A0_STATE", DIMM_ID0, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB0
  {"CPU0_DIMM_A1_STATE", DIMM_ID1, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB1
  {"CPU0_DIMM_A2_STATE", DIMM_ID2, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB2
  {"CPU0_DIMM_A3_STATE", DIMM_ID3, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB3
  {"CPU0_DIMM_A4_STATE", DIMM_ID4, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB4
  {"CPU0_DIMM_A5_STATE", DIMM_ID5, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB5
  {"CPU0_DIMM_A6_STATE", DIMM_ID6, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB6
  {"CPU0_DIMM_A7_STATE", DIMM_ID7, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB7
  {"CPU0_DIMM_A8_STATE", DIMM_ID8, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB8
  {"CPU0_DIMM_A9_STATE", DIMM_ID9, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xB9
  {"CPU0_DIMM_A10_STATE", DIMM_ID10, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xBA
  {"CPU0_DIMM_A11_STATE", DIMM_ID11, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xBB
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xBC
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xBD
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xBE
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xBF

  {"CPU1_DIMM_B0_STATE", DIMM_ID12, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC0
  {"CPU1_DIMM_B1_STATE", DIMM_ID13, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC1
  {"CPU1_DIMM_B2_STATE", DIMM_ID14, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC2
  {"CPU1_DIMM_B3_STATE", DIMM_ID15, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC3
  {"CPU1_DIMM_B4_STATE", DIMM_ID16, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC4
  {"CPU1_DIMM_B5_STATE", DIMM_ID17, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC5
  {"CPU1_DIMM_B6_STATE", DIMM_ID18, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC6
  {"CPU1_DIMM_B7_STATE", DIMM_ID19, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC7
  {"CPU1_DIMM_B8_STATE", DIMM_ID20, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC8
  {"CPU1_DIMM_B9_STATE", DIMM_ID21, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xC9
  {"CPU1_DIMM_B10_STATE", DIMM_ID22, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xCA
  {"CPU1_DIMM_B11_STATE", DIMM_ID23, read_cpu_dimm_state, true, {0, 0, 0, 0, 0, 0, 0, 0}, STATE}, //0xCB
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xCC
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xCD
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xCE
  {NULL, 0, NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}, //0xCF

  {"RETIMER0_TEMP", 0, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD0
  {"RETIMER1_TEMP", 1, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD1
  {"RETIMER2_TEMP", 2, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD2
  {"RETIMER3_TEMP", 3, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD3
  {"RETIMER4_TEMP", 4, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD4
  {"RETIMER5_TEMP", 5, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD5
  {"RETIMER6_TEMP", 6, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD6
  {"RETIMER7_TEMP", 7, read_retimer_temp, false, {105, 0, 107, 5, 0, -7, 0, 0}, TEMP}, //0xD7
  {"P5V_AUX_ADC"        , ADC_CH8,  read_iic_adc_val, false, {5.26, 0, 0, 4.74, 0, 0, 0, 0}, VOLT}, //0xD8
  {"P1V8_AUX_ADC"       , ADC_CH9,  read_iic_adc_val, false, {1.90, 0, 0, 1.70, 0, 0, 0, 0}, VOLT}, //0xD9
  {"P1V2_AUX_ADC"       , ADC_CH10, read_iic_adc_val, false, {1.27, 0, 0, 1.13, 0, 0, 0, 0}, VOLT}, //0xDA
  {"P1V8_CPU0_RT_1D_ADC", ADC_CH11, read_iic_adc_val, false, {1.85, 0, 0, 1.76, 0, 0, 0, 0}, VOLT}, //0xDB
  {"P1V8_CPU1_RT_1D_ADC", ADC_CH12, read_iic_adc_val, false, {1.85, 0, 0, 1.76, 0, 0, 0, 0}, VOLT}, //0xDC
  {"PVDD_33_S5_ADC"     , ADC_CH13, read_iic_adc_val, false, {3.46, 0, 0, 3.13, 0, 0, 0, 0}, VOLT}, //0xDD
  {"PVDD18_S5_P0_ADC"   , ADC_CH14, read_iic_adc_val, false, {1.87, 0, 0, 1.73, 0, 0, 0, 0}, VOLT}, //0xDE
  {"PVDD18_S5_P1_ADC"   , ADC_CH15, read_iic_adc_val, false, {1.87, 0, 0, 1.73, 0, 0, 0, 0}, VOLT}, //0xDF

  {"VR_RT_P0V9_VDD1_TEMP", VR_ID10, read_vr_temp, false, {105, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0xE0
  {"VR_RT_P0V9_VDD1_VOUT", VR_ID10, read_vr_vout, false, {0.95, 0, 0, 0.85, 0, 0, 0}, VOLT}, //0xE1
  {"VR_RT_P0V9_VDD1_IOUT", VR_ID10, read_vr_iout, false, {0, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0xE2
  {"VR_RT_P0V9_VDD1_POUT", VR_ID10, read_vr_pout, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0xE3
  {"VR_RT_P0V9_VDD2_TEMP", VR_ID11, read_vr_temp, false, {105, 0, 0, 5, 0, 0, 0, 0}, TEMP}, //0xE4
  {"VR_RT_P0V9_VDD2_VOUT", VR_ID11, read_vr_vout, false, {0.95, 0, 0, 0.85, 0, 0, 0}, VOLT}, //0xE5
  {"VR_RT_P0V9_VDD2_IOUT", VR_ID11, read_vr_iout, false, {0, 0, 0, 0, 0, 0, 0, 0}, CURR}, //0xE6
  {"VR_RT_P0V9_VDD2_POUT", VR_ID11, read_vr_pout, false, {0, 0, 0, 0, 0, 0, 0, 0}, POWER}, //0xE7
};

extern struct snr_map sensor_map[];

size_t mb_sensor_cnt = sizeof(mb_sensor_list)/sizeof(uint8_t);
size_t mb_discrete_sensor_cnt = sizeof(mb_discrete_sensor_list)/sizeof(uint8_t);

int
read_adc_val(uint8_t fru, uint8_t sensor_num, float *value) {
  uint8_t adc_id = sensor_map[fru].map[sensor_num].id;
  if (adc_id >= ADC_NUM_CNT) {
    return -1;
  }
  return sensors_read_adc(sensor_map[fru].map[sensor_num].snr_name, value);
}

static int
read_bat_val(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = -1;

  gpio_desc_t *gp_batt = gpio_open_by_shadow(GPIO_P3V_BAT_SCALED_EN);
  if (!gp_batt) {
    return -1;
  }
  if (gpio_set_value(gp_batt, GPIO_VALUE_HIGH)) {
    goto bail;
  }

#ifdef DEBUG
  syslog(LOG_DEBUG, "%s %s\n", __func__, path);
#endif
  msleep(20);
  ret = read_adc_val(fru, sensor_num, value);
  if (gpio_set_value(gp_batt, GPIO_VALUE_LOW)) {
    goto bail;
  }

bail:
  gpio_close(gp_batt);
  return ret;
}

int
read_iic_adc_val(uint8_t fru, uint8_t sensor_num, float *value) {
  return sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
}

int
oper_iic_adc_power(uint8_t fru, uint8_t volt_snr_num, uint8_t curr_snr_num, float *value) {
  int ret;
  float iout=0;
  float vout=0;

  ret = read_iic_adc_val(fru, curr_snr_num, &iout);
  if(ret)
    return -1;

  ret = read_hsc_vin(FRU_MB, volt_snr_num, &vout);
  if(ret)
    return -1;

  *value = iout * vout;
  return 0;
}

static int
read_e1s_power(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret;

  ret = oper_iic_adc_power(fru, MB_SNR_HSC_VIN, MB_SNR_E1S_P12V_IOUT, value);
  return ret;
}

static int
read_e1s_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  int fd = 0, ret = -1;
  char fn[32];
  uint8_t tlen, rlen, addr, bus;
  uint8_t tbuf[16] = {0};
  uint8_t rbuf[16] = {0};
  uint8_t hd_id = sensor_map[fru].map[sensor_num].id;
  static uint8_t retry=0;

  bus = e1s_info_list[hd_id].bus;
  addr = e1s_info_list[hd_id].slv_addr;

  snprintf(fn, sizeof(fn), "/dev/i2c-%d", bus);
  fd = open(fn, O_RDWR);
  if (fd < 0) {
    return -1;
  }

  //Temp Register
  tbuf[0] = 0x03;
  tlen = 1;
  rlen = 1;

  ret = i2c_rdwr_msg_transfer(fd, addr, tbuf, tlen, rbuf, rlen);

#ifdef DEBUG
  syslog(LOG_DEBUG, "%s Temp[%d]=%x bus=%x slavaddr=%x\n", __func__, hd_id, rbuf[0], bus, addr);
#endif

  if( ret < 0 || (rbuf[0] >= 0x80) ) {
    retry++;
    ret = retry_err_handle(retry, 5);
    goto err_exit;
  }

  *value = rbuf[0];
  retry=0;
err_exit:
  if (fd > 0) {
    close(fd);
  }
  return ret;
}

int
apml_channel_lock(uint8_t cpu_id) {
  int ret = 0, lfd = -1;

  if (g_has_mux == false) {
    return -1;
  }

  lfd = single_instance_lock_blocked("apml_mux");
  if (lfd < 0) {
    syslog(LOG_WARNING, "%s, get apml_mux lock failed", __FUNCTION__);
  }

  gpio_value_t expected_value = cpu_id == CPU_ID0 ? GPIO_VALUE_LOW : GPIO_VALUE_HIGH;
  gpio_value_t current_value = gpio_get_value_by_shadow(GPIO_APML_MUX2_SEL);
  if (current_value != expected_value) {
    ret = gpio_set_value_by_shadow(GPIO_APML_MUX2_SEL, expected_value);
    if (ret < 0) {
      syslog(LOG_WARNING, "%s, set apml sgpio fail", __FUNCTION__);
      if (lfd >= 0) {
        single_instance_unlock(lfd);
      }
      return ret;
    }
    msleep(DELAY_APML_MUX);
  }

  return lfd;
}

void
apml_channel_unlock(int lock) {
  if (lock >= 0) {
    single_instance_unlock(lock);
  }
}

static int reset_mux() {
  int fd = 0, ret = -1;
  char fn[32];
  uint8_t tlen, rlen, addr, bus;
  uint8_t tbuf[16] = {0};
  uint8_t rbuf[16] = {0};

  bus = I2C_BUS_0;
  addr = 0xE2;

  snprintf(fn, sizeof(fn), "/dev/i2c-%d", bus);
  fd = open(fn, O_RDWR);
  if (fd < 0) {
    return ret;
  }

  tbuf[0] = 0x46;
  tbuf[1] = 0x01;
  tlen = 2;
  rlen = 0;

  ret = i2c_rdwr_msg_transfer(fd, addr, tbuf, tlen, rbuf, rlen);
  close(fd);

  return ret;
}

static int
read_cpu_pkg_pwr(uint8_t fru, uint8_t sensor_num, float *value) {
  uint8_t cpu_id = sensor_map[fru].map[sensor_num].id;
  int ret, lock = -1;
  char* cpu_chips[] = {
    "sbrmi-i2c-0-3c",
    "sbrmi-i2c-0-38",
  };
  static uint8_t retry[ARRAY_SIZE(cpu_chips)] = {0};

  if(!is_cpu_socket_occupy(cpu_id))
    return READING_NA;

  if(pal_bios_completed(fru) != true) {
    return READING_NA;
  }

  lock = apml_channel_lock(cpu_id);
  ret = sensors_read(cpu_chips[cpu_id], sensor_map[fru].map[sensor_num].snr_name, value);
  apml_channel_unlock(lock);
  if (ret) {
    retry[cpu_id]++;
    return retry_err_handle(retry[cpu_id], 3);
  }

  retry[cpu_id] = 0;
  return ret;
}

static int
read_cpu_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  uint8_t cpu_id = sensor_map[fru].map[sensor_num].id;
  int ret, lock = -1;
  char* cpu_chips[] = {
    "sbtsi-i2c-0-4c",
    "sbtsi-i2c-0-48",
  };
  static uint8_t retry[ARRAY_SIZE(cpu_chips)] = {0};

  if(pal_bios_completed(fru) != true) {
    return READING_NA;
  }

  if(!is_cpu_socket_occupy(cpu_id))
    return READING_NA;

  lock = apml_channel_lock(cpu_id);
  ret = sensors_read(cpu_chips[cpu_id], sensor_map[fru].map[sensor_num].snr_name, value);
  apml_channel_unlock(lock);
  if (ret) {
    retry[cpu_id]++;
    return retry_err_handle(retry[cpu_id], 3);
  }

  retry[cpu_id] = 0;
  return ret;
}

static int
read_i3c_dimm_temp(uint8_t cpu_id, uint8_t dimm_id, float *value) {
  const char* snr_path[2] = {DIMM_TS0_SNR_DIR, DIMM_TS1_SNR_DIR};
  int fd;
  float ts[2] = {-256, -256};
  char fp[32] = {0};
  uint8_t txbuf[1] = {0x31};
  uint8_t rxbuf[2] = {0};

  for (int i = 0; i < 2; i++) {
    if(dimm_id < CHANNEL_OF_DIMM_NUM) {
      snprintf(fp, sizeof(fp), snr_path[i], cpu_id*2, dimm_id%CHANNEL_OF_DIMM_NUM);
    }
    else {
      snprintf(fp, sizeof(fp), snr_path[i], cpu_id*2+1, dimm_id%CHANNEL_OF_DIMM_NUM);
    }

    fd = i3c_open(fp);
    if (fd >= 0) {
      if (i3c_rdwr_msg_xfer(fd, txbuf, 1, rxbuf, 2) == 0) {
        if (GETBIT(rxbuf[1], 4)) { // negative
          ts[i] = -0.25 * (((~((rxbuf[1] << 8) | rxbuf[0]) >> 2) + 1) & 0x3FF);
        }
        else {
          ts[i] = 0.25 * ((((rxbuf[1] << 8) | rxbuf[0]) >> 2) & 0x3FF);
        }
      }
      i3c_close(fd);
    }
  }

  *value = (ts[0] > ts[1]) ? ts[0] : ts[1];

  //TS sensor is +-256
  if (*value == -256) {
    return -1;
  }

  return 0;
}

static void decode_dimm_temp(uint16_t raw, float *temp)
{
  if (raw <= 0x3FF)
    *temp = raw * SCALING_FACTOR;
  else
    *temp = (raw - 0x800) * SCALING_FACTOR;
}

static int
read_dimm_temp(uint8_t fru, uint8_t sensor_num, float *value,
                uint8_t dimm_id, uint8_t cpu_id) {
  struct dimm_thermal d_sensor;
  float temp = 0;
  oob_status_t ret;

  uint8_t addr = (dimm_id/CHANNEL_OF_DIMM_NUM << 4) + (dimm_id%CHANNEL_OF_DIMM_NUM);

  ret = read_dimm_thermal_sensor(cpu_id, addr, &d_sensor);
  if(!ret) {
    decode_dimm_temp(d_sensor.sensor, &temp);
    *value = (float)temp;
  } else {
    return -1;
  }

  return 0;
}

static int
read_cpu0_dimm_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret, lock = -1;
  char has_mux[MAX_VALUE_LEN] = {0};
  uint8_t dimm_id = sensor_map[fru].map[sensor_num].id;
  float unc = sensor_map[fru].map[sensor_num].snr_thresh.unc_thresh;
  static float max_temp;
  static uint8_t retry[MAX_DIMM_NUM] = {0};
  static bool asserted = false;

  if(!is_cpu_socket_occupy(CPU_ID0)) {
    return READING_NA;
  }

  if (!is_dimm_present(dimm_id)){
    return READING_NA;
  }

  if (sensor_num == APML_SNR_START_INDEX) {
    kv_get("apml_mux", has_mux, 0, 0);
    if (!strcmp(has_mux, "0")) {
      g_has_mux = true;
    } else if (!strcmp(has_mux, "1")) {
      g_has_mux = false;
      ret = reset_mux();
      if (ret < 0) {
        syslog(LOG_WARNING, "%s, reset mux fail", __FUNCTION__);
      }
    }
  }

  if (gpio_get_value_by_shadow("FM_CPU0_SPD_HOST_CTRL_R_N") == GPIO_VALUE_LOW) {
    if(dimm_id == DIMM_ID0) {
      max_temp = 0;
    }

    if ((ret = read_i3c_dimm_temp(CPU_ID0, dimm_id, value)) == 0) {
      if (max_temp < *value) {
        max_temp = *value;
      }
    }

    if((dimm_id == DIMM_ID11) && asserted == false && (max_temp >= unc)) {
      lock = apml_channel_lock(CPU_ID0);
      if (write_dram_throttle (CPU_ID0, 50) == OOB_SUCCESS) {
        asserted = true;
      }
      apml_channel_unlock(lock);
    }

    if((dimm_id == DIMM_ID11) && asserted == true && (max_temp <= unc-1)) {
      lock = apml_channel_lock(CPU_ID0);
      if (write_dram_throttle (CPU_ID0, 0) == OOB_SUCCESS) {
        asserted = false;
      }
      apml_channel_unlock(lock);
    }
  }
  else {
    lock = apml_channel_lock(CPU_ID0);
    ret = read_dimm_temp(fru, sensor_num, value, dimm_id, CPU_ID0);
    apml_channel_unlock(lock);
  }

  if ( ret != 0 ) {
    retry[dimm_id]++;
    return retry_err_handle(retry[dimm_id], 3);
  }

#ifdef DEBUG
  syslog(LOG_DEBUG, "%s DIMM Temp=%f id=%d\n", __func__, *value, dimm_id);
#endif
  retry[dimm_id] = 0;
  return 0;
}

static int
read_cpu1_dimm_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret, lock = -1;
  uint8_t dimm_id = sensor_map[fru].map[sensor_num].id;
  float unc = sensor_map[fru].map[sensor_num].snr_thresh.unc_thresh;
  static float max_temp;
  static uint8_t retry[MAX_DIMM_NUM] = {0};
  static bool asserted = false;

  if(!is_cpu_socket_occupy(CPU_ID1)) {
    return READING_NA;
  }

  if (!is_dimm_present(dimm_id + PER_CPU_DIMM_NUMBER_MAX)){
    return READING_NA;
  }

  if (gpio_get_value_by_shadow("FM_CPU0_SPD_HOST_CTRL_R_N") == GPIO_VALUE_LOW) {
    if(dimm_id == DIMM_ID0) {
      max_temp = 0;
    }

    if ((ret = read_i3c_dimm_temp(CPU_ID1, dimm_id, value)) == 0) {
      if (max_temp < *value) {
        max_temp = *value;
      }
    }

    if((dimm_id == DIMM_ID11) && asserted == false && (max_temp >= unc)) {
      lock = apml_channel_lock(CPU_ID1);
      if(write_dram_throttle (CPU_ID1, 50) == OOB_SUCCESS) {
        asserted = true;
      }
      apml_channel_unlock(lock);
    }

    if((dimm_id == DIMM_ID11) && asserted == true && (max_temp <= unc-1)) {
      lock = apml_channel_lock(CPU_ID1);
      if(write_dram_throttle (CPU_ID1, 0) == OOB_SUCCESS) {
        asserted = false;
      }
      apml_channel_unlock(lock);
    }
  }
  else {
    lock = apml_channel_lock(CPU_ID1);
    ret = read_dimm_temp(fru, sensor_num, value, dimm_id, CPU_ID1);
    apml_channel_unlock(lock);
  }

  if ( ret != 0 ) {
    retry[dimm_id]++;
    return retry_err_handle(retry[dimm_id], 3);
  }

#ifdef DEBUG
  syslog(LOG_DEBUG, "%s DIMM Temp=%f id=%d\n", __func__, *value, dimm_id);
#endif
  retry[dimm_id] = 0;
  return 0;
}

static int
read_i3c_dimm_power(uint8_t dimm_id, float *value) {
  int fd, ret = 0;
  char fp[32] = {0};
  uint8_t txbuf[1] = {0x0C};
  uint8_t rxbuf[1] = {0};

  snprintf(fp, sizeof(fp), DIMM_PWR_SNR_DIR, dimm_id/CHANNEL_OF_DIMM_NUM, dimm_id%CHANNEL_OF_DIMM_NUM);
  fd = i3c_open(fp);
  if (fd < 0) {
    syslog(LOG_WARNING, "%s, fp =%s, open failed", __FUNCTION__, fp);
    return -1;
  }

  ret = i3c_rdwr_msg_xfer(fd, txbuf, 1, rxbuf, 1);
  i3c_close(fd);
  if (ret == 0) {
    *value = ((float)(rxbuf[0] * 125))/1000;
  }

   return ret;
}


static int
read_dimm_power(uint8_t fru, uint8_t sensor_num, float *value,
                uint8_t dimm_id, uint8_t cpu_id) {
  struct dimm_power d_power;
  oob_status_t ret;

  uint8_t addr = (dimm_id/CHANNEL_OF_DIMM_NUM << 4) + (dimm_id%CHANNEL_OF_DIMM_NUM);
  ret = read_dimm_power_consumption(cpu_id, addr, &d_power);

  if(ret)
    return -1;

  *value = ((float)d_power.power)/1000;
  return 0;
}

static int
read_cpu0_dimm_power(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret, lock = -1;
  static uint8_t retry[MAX_DIMM_NUM] = {0};
  uint8_t dimm_id = sensor_map[fru].map[sensor_num].id;

  if (!is_cpu_socket_occupy(CPU_ID0)){
    return READING_NA;
  }

  if (!is_dimm_present(dimm_id)){
    return READING_NA;
  }

  if (gpio_get_value_by_shadow("FM_CPU0_SPD_HOST_CTRL_R_N") == GPIO_VALUE_LOW) {
    return read_i3c_dimm_power(dimm_id, value);
  }

  lock = apml_channel_lock(CPU_ID0);
  ret = read_dimm_power(fru, sensor_num, value, dimm_id, CPU_ID0);
  apml_channel_unlock(lock);
  if ( ret != 0 ) {
    retry[dimm_id]++;
    return retry_err_handle(retry[dimm_id], 5);
  }

  retry[dimm_id] = 0;
  return 0;
}

static int
read_cpu1_dimm_power(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret, lock = -1;
  static uint8_t retry[MAX_DIMM_NUM] = {0};
  uint8_t dimm_id = sensor_map[fru].map[sensor_num].id;

  if (!is_cpu_socket_occupy(CPU_ID1)) {
    return READING_NA;
  }

  if (!is_dimm_present(dimm_id + PER_CPU_DIMM_NUMBER_MAX)){
    return READING_NA;
  }

  if (gpio_get_value_by_shadow("FM_CPU0_SPD_HOST_CTRL_R_N") == GPIO_VALUE_LOW) {
    return read_i3c_dimm_power(dimm_id + CHANNEL_OF_DIMM_NUM*2, value);
  }

  lock = apml_channel_lock(CPU_ID1);
  ret = read_dimm_power(fru, sensor_num, value, dimm_id, CPU_ID1);
  apml_channel_unlock(lock);
  if ( ret != 0 ) {
    retry[dimm_id]++;
    return retry_err_handle(retry[dimm_id], 5);
  }

  retry[dimm_id] = 0;
  return 0;
}

//Sensor HSC
static int
read_hsc_vin(uint8_t fru, uint8_t sensor_num, float *value) {
  return sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
}

static int
read_hsc_iout(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = 0;
  char source[10] = {0};

  ret = sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
  if (ret < 0) {
   return ret;
  }

  kv_get("mb_hsc_source", source, 0, 0);

  if (!strcmp(source, "0")) {
    *value = (*value * 1.0202 + 0.7316);
  }
  else if(!strcmp(source, "1")) {
    *value = (*value * 1.3459 - 0.1993);
  }

  return 0;
}

static int
read_hsc_pin(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = 0;
  char source[10] = {0};

  ret = sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);

  if (ret < 0) {
    return ret;
  }

  kv_get("mb_hsc_source", source, 0, 0);

  if (!strcmp(source, "0")) {
    *value = (*value * 1.0265 + 9.865);
  }
  else if(!strcmp(source, "1")) {
    *value = (*value * 1.3481 - 2.7754);
  }

  return 0;
}

static int
read_hsc_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  return sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
}

static int
read_hsc_peak_pin(uint8_t fru, uint8_t sensor_num, float *value) {
  return sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
}

static int
read_mb_temp (uint8_t fru, uint8_t sensor_num, float *value) {
  return sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
}

int
read_isl28022(uint8_t fru, uint8_t sensor_num, float *value) {
  int fd = 0, ret = -1;
  char fn[32];
  uint8_t tlen, rlen, addr, bus;
  float scale;
  uint8_t tbuf[16] = {0};
  uint8_t rbuf[16] = {0};
  uint8_t dpm_id = sensor_map[fru].map[sensor_num].id;

  bus = dpm_info_list[dpm_id].bus;
  addr = dpm_info_list[dpm_id].slv_addr;
  scale = dpm_info_list[dpm_id].vbus_scale;

  snprintf(fn, sizeof(fn), "/dev/i2c-%d", bus);
  fd = open(fn, O_RDWR);
  if (fd < 0) {
    goto err_exit;
  }

  //Voltage BUS Register
  tbuf[0] = 0x02;
  tlen = 1;
  rlen = 2;

  ret = i2c_rdwr_msg_transfer(fd, addr, tbuf, tlen, rbuf, rlen);

#ifdef DEBUG
  syslog(LOG_DEBUG, "%s Voltage [%d] =%x %x bus=%x slavaddr=%x\n", __func__, dpm_id,
         rbuf[1], rbuf[0], bus, addr);
#endif

  *value = ((rbuf[0] << 8 | rbuf[1]) >> 2) * scale;
err_exit:
  if (fd > 0) {
    close(fd);
  }
  return ret;
}

int
read_dpm_vout(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = 0;
  uint8_t dpm_id = sensor_map[fru].map[sensor_num].id;
  static uint8_t retry[DPM_NUM_CNT] = {0};
  uint8_t mb_rev;
  uint8_t mb_sku = 0;

  if(pal_get_board_rev_id(FRU_MB, &mb_rev))
    return -1;

  if(pal_get_board_sku_id(FRU_MB, &mb_sku))
    return -1;

  if (mb_rev == 0) {
    ret = read_isl28022(fru, sensor_num, value);
  } else {
    if (pal_is_artemis()) {
      if (  (mb_sku & 0x0F) == GTA_CONFIG_1
         || (mb_sku & 0x0F) == GTA_CONFIG_8
         || (mb_sku & 0x0F) == GTA_CONFIG_9
         || (mb_sku & 0x0F) == GTA_CONFIG_12
         || (mb_sku & 0x0F) == GTA_CONFIG_13) {
        ret = read_isl28022(fru, sensor_num, value);
      } else {
        ret = sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
      }
    } else {
      ret = sensors_read(NULL, sensor_map[fru].map[sensor_num].snr_name, value);
    }
  }

  if (ret || *value == 0) {
    retry[dpm_id]++;
    return retry_err_handle(retry[dpm_id], 2);
  }

  retry[dpm_id] = 0;
  return ret;
}

static void
_print_sensor_discrete_log(uint8_t fru, uint8_t snr_num, char *snr_name,
    uint8_t val, char *event) {
  if (val) {
    syslog(LOG_CRIT, "ASSERT: %s discrete - raised - FRU: %d", event, fru);
  } else {
    syslog(LOG_CRIT, "DEASSERT: %s discrete - settled - FRU: %d", event, fru);
  }
  pal_update_ts_sled();
}

static int
check_frb3(uint8_t fru_id, uint8_t sensor_num, float *value) {
  static uint8_t frb3_fail = 0x10; // bit 4: FRB3 failure
  static time_t rst_time = 0;
  static uint8_t postcodes_last[256] = {0};
  uint8_t postcodes[256] = {0};
  struct stat file_stat;
  int rc;
  size_t len = 0, cmp_len = sizeof(postcodes);
  char sensor_name[32] = {0};
  char error[32] = {0};
  static struct timespec time_start;
  struct timespec time_end;
  long time_diff = 0;
  static bool is_frb3_log = false;
  static bool is_init     = false;

  if (fru_id != FRU_MB) {
    syslog(LOG_ERR, "Not Supported Operation for fru %d", fru_id);
    return READING_NA;
  }

  if (!is_init) {
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    is_init = true;
  }

  if (stat("/tmp/rst_touch", &file_stat) == 0 && file_stat.st_mtime > rst_time) {
    is_frb3_log = false;
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    rst_time = file_stat.st_mtime;
    // assume fail till we know it is not
    frb3_fail = 0x10; // bit 4: FRB3 failure
    cmp_len = sizeof(postcodes);
    // cache current postcode buffer
    memset(postcodes_last, 0, sizeof(postcodes_last));
    pal_get_lpc_pcc_record(FRU_MB, postcodes_last, sizeof(postcodes_last), &len);
  }

  rc = pal_get_lpc_pcc_record(FRU_MB, postcodes, cmp_len, &len);
  if (frb3_fail) {
    // KCS transaction
    if (stat("/tmp/kcs_touch", &file_stat) == 0 && file_stat.st_mtime > rst_time)
      frb3_fail = 0;

    // Port 80 updated
    if (rc == PAL_EOK && memcmp(postcodes_last, postcodes, sizeof(postcodes))) {
      frb3_fail = 0;
      // reduce the arg copy of pal_get_lpc_pcc_record(),
      // just to keep updating postcode in cache store
      cmp_len = 0;
    }

    // BIOS POST COMPLT, in case BMC reboot when system idle in OS
    if (pal_bios_completed(FRU_MB))
      frb3_fail = 0;
  }

  if (frb3_fail) {
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    time_diff = time_end.tv_sec - time_start.tv_sec;
  }

  if (time_diff >= FRB3_TRIGGER_TIME && !is_frb3_log) {
    pal_get_sensor_name(fru_id, sensor_num, sensor_name);
    snprintf(error, sizeof(error), "FRB3 failure");
    _print_sensor_discrete_log(fru_id, sensor_num, sensor_name, frb3_fail, error);
    is_frb3_log = true;

    pal_check_power_rail(0);

    /* Workaround_for_Genoa_Erratum_#1477 */
    char rev_id[MAX_VALUE_LEN] = {0};
    kv_get("mb_rev", rev_id, 0, 0);

    int fd = 0, ret = -1;
    char fn[32];
    uint8_t tlen, rlen, addr, bus;
    uint8_t tbuf[16] = {0};
    uint8_t rbuf[16] = {0};
    uint8_t dimm_pwr_ok = 0xF0;

    bus = I2C_BUS_7;
    addr = 0x46;

    snprintf(fn, sizeof(fn), "/dev/i2c-%d", bus);
    fd = open(fn, O_RDWR);
    if (fd < 0) {
      return ret;
    }

    tbuf[0] = 0x15;
    tlen = 1;
    rlen = 1;
    ret = i2c_rdwr_msg_transfer(fd, addr, tbuf, tlen, rbuf, rlen);
    close(fd);

    if (ret == 0) {
      dimm_pwr_ok = (rbuf[0] & 0xF0);
    }
    else {
      syslog(LOG_CRIT, "%s, dump CPLD reg failed, ret = %d\n", __func__, ret);
    }


    int id = atoi(rev_id);
    if (id < 4 && dimm_pwr_ok == 0) {
      syslog(LOG_CRIT, "Detect Genoa System Hang (FRB3/No DIMM Power OK): 0x%2x\n", dimm_pwr_ok);
      pal_sled_cycle();
    }
    /* Workaround_for_Genoa_Erratum_#1477 */
  }

  return 0;
}

static
int read_frb3(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = 0;
  uint8_t fru_id = sensor_map[fru].map[sensor_num].id;

#ifdef DEBUG
  syslog(LOG_INFO, "%s\n", __func__);
#endif
  ret = check_frb3(fru_id, MB_SNR_PROCESSOR_FAIL, value);
  *value = 0;
  return ret;
}

static
int read_retimer_temp(uint8_t fru, uint8_t sensor_num, float *value) {
  int ret = 0, lock = -1;
  const char shadow[MAX_VALUE_LEN] = "RST_PERST_CPUx_SWB_N";
  char rev_id[MAX_VALUE_LEN] = {0};
  float val = 0;
  static uint8_t retry[8]= {0};

  static AriesDeviceType ariesDevice[8];
  static AriesI2CDriverType i2cDriver[8];
  int rt_id = sensor_map[fru].map[sensor_num].id;
  int bus = RT_DEV_INFO[rt_id].bus;
  int addr = RT_DEV_INFO[rt_id].addr;

  static int is_init[8] = {0};

  if (gpio_get_value_by_shadow(shadow) != GPIO_VALUE_HIGH) {
    return READING_NA;
  }

  kv_get("mb_rev", rev_id, 0, 0);
  if (!strcmp(rev_id, "2")) {                // 2 retimer SKU
    if (sensor_num != MB_SNR_RETIMER0_TEMP && sensor_num != MB_SNR_RETIMER4_TEMP) {
      return READING_NA;
    }
  }

  if ((lock = mb_retimer_lock()) < 0) {
    syslog(LOG_WARNING, "%s: mb_retimer_lock failed", __func__);
    return READING_SKIP;
  }

  if (is_init[rt_id] == 0) {
    ret = SetupAriesDevice(&ariesDevice[rt_id], &i2cDriver[rt_id], bus, addr);
    if (ret != ARIES_SUCCESS) {
      ret = READING_NA;
      goto err_exit;
    }

    is_init[rt_id] = 1;
  }

  ret = AriesGetTemp(&ariesDevice[rt_id], &i2cDriver[rt_id], bus, addr, &val);

  if (ret != ARIES_SUCCESS) {
    retry[sensor_num - MB_SNR_RETIMER0_TEMP]++;
    ret = retry_err_handle(retry[sensor_num - MB_SNR_RETIMER0_TEMP], 5);
    goto err_exit;
  }

  *value = val;

err_exit:
  mb_retimer_unlock(lock);
  return ret;
}

static
int read_retimer_health(uint8_t fru, uint8_t sensor_num, float *value) {
  const int HEARTBEAT = 1;
  const int CODE_LOAD = 1 << 1;
  static uint8_t retry[MAX_NUM_RETIMERS] = {0};
  char rev_id[32] = {0};
  char fru_name[32] = {0};
  int ret = 0, lock = -1;
  int retimer_id = sensor_map[fru].map[sensor_num].id;
  uint8_t health = 0;

  if(pal_get_fru_name(fru, fru_name) || retimer_id >= MAX_NUM_RETIMERS) {
    return READING_NA;
  }

  /*  Simply return if one of the signal is inactive. */
  if (gpio_get_value_by_shadow("RST_PERST_CPUx_SWB_N") != GPIO_VALUE_HIGH ||
      gpio_get_value_by_shadow("FM_RST_CPU1_RESETL_N") != GPIO_VALUE_HIGH) {
    return READING_NA;
  }

  kv_get("mb_rev", rev_id, 0, 0);
  if (!strcmp(rev_id, "2")) {
    if (sensor_num != MB_SNR_RETIMER0_TEMP && sensor_num != MB_SNR_RETIMER4_TEMP) {
      return READING_NA;
    }
  }

  if ((lock = mb_retimer_lock()) < 0) {
    syslog(LOG_WARNING, "%s: mb_retimer_lock failed", __func__);
    return READING_SKIP;
  }

  ret = AriesGetHealth(fru_name, retimer_id, &health);
  mb_retimer_unlock(lock);

  if ((ret == ARIES_SUCCESS) &&
      ((health & (HEARTBEAT|CODE_LOAD)) == (HEARTBEAT|CODE_LOAD))) {
    *value = 0;
  }
  else {
    // If the system is powered off during the retimer health reading,
    // simply return if one of the signal is inactive.
    if (gpio_get_value_by_shadow("RST_PERST_CPUx_SWB_N") != GPIO_VALUE_HIGH ||
        gpio_get_value_by_shadow("FM_RST_CPU1_RESETL_N") != GPIO_VALUE_HIGH) {
      return READING_NA;
    }
    // These retries only occurs for the time duration between gpiod and
    // sensord (about 1 sec) when BMC boots up. So we don't need to reset
    // the retry to 0.
    retry[retimer_id]++;
    if (retry_err_handle(retry[retimer_id], 5) == READING_SKIP) {
      return READING_SKIP;
    }
    // If the maximum retry attempts are exceeded, it should be considered
    // as the retimer being in an unhealthy state.
    *value = 1;
  }

  return 0;
}

static
int read_cpu_dimm_state(uint8_t fru, uint8_t sensor_num, float *value) {
  const char SPD_CTRL[MAX_VALUE_LEN] = "FM_CPU0_SPD_HOST_CTRL_R_N";
  const char *err_list[MAX_PMIC_ERR_TYPE] = {0};

  int ret, i, index=0;
  static bool dimm_err_metric[MAX_DIMM_NUM][MAX_PMIC_ERR_TYPE]= {false};
  static bool dimm_prsnt_list[MAX_DIMM_NUM] = {0};
  uint8_t dimm_id = sensor_map[fru].map[sensor_num].id; // dimm_id 0-23
  uint8_t dimm_num = dimm_id%PER_CPU_DIMM_NUMBER_MAX;   // per cpu dimm number 0-11
  uint8_t cpu_id = dimm_id/PER_CPU_DIMM_NUMBER_MAX;     // cpu_id: 0-1
  uint8_t err_cnt = 0;
  bool curr[MAX_PMIC_ERR_TYPE] = {false};
  bool tag=false;
  char name[64] = {0};

  *value = 0;
  if (dimm_id == DIMM_ID0) {
    get_dimm_present_info(FRU_MB, dimm_prsnt_list);
  }

  if (dimm_prsnt_list[dimm_id] == false ||
      gpio_get_value_by_shadow(SPD_CTRL) == GPIO_VALUE_HIGH) {
    return READING_NA;
  }

  pmic_list_err(fru, cpu_id, dimm_num, err_list, &err_cnt);
  if(err_cnt > 0) {
    for (i = 0; i < err_cnt; i++) {
      index = pmic_err_index(err_list[i]);
      curr[index] = true;
    }

    // If multiple PMIC error occurs, the SEL will be logged
    // multiple times.
    for (i = 0; i < MAX_PMIC_ERR_TYPE; i++) {
      tag = curr[i] ^ dimm_err_metric[dimm_id][i];
      if(tag && curr[i] == true) {
        pmic_err_name(i, name);
        syslog(LOG_CRIT, "ASSERT: DIMM %s - PMIC Error: %s",
               get_dimm_label(cpu_id, dimm_num), name);
        dimm_err_metric[dimm_id][i] = curr[i];
      }
    }

    // Since the SEL log has been logged, so we clear PMIC err.
    ret = pmic_clear_err(fru, cpu_id, dimm_num);
    if (!ret) {
      for (i = 0; i < MAX_PMIC_ERR_TYPE; i++) {
        dimm_err_metric[dimm_id][i] = false;
      }
    }
  }

  return 0;
}
