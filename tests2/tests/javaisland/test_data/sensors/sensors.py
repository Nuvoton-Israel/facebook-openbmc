# ",/usr/bin/env python,
# ,
# Copyright 2018-present Facebook. All Rights Reserved.,
# ,
# This program file is free software; you can redistribute it and/or modify it,
# under the terms of the GNU General Public License as published by the,
# Free Software Foundation; version 2 of the License.,
# ,
# This program is distributed in the hope that it will be useful, but WITHOUT,
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or,
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License,
# for more details.,
# ,
# You should have received a copy of the GNU General Public License,
# along with this program in a file named COPYING; if not, write to the,
# Free Software Foundation, Inc.,,
# 51 Franklin Street, Fifth Floor,,
# Boston, MA 02110-1301 USA,
# ,

SENSORS_SLOT = [
    "MB_INLET_TEMP_C",
    "MB_OUTLET_TEMP_C",
    "MB_FIO_FRONT_TEMP_C",
    "MB_SOC_CPU_TEMP_C",
    "MB_E1S_SSD_TEMP_C",
    "MB_HSC_TEMP_C",
    "MB_RETIMER_TEMP_C",
    "MB_LPDDR5_UP_TEMP_C",
    "MB_LPDDR5_DOWN_TEMP_C",
    "MB_HSC_INPUT_VOLT_V",
    "MB_ADC_P12V_STBY_VOLT_V",
    "MB_ADC_VDD_1V8_VOLT_V",
    "MB_ADC_P3V3_STBY_VOLT_V",
    "MB_ADC_SOCVDD_VOLT_V",
    "MB_ADC_P3V_BAT_VOLT_V",
    "MB_ADC_CPUVDD_VOLT_V",
    "MB_ADC_1V2_VOLT_V",
    "MB_ADC_VDD_3V3_VOLT_V",
    "MB_ADC_P1V2_STBY_VOLT_V",
    "MB_ADC_FBVDDQ_VOLT_V",
    "MB_ADC_FBVDDP2_VOLT_V",
    "MB_ADC_FBVDD1_VOLT_V",
    "MB_ADC_P5V_STBY_VOLT_V",
    "MB_ADC_CPU_DVDD_VOLT_V",
    "MB_E1S_SSD_VOLT_V",
    "MB_CPUVDD_VOLT_V",
    "MB_SOCVDD_VOLT_V",
    "MB_HSC_OUTPUT_CURR_A",
    "MB_E1S_SSD_CURR_A",
    "MB_CPU_PWR_W",
    "MB_HSC_INPUT_PWR_W",
    "MB_E1S_SSD_PWR_W",
    "MB_CPUVDD_PWR_W",
    "MB_SOCVDD_PWR_W",
    "MB_CPU_THROTTLE_STA",
    "MB_PWR_BREAK_STA",
    "MB_SPARE_CH_STA",
    "MB_PAGE_RETIRE_CNT",
]

SENSOR_NIC = ["NIC_TEMP_C"]

SENSORS_BMC = [
    "BB_FAN0_TACH_RPM",
    "BB_FAN1_TACH_RPM",
    "BB_FAN2_TACH_RPM",
    "BB_FAN3_TACH_RPM",
    "BB_FAN4_TACH_RPM",
    "BB_FAN5_TACH_RPM",
    "BB_FAN6_TACH_RPM",
    "BB_FAN7_TACH_RPM",
    "BB_PWM0_TACH_PCT",
    "BB_PWM1_TACH_PCT",
    "BB_PWM2_TACH_PCT",
    "BB_PWM3_TACH_PCT",
    "BB_OUTLET_TEMP_C",
    "BB_INLET_TEMP_C",
    "BB_P5V_VOLT_V",
    "BB_P12V_VOLT_V",
    "BB_P3V3_STBY_VOLT_V",
    "BB_P1V8_STBY_VOLT_V",
    "BB_P1V2_STBY_VOLT_V",
    "BB_P2V5_STBY_VOLT_V",
    "BB_ADC_P1V0_STBY_VOLT_V",
    "BB_ADC_P0V6_STBY_VOLT_V",
    "BB_ADC_P3V3_RGM_STBY_VOLT_V",
    "BB_ADC_P5V_USB_VOLT_V",
    "BB_ADC_P3V3_NIC_VOLT_V",
    "BB_HSC_TEMP_C",
    "BB_HSC_INPUT_VOLT_V",
    "BB_HSC_INPUT_PWR_W",
    "BB_HSC_OUTPUT_CURR_A",
    "BB_HSC_PEAK_OUTPUT_CURR_A",
    "BB_HSC_PEAK_INPUT_PWR_W",
    "BB_MEDUSA_OUTPUT_VOLT_V",
    "BB_MEDUSA_INPUT_VOLT_V",
    "BB_MEDUSA_CURR_A",
    "BB_MEDUSA_PWR_W",
    "BB_MEDUSA_VDELTA_VOLT_V",
    "BB_CPU_CL_VDELTA_VOLT_V",
    "BB_CPU_PDB_VDELTA_VOLT_V",
    "BB_PMON_CURRENT_LEAKAGE_CURR_A",
    "BB_ADC_FAN_OUTPUT_CURR_A",
    "BB_FAN_PWR_W",
    "BB_ADC_NIC_P12V_VOLT_V",
    "BB_ADC_NIC_OUTPUT_CURR_A",
    "BB_NIC_PWR_W",
]

SENSORS_AGGREGATE = ["BB_SYSTEM_AIRFLOW_CFM"]

SENSORS = {
    "slot1": SENSORS_SLOT,
    "slot3": SENSORS_SLOT,
    "bmc": SENSORS_BMC,
    "nic": SENSOR_NIC,
    "aggregate": SENSORS_AGGREGATE,
}
