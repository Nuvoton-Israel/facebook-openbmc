/*
 * Copyright 2020 Astera Labs, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aries_misc.c
 * @brief Implementation of helper functions for the SDK.
 */

#include "aries_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

AriesBifurcationParamsType bifurcationModes[36] = {
    /** Links in bifurcation string read right to left */
    /** Bifurcation properties for x16 */
    {1, {{0, 16, 0}}},
    /** Bifurcation properties for x8 */
    {1, {{0, 8, 0}}},
    /** Bifurcation properties for x4 */
    {1, {{0, 4, 0}}},
    /** Bifurcation properties for x8x8 */
    {2, {{0, 8, 0}, {8, 8, 1}}},
    /** Bifurcation properties for x8x4x4 */
    {3, {{0, 4, 0}, {4, 4, 1}, {8, 8, 2}}},
    /** Bifurcation properties for x4x4x8 */
    {3, {{0, 8, 0}, {8, 4, 1}, {12, 4, 2}}},
    /** Bifurcation properties for x4x4x4x4 */
    {4, {{0, 4, 0}, {4, 4, 1}, {8, 4, 2}, {12, 4, 3}}},
    /** Bifurcation properties for x2x2x2x2x2x2x2x2 */
    {8,
     {{0, 2, 0},
      {2, 2, 1},
      {4, 2, 2},
      {6, 2, 3},
      {8, 2, 4},
      {10, 2, 5},
      {12, 2, 6},
      {14, 2, 7}}},
    /** Bifurcation properties for x8x4x2x2 */
    {4, {{0, 2, 0}, {2, 2, 1}, {4, 4, 2}, {8, 8, 3}}},
    /** Bifurcation properties for x8x2x2x4 */
    {4, {{0, 4, 0}, {4, 2, 1}, {6, 2, 2}, {8, 8, 3}}},
    /** Bifurcation properties for x2x2x4x8 */
    {4, {{0, 8, 0}, {8, 4, 1}, {12, 2, 2}, {14, 2, 3}}},
    /** Bifurcation properties for x4x2x2x8 */
    {4, {{0, 8, 0}, {8, 2, 1}, {10, 2, 2}, {12, 4, 3}}},
    /** Bifurcation properties for x2x2x2x2x8 */
    {5, {{0, 8, 0}, {8, 2, 1}, {10, 2, 2}, {12, 2, 3}, {14, 2, 4}}},
    /** Bifurcation properties for x8x2x2x2x2 */
    {5, {{0, 2, 0}, {2, 2, 1}, {4, 2, 2}, {6, 2, 3}, {8, 8, 4}}},
    /** Bifurcation properties for x2x2x4x4x4 */
    {5, {{0, 4, 0}, {4, 4, 1}, {8, 4, 2}, {12, 2, 3}, {14, 2, 4}}},
    /** Bifurcation properties for x4x2x2x4x4 */
    {5, {{0, 4, 0}, {4, 4, 1}, {8, 2, 2}, {10, 2, 3}, {12, 4, 4}}},
    /** Bifurcation properties for x4x4x2x2x4 */
    {5, {{0, 4, 0}, {4, 2, 1}, {6, 2, 2}, {8, 4, 3}, {12, 4, 4}}},
    /** Bifurcation properties for x4x4x4x2x2 */
    {5, {{0, 2, 0}, {2, 2, 1}, {4, 4, 2}, {8, 4, 3}, {12, 4, 4}}},
    /** Bifurcation properties for x2x2x2x2x4x4 */
    {6, {{0, 4, 0}, {4, 4, 1}, {8, 2, 2}, {10, 2, 3}, {12, 2, 4}, {14, 2, 5}}},
    /** Bifurcation properties for x2x2x4x2x2x4 */
    {6, {{0, 4, 0}, {4, 2, 1}, {6, 2, 2}, {8, 4, 3}, {12, 2, 4}, {14, 2, 5}}},
    /** Bifurcation properties for x4x2x2x2x2x4 */
    {6, {{0, 4, 0}, {4, 2, 1}, {6, 2, 2}, {8, 2, 3}, {10, 2, 4}, {12, 4, 5}}},
    /** Bifurcation properties for x2x2x4x4x2x2 */
    {6, {{0, 2, 0}, {2, 2, 1}, {4, 4, 2}, {8, 4, 3}, {12, 2, 4}, {14, 2, 5}}},
    /** Bifurcation properties for x4x2x2x4x2x2 */
    {6, {{0, 2, 0}, {2, 2, 1}, {4, 4, 2}, {8, 2, 3}, {10, 2, 4}, {12, 4, 5}}},
    /** Bifurcation properties for x4x4x2x2x2x2 */
    {6, {{0, 2, 0}, {2, 2, 1}, {4, 2, 2}, {6, 2, 3}, {8, 4, 4}, {12, 4, 5}}},
    /** Bifurcation properties for x2x2x2x2x2x2x4 */
    {7,
     {{0, 4, 0},
      {4, 2, 1},
      {6, 2, 2},
      {8, 2, 3},
      {10, 2, 4},
      {12, 2, 5},
      {14, 2, 6}}},
    /** Bifurcation properties for x2x2x2x2x4x2x2 */
    {7,
     {{0, 2, 0},
      {2, 2, 1},
      {4, 4, 2},
      {8, 2, 3},
      {10, 2, 4},
      {12, 2, 5},
      {14, 2, 6}}},
    /** Bifurcation properties for x2x2x4x2x2x2x2 */
    {7,
     {{0, 2, 0},
      {2, 2, 1},
      {4, 2, 2},
      {6, 2, 3},
      {8, 4, 4},
      {12, 2, 5},
      {14, 2, 6}}},
    /** Bifurcation properties for x4x2x2x2x2x2x2 */
    {7,
     {{0, 2, 0},
      {2, 2, 1},
      {4, 2, 2},
      {6, 2, 3},
      {8, 2, 4},
      {10, 2, 5},
      {12, 4, 6}}},
    /** Bifurcation properties for x4x4 */
    {2, {{0, 4, 0}, {4, 4, 1}}},
    /** Bifurcation properties for x2x2x4 */
    {3, {{0, 4, 0}, {4, 2, 1}, {6, 2, 2}}},
    /** Bifurcation properties for x4x2x2 */
    {3, {{0, 2, 0}, {2, 2, 1}, {4, 4, 2}}},
    /** Bifurcation properties for x2x2x2x2 */
    {4, {{0, 2, 0}, {2, 2, 1}, {4, 2, 2}, {6, 2, 3}}},
    /** Bifurcation properties for x2x2 */
    {2, {{0, 2, 0}, {2, 2, 1}}},
    /** Bifurcation properties for x4x8x4 */
    {3, {{0, 4, 0}, {4, 8, 1}, {12, 4, 2}}},
    /** Bifurcation properties for x2 */
    {1, {{0, 2, 0}}}};

char* deviceTypes[3] = {"PT5", "PT4", "ET4"};

/**
 * @brief Read FW version info from the Main Micro space
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  offset  Offset inside FW struct for member
 * @param[in,out]  dataVal - data captured from registeres
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadFwVersion(AriesI2CDriverType* i2cDriver, int offset,
                                  uint8_t* dataVal)
{
    AriesErrorType rc;
    int addr = ARIES_MAIN_MICRO_FW_INFO + offset;
    if (offset == ARIES_MM_FW_VERSION_BUILD)
    {
        rc = ariesReadBlockDataMainMicroIndirect(i2cDriver, addr, 2, dataVal);
        CHECK_SUCCESS(rc);
    }
    else
    {
        rc = ariesReadByteDataMainMicroIndirect(i2cDriver, addr, dataVal);
        CHECK_SUCCESS(rc);
    }
    return ARIES_SUCCESS;
}

int ariesFirmwareIsAtLeast(AriesDeviceType* device, uint8_t major,
                           uint8_t minor, uint16_t build)
{
    if ((device->fwVersion.major == 0) && (device->fwVersion.minor == 0) &&
        (device->fwVersion.build == 0))
    {
        return false;
    }
    else if ((device->fwVersion.major > major) ||
             ((device->fwVersion.major == major) &&
              (device->fwVersion.minor > minor)) ||
             ((device->fwVersion.major == major) &&
              (device->fwVersion.minor == minor) &&
              (device->fwVersion.build >= build)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief I2C Master init for EEPROM Write-Thru
 */
AriesErrorType ariesI2CMasterInit(AriesI2CDriverType* i2cDriver)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t dataWord[2] = {0};

    // Disable I2C master
    dataByte[0] = 0;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x6c, 1, dataByte);
    CHECK_SUCCESS(rc);
    // I2C Control register, 1MHz speed
    dataWord[0] = 0xe5;
    dataWord[1] = 0xf;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x0, 2, dataWord);
    CHECK_SUCCESS(rc);
    // Target Address Register
    dataByte[0] = 0x50;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x04, 1, dataByte);
    CHECK_SUCCESS(rc);
    // Receive FIFO Threshold Register
    dataByte[0] = 0;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x38, 1, dataByte);
    CHECK_SUCCESS(rc);
    // Transmit FIFO Threshold Register
    dataByte[0] = 4;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x3c, 1, dataByte);
    CHECK_SUCCESS(rc);
    // SDA Hold Time, 100ns
    dataByte[0] = 100;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x7c, 1, dataByte);
    CHECK_SUCCESS(rc);
    // Enable I2C master
    dataByte[0] = 1;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x6c, 1, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Write to I2C Master ctrl register
 */
AriesErrorType ariesI2CMasterWriteCtrlReg(AriesI2CDriverType* i2cDriver,
                                          uint32_t address,
                                          uint8_t lengthDataBytes,
                                          uint8_t* values)
{
    AriesErrorType rc;
    uint8_t addr[1] = {0};
    uint8_t tmpVal[1] = {0};
    addr[0] = address;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, addr);
    CHECK_SUCCESS(rc);
    uint8_t dataBytes[4] = {0};
    uint16_t dataAddress[4] = {
        ARIES_I2C_MST_DATA0_ADDR, ARIES_I2C_MST_DATA1_ADDR,
        ARIES_I2C_MST_DATA2_ADDR, ARIES_I2C_MST_DATA3_ADDR};

    int i = 0;
    for (i = 0; i < lengthDataBytes; i++)
    {
        dataBytes[i] = values[i];
    }

    for (i = 0; i < 4; i++)
    {
        tmpVal[0] = dataBytes[i];
        rc = ariesWriteByteData(i2cDriver, dataAddress[i], tmpVal);
        CHECK_SUCCESS(rc);
    }

    // self.wr_csr_cmd(1)
    uint8_t cmd[1] = {0};
    cmd[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, cmd);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Set I2C Master frequency
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  frequencyHz      I2C Master frequency in Hz
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterSetFrequency(AriesI2CDriverType* i2cDriver,
                                          int frequencyHz)
{
    AriesErrorType rc;

    // Check if desired frequency is within supported range
    // return invalid arg error if not
    if (frequencyHz > 1000000)
    {
        ASTERA_ERROR("Cannot set I2C Master frequency greater than 1MHz");
        return ARIES_INVALID_ARGUMENT;
    }
    else if (frequencyHz < 400000)
    {
        ASTERA_ERROR("Cannot set I2C Master frequency less than 400KHz");
        return ARIES_INVALID_ARGUMENT;
    }

    int defaultSclLowCnt = 0x28a;
    int defaultSclHighCnt = 0x12c;
    int defaultFreqHz = 935000;

    int newSclLowCnt = (defaultFreqHz / frequencyHz) * defaultSclLowCnt;
    int newSclHighCnt = (defaultFreqHz / frequencyHz) * defaultSclHighCnt;

    uint8_t dataWord[2] = {0};
    uint8_t dataByte[1] = {0};

    // Reset I2C IP
    // self.csr.misc.hw_rst = 0x200
    dataWord[0] = 0x0;
    dataWord[1] = 0x2;
    rc = ariesWriteWideRegister(i2cDriver, ARIES_HW_RST_ADDR, 2, dataWord);
    CHECK_SUCCESS(rc);

    // Set IC_ENABLE=0 to allow changing the LCNT/HCNT settings
    // self.csr.muc.al_main_ext_csr.i2c_mst_addr   = 0x6c
    dataByte[0] = 0x6c;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_0 = 0
    dataByte[0] = 0x0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_cmd    = 1
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Unreset I2C IP
    // self.csr.misc.hw_rst = 0x0
    dataWord[0] = 0x0;
    dataWord[1] = 0x0;
    rc = ariesWriteWideRegister(i2cDriver, ARIES_HW_RST_ADDR, 2, dataWord);
    CHECK_SUCCESS(rc);

    // Set IC_FS_SCL_HCNT
    // self.csr.muc.al_main_ext_csr.i2c_mst_addr   = 0x1c
    dataByte[0] = 0x1c;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_0 = new_fs_scl_hcnt & 0xff
    dataByte[0] = newSclHighCnt & 0xff;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_1 = (new_fs_scl_hcnt >> 8) &
    // 0xff
    dataByte[0] = (newSclHighCnt >> 8) & 0xff;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_cmd    = 1
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Set IC_FS_SCL_LCNT
    // self.csr.muc.al_main_ext_csr.i2c_mst_addr   = 0x20
    dataByte[0] = 0x20;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_0 = new_fs_scl_lcnt & 0xff
    dataByte[0] = newSclLowCnt & 0xff;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_1 = (new_fs_scl_lcnt >> 8) &
    // 0xff
    dataByte[0] = (newSclLowCnt >> 8) & 0xff;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_cmd    = 1
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Set IC_ENABLE=1
    // self.csr.muc.al_main_ext_csr.i2c_mst_addr   = 0x6c
    dataByte[0] = 0x6c;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_data_0 = 1
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    // self.csr.muc.al_main_ext_csr.i2c_mst_cmd    = 1
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Get the location where the EEPROM image ends, which is after a
 * specific sequence - a55aa555aff. This can be used to help reduce the overall
 * fw load time
 *
 * @param[in]  data  Full EEPROM image as a byte array
 * @param[in]  numBytes  Number of bytes to write (<= 256k)
 * @return     location - Index until which we have to write
 */
int ariesGetEEPROMImageEnd(uint8_t* data)
{
    int dataIdx = 0;
    int seqIdx = 0;
    int location;

    uint8_t sequence[11] = {0xa5, 0x5a, 0xa5, 0x5a, 0xff, 0x0,
                            0x0,  0x0,  0x0,  0xff, 0xff};
    int seqLen = 11;

    while ((dataIdx < ARIES_EEPROM_MAX_NUM_BYTES) && (seqIdx < seqLen))
    {
        // Increment both pointers if element matches
        if (data[dataIdx] == sequence[seqIdx])
        {
            dataIdx++;
            seqIdx++;
            // Check if sequence is fully traversed
            if (seqIdx == seqLen)
            {
                location = dataIdx;
                return location;
            }
        }
        else
        {
            dataIdx = dataIdx - seqIdx + 1;
            seqIdx = 0;
        }
    }
    // If we cant find the end return max num of bytes
    return ARIES_EEPROM_MAX_NUM_BYTES;
}

/**
 * @brief Write multiple blocks of data to the EEPROM with help from
 * Main Micro
 *
 * @param[in]  device      Aries Device struct
 * @param[in]  address     EEPROM address
 * @param[in]  numBytes    Number of bytes to write (<= 256k)
 * @param[in]  values      EEPROM data to be written as a byte array
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterMultiBlockWrite(AriesDeviceType* device,
                                             uint16_t address, int numBytes,
                                             uint8_t* values)
{
    uint8_t dataByte[1] = {0};
    // Data is arranged in four-byte chunks
    uint8_t dataBytes[4] = {0};
    uint8_t addr15To8;
    uint8_t addr7To0;
    AriesErrorType rc;

    // IC Data command
    dataByte[0] = 0x10;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);

    // Prepare Flag Byte
    dataByte[0] = 0;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_DATA1_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);

    // Send Address
    addr15To8 = (address >> 8) & 0xff;
    addr7To0 = address & 0xff;
    dataByte[0] = addr15To8;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_DATA0_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_CMD_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = addr7To0;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_DATA0_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_I2C_MST_CMD_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);

    int numIters = numBytes / device->eeprom.blockWriteSize;
    int iterIdx;
    int numBlocks = device->eeprom.blockWriteSize / 4; // 4-byte blocks
    int blockIdx;
    int byteIdx;

    int offset = 0;
    uint8_t cmd;
    int try;
    int numTries = 30;
    bool mmBusy = false;

    for (iterIdx = 0; iterIdx < numIters; iterIdx++)
    {
        // determine MM-assist command
        cmd = ARIES_MM_EEPROM_WRITE_REG_CODE;
        if (iterIdx == (numIters - 1))
        {
            cmd = ARIES_MM_EEPROM_WRITE_END_CODE;
        }
        cmd = cmd | device->eeprom.blockCmdModifier;

        // Write data
        for (blockIdx = 0; blockIdx < numBlocks; blockIdx++)
        {
            // prepare the data
            for (byteIdx = 0; byteIdx < 4; byteIdx++)
            {
                dataBytes[byteIdx] = values[(offset + blockIdx * 4 + byteIdx)];
            }
            // write the data to Retimer holding registers
            rc = ariesWriteBlockData(
                device->i2cDriver, device->eeprom.blockBaseAddr + 4 * blockIdx,
                4, dataBytes);
            CHECK_SUCCESS(rc);
        }

        // Write cmd
        dataByte[0] = cmd;
        rc = ariesWriteByteData(device->i2cDriver,
                                ARIES_MM_EEPROM_ASSIST_CMD_ADDR, dataByte);
        CHECK_SUCCESS(rc);

        // Verify Command returned back to zero
        mmBusy = true;
        for (try = 0; try < numTries; try++)
        {
            rc = ariesReadByteData(device->i2cDriver,
                                   ARIES_MM_EEPROM_ASSIST_CMD_ADDR, dataByte);
            CHECK_SUCCESS(rc);
            if (dataByte[0] == 0)
            {
                mmBusy = false;
                break;
            }
            usleep(ARIES_MM_STATUS_TIME);
        }

        // If status not reset to 0, return BUSY error
        if (mmBusy)
        {
            ASTERA_TRACE(
                "ERROR: Main Micro busy writing data block to EEPROM. Did not commit write");
            return ARIES_EEPROM_MM_STATUS_BUSY;
        }

        offset += device->eeprom.blockWriteSize;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Rewrite a byte of data and verify it got written to the EEPROM with
 * help from Main Micro
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  address    EEPROM address to rewrite and verify
 * @param[in]  value      Data to write to EEPROM (1 byte)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterRewriteAndVerifyByte(AriesI2CDriverType* i2cDriver,
                                                  int address, uint8_t* value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Write byte
    rc = ariesI2CMasterSendByteBlockData(i2cDriver, address, 1, value);
    CHECK_SUCCESS(rc);
    usleep(ARIES_I2C_MASTER_WRITE_DELAY);

    rc = ariesI2CMasterSendAddress(i2cDriver, address);
    CHECK_SUCCESS(rc);

    // Read byte
    dataByte[0] = 0x3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    usleep(ARIES_I2C_MASTER_CMD_RST);
    dataByte[0] = 0x0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesReadByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    if (value[0] == dataByte[0])
    {
        ASTERA_INFO("        Re-write succeeded");
        rc = ARIES_SUCCESS;
    }
    else
    {
        ASTERA_INFO("        Re-write failed. Expected %d but got %d", value[0],
                    dataByte[0]);
        rc = ARIES_EEPROM_VERIFY_FAILURE;
    }

    return rc;
}

/**
 * @brief Uses I2C master to set the address pointer in the EEPROM
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  address    EEPROM address to send
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterSendAddress(AriesI2CDriverType* i2cDriver,
                                         int address)
{
    uint8_t dataByte[1] = {0};
    uint8_t addr15To8;
    uint8_t addr7To0;
    AriesErrorType rc;

    dataByte[0] = 0x10;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Prepare Flag Byte
    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Program address
    addr15To8 = (address >> 8) & 0xff;
    addr7To0 = address & 0xff;
    dataByte[0] = addr15To8;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = addr7To0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Uses I2C master to write multiple bytes to EEPROM
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  address    EEPROM address to send
 * @param[in]  numBytes   Number of bytes to be sent
 * @param[in]  value      Data bytes to send
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterSendByteBlockData(AriesI2CDriverType* i2cDriver,
                                               int address, int numBytes,
                                               uint8_t* value)
{
    // Write CSR Address
    uint8_t dataByte[1] = {0};
    uint8_t addr15To8;
    uint8_t addr7To0;
    int byteIndex = 0;
    AriesErrorType rc;

    dataByte[0] = 0x10;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Prepare Flag Byte
    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    addr15To8 = (address >> 8) & 0xff;
    addr7To0 = address & 0xff;
    dataByte[0] = addr15To8;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = addr7To0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Write all data bytes except last
    for (byteIndex = 0; byteIndex < (numBytes - 1); byteIndex++)
    {
        dataByte[0] = value[byteIndex];
        rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
        CHECK_SUCCESS(rc);
        dataByte[0] = 1;
        rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
        CHECK_SUCCESS(rc);
    }

    // Write last byte
    dataByte[0] = 2;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = value[(numBytes - 1)];
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Uses I2C master to write a byte to EEPROM
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  value      Data byte
 * @param[in]  flag       I2C flag
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterSendByte(AriesI2CDriverType* i2cDriver,
                                      uint8_t* value, int flag)
{
    uint8_t dataByte[1] = {0};
    AriesErrorType rc;

    dataByte[0] = 0x10;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, value);
    CHECK_SUCCESS(rc);
    dataByte[0] = flag << 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Uses I2C master to read multiple bytes to EEPROM
 *
 * @param[in]  device      Aries Device struct
 * @param[out]  dataBytes  Data bytes recieved
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterReceiveByteBlock(AriesDeviceType* device,
                                              uint8_t* dataBytes)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    // Data is transfered in 4-byte burst transactions
    uint8_t data[4] = {0};
    bool mmBusy;

    // Read data from EEPROM and terminate transaction
    dataByte[0] = ARIES_MM_EEPROM_READ_END_CODE |
                  device->eeprom.blockCmdModifier;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_MM_EEPROM_ASSIST_CMD_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);

    usleep(ARIES_MM_READ_CMD_WAIT);

    int numTries = 30;
    int tryIndex;
    mmBusy = true;
    for (tryIndex = 0; tryIndex < numTries; tryIndex++)
    {
        rc = ariesReadByteData(device->i2cDriver,
                               ARIES_MM_EEPROM_ASSIST_CMD_ADDR, dataByte);
        CHECK_SUCCESS(rc);
        if (dataByte[0] == 0)
        {
            mmBusy = false;
            break;
        }
        usleep(ARIES_MM_STATUS_TIME);
    }

    // If status not reset to 0, return BUSY error
    if (mmBusy)
    {
        ASTERA_ERROR("ERROR: Main Micro busy. Read data not ready");
        return ARIES_EEPROM_MM_STATUS_BUSY;
    }

    int byteIdx;
    int dataByteIdx;
    for (byteIdx = 0; byteIdx < device->eeprom.blockWriteSize; byteIdx += 4)
    {
        rc = ariesReadBlockData(device->i2cDriver,
                                (device->eeprom.blockBaseAddr + byteIdx), 4,
                                data);
        CHECK_SUCCESS(rc);
        for (dataByteIdx = 0; dataByteIdx < 4; dataByteIdx++)
        {
            dataBytes[(byteIdx + dataByteIdx)] = data[dataByteIdx];
        }
    }
    return ARIES_SUCCESS;
}

/**
 * @brief Calculate checksum of this EEPROM block
 *
 * @param[in]  i2cDriver  Aries i2c driver
 * @param[in]  checksum   Checksum value returned by function
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesI2CMasterGetChecksum(AriesDeviceType* device,
                                         uint16_t blockEnd, uint32_t* checksum)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t dataBytes[4] = {0};
    int try;
    uint8_t byteIdx;
    uint8_t dataByteIdx;
    int numTries = 500;
    uint8_t commandCode;

    if (blockEnd != 0)
    {
        // Set up end index for partial checksum calculation
        dataBytes[0] = blockEnd & 0xff;
        dataBytes[1] = (blockEnd >> 8) & 0xff;
        dataBytes[2] = 0;
        dataBytes[3] = 0;
        rc = ariesWriteBlockData(device->i2cDriver,
                                 device->eeprom.blockBaseAddr, 4, dataBytes);
        CHECK_SUCCESS(rc);
        commandCode = ARIES_MM_EEPROM_CHECKSUM_PARTIAL_CODE;
    }
    else
    {
        // Full-block checksum
        commandCode = ARIES_MM_EEPROM_CHECKSUM_CODE;
    }

    bool mmBusy = true;
    dataByte[0] = commandCode | device->eeprom.blockCmdModifier;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_MM_EEPROM_ASSIST_CMD_ADDR,
                            dataByte);
    CHECK_SUCCESS(rc);

    sleep(ARIES_MM_CALC_CHECKSUM_WAIT);

    for (try = 0; try < numTries; try++)
    {
        rc = ariesReadByteData(device->i2cDriver,
                               ARIES_MM_EEPROM_ASSIST_CMD_ADDR, dataByte);
        CHECK_SUCCESS(rc);
        if (dataByte[0] == 0)
        {
            mmBusy = false;
            break;
        }
        usleep(ARIES_MM_CALC_CHECKSUM_TRY_TIME);
    }

    if (mmBusy)
    {
        ASTERA_ERROR(
            "ERROR: Main Micro busy calculating block checksum. Read data not ready");
        return ARIES_EEPROM_MM_STATUS_BUSY;
    }

    // Compute checksum
    *checksum = 0;

    for (byteIdx = 0; byteIdx < ARIES_EEPROM_MM_BLOCK_CHECKSUM_WRITE_SIZE;
         byteIdx += 4)
    {
        rc = ariesReadBlockData(device->i2cDriver,
                                (device->eeprom.blockBaseAddr + byteIdx), 4,
                                dataBytes);
        CHECK_SUCCESS(rc);
        for (dataByteIdx = 0; dataByteIdx < 4; dataByteIdx++)
        {
            *checksum |= (dataBytes[dataByteIdx]
                          << (8 * (dataByteIdx + byteIdx)));
        }
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesI2CMasterReceiveByte(AriesI2CDriverType* i2cDriver,
                                         uint8_t* value)
{
    uint8_t dataByte[1] = {0};
    AriesErrorType rc;

    dataByte[0] = 0x10;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0x3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    usleep(ARIES_I2C_MASTER_CMD_RST);
    dataByte[0] = 0x0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesReadByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, value);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType
    ariesI2CMasterReceiveContinuousByte(AriesI2CDriverType* i2cDriver,
                                        uint8_t* value)
{
    uint8_t dataByte[1] = {0};
    AriesErrorType rc;

    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesReadByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, value);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesI2CMasterSetPage(AriesI2CDriverType* i2cDriver, int page)
{
    uint8_t tar[1] = {0};
    AriesErrorType rc;

    // Power-on default value is 0x50
    // rc = ariesReadByteData(i2cDriver, 0xd0c, tar);
    // CHECK_SUCCESS(rc);
    tar[0] = 0x50 | (page & 3);

    uint8_t dataByte[1] = {0};

    dataByte[0] = 0;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x6c, 1, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x04, 1, tar);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesI2CMasterWriteCtrlReg(i2cDriver, 0x6c, 1, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Get temp calibration codes, lot ID, and chip ID from eFuse.
 *
 * @param[in]  device   Aries Device struct
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetTempCalibrationCodes(AriesDeviceType* device)
{
    AriesErrorType rc;
    AriesErrorType lc;
    uint8_t dataByte[1] = {0};
    uint8_t dataBytes5[5] = {0};
    uint8_t invalid;
    uint8_t flag;
    uint8_t offset;
    uint8_t calCode;
    uint8_t deviceType;
    uint8_t deviceWidth;
    uint8_t eFuseCPChuckTempC;

    // Only read from eFuse once - if we haven't done so already
    if (device->eFuseRead == 0)
    {
        // eFuse read procedure
        lc = ariesLock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        // 1. Switch to refclk/8 clock for TCK
        // self.csr.misc.efuse_cntl.sms_clk_sel = 1
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // Assert bit 25
        dataBytes5[3] |= (1 << 1);
        rc = ariesWriteWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                    dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // 2. Assert efuse_load
        // self.csr.misc.sms_efuse_cntl.sms_efuse_load = 1
        rc = ariesReadByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                               dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // Assert bit 7
        dataByte[0] |= (1 << 7);
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // 3. Assert smart_test
        // self.csr.misc.efuse_cntl.smart_test = 1
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // Assert bit 24
        dataBytes5[3] |= (1 << 0);
        rc = ariesWriteWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                    dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // 4. De-assert smart_test
        // self.csr.misc.efuse_cntl.smart_test = 0
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // De-assert bit 24
        dataBytes5[3] &= ~(1 << 0);
        rc = ariesWriteWideRegister(device->i2cDriver, ARIES_EFUSE_CNTL, 5,
                                    dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // 5. De-assert efuse_load
        // self.csr.misc.sms_efuse_cntl.sms_efuse_load = 0
        rc = ariesReadByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                               dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // De-assert bit 7
        dataByte[0] &= ~(1 << 7);
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Read eFuse “primary page invalid” bit and adjust offset accordingly
        // Set address
        dataByte[0] = 63;
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // Read data
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS, 2,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        invalid = dataBytes5[0];

        if (invalid & 0x80)
        {
            offset = 64;
        }
        else
        {
            offset = 0;
        }

        // Determine calibration codes
        // Set address
        dataByte[0] = 48 + offset;
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        // Read data
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS, 2,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        flag = dataBytes5[0];

        // Compute PMA A calibration codes
        int qs;
        for (qs = 0; qs < 4; qs++)
        {
            if (flag & 0x4)
            {
                dataByte[0] = 34 + (qs * 4) + offset;
                rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                        dataByte);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(device->i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                rc = ariesReadWideRegister(device->i2cDriver,
                                           ARIES_SMS_EFUSE_STS, 2, dataBytes5);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(device->i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                calCode = dataBytes5[0];
                if (calCode == 0)
                {
                    calCode = 84;
                }
            }
            else
            {
                calCode = 84;
            }
            device->tempCalCodePmaA[qs] = calCode;
        }

        // Compute PMA B calibration codes
        for (qs = 0; qs < 4; qs++)
        {
            if (flag & 0x04)
            {
                dataByte[0] = 32 + (qs * 4) + offset;
                rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                        dataByte);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(device->i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                rc = ariesReadWideRegister(device->i2cDriver,
                                           ARIES_SMS_EFUSE_STS, 2, dataBytes5);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(device->i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                calCode = dataBytes5[0];
                if (calCode == 0)
                {
                    calCode = 84;
                }
            }
            else
            {
                calCode = 84;
            }
            device->tempCalCodePmaB[qs] = calCode;
        }

        // Calcualte the average PMA calibration code
        if (device->partNumber == ARIES_PTX16)
        {
            device->tempCalCodeAvg =
                (device->tempCalCodePmaA[0] + device->tempCalCodePmaA[1] +
                 device->tempCalCodePmaA[2] + device->tempCalCodePmaA[3] +
                 device->tempCalCodePmaB[0] + device->tempCalCodePmaB[1] +
                 device->tempCalCodePmaB[2] + device->tempCalCodePmaB[3] +
                 8 / 2) /
                8; // Add denominator/2 to cause integer rounding
        }
        else if (device->partNumber == ARIES_PTX08)
        {
            device->tempCalCodeAvg =
                (device->tempCalCodePmaA[1] + device->tempCalCodePmaA[2] +
                 device->tempCalCodePmaB[1] + device->tempCalCodePmaB[2] +
                 4 / 2) /
                4; // Add denominator/2 to cause integer rounding
        }
        else
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return ARIES_INVALID_ARGUMENT;
        }

        // Read 12-byte chip ID
        int b = 0;
        for (b = 0; b < 12; b++)
        {
            // Chip ID starts at byte 0 in eFuse
            dataByte[0] = 0 + b + offset;
            rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                    dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(device->i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS,
                                       2, dataBytes5);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(device->i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            device->chipID[b] = dataBytes5[0];
        }

        // Read 6-byte lot number
        for (b = 0; b < 6; b++)
        {
            // Lot number starts at byte 16 in eFuse
            dataByte[0] = 16 + b + offset;
            rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                    dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(device->i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS,
                                       2, dataBytes5);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(device->i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            device->lotNumber[b] = dataBytes5[0];
        }

        // Read DUT part number
        dataByte[0] = 15 + offset;
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS, 2,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        deviceType = (dataBytes5[0] >> 6) & 0x3;
        deviceWidth = dataBytes5[0] & 0x3F;
        if (deviceType <= 2)
        {
            snprintf(device->fullPartNumber, 8, "%s", deviceTypes[deviceType]);
        }
        else
        {
            sprintf(device->fullPartNumber, "ptx");
        }
        if (deviceWidth == 1)
        {
            sprintf(device->fullPartNumber + 3, "161");
        }
        else if (deviceWidth == 2)
        {
            if (deviceType == 1)
            {
                sprintf(device->fullPartNumber + 3, "080");
            }
            else
            {
                sprintf(device->fullPartNumber + 3, "081");
            }
        }
        else
        {
            if (device->partNumber == ARIES_PTX08)
            {
                sprintf(device->fullPartNumber + 3, "08xx");
            }
            else
            {
                sprintf(device->fullPartNumber + 3, "16xx");
            }
        }

        // CP Program Version is byte 49 in eFuse
        dataByte[0] = 49 + offset;
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS, 2,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        device->eFuseCPVersion = dataBytes5[0];

        // Wafer sort chuck temperature is byte 62 in eFuse
        // CP Program Version 14 and later store chuck temperature
        dataByte[0] = 62 + offset;
        rc = ariesWriteByteData(device->i2cDriver, ARIES_SMS_EFUSE_CNTL,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        rc = ariesReadWideRegister(device->i2cDriver, ARIES_SMS_EFUSE_STS, 2,
                                   dataBytes5);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(device->i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        eFuseCPChuckTempC = dataBytes5[0];
        // Use CP Program Version and eFuse chuck temperature to store temp
        // calibration reference temperature.
        if (device->eFuseCPVersion >= 14)
        {
            device->tempCalCodeRefTempC = eFuseCPChuckTempC;
        }
        else if (device->eFuseCPVersion >= 11)
        {
            device->tempCalCodeRefTempC = 85;
        }
        else
        {
            device->tempCalCodeRefTempC = 110;
        }

        // Done reading from eFuse
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        device->eFuseRead = 1;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Get max. temp reading across all PMAs
 *
 * @param[in]  device   Aries Device struct
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadPmaTempMax(AriesDeviceType* device)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};
    int adcCode;

    if (!ariesFirmwareIsAtLeast(device, 1, 1, 31))
    {
        return ARIES_TEMP_READ_NOT_READY;
    }

    rc = ariesReadWideRegister(device->i2cDriver,
                               ARIES_ALL_TIME_MAX_TEMP_ADC_CSR, 4, dataBytes);
    CHECK_SUCCESS(rc);
    adcCode = (dataBytes[3] << 24) + (dataBytes[2] << 16) +
              (dataBytes[1] << 8) + dataBytes[0];

    if (adcCode == 0 || adcCode >= 0x3ff)
    {
        return ARIES_TEMP_READ_NOT_READY;
    }

    device->maxTempC = ariesTsenseADCToDegrees(device, adcCode,
                                               device->tempCalCodeAvg);

    return ARIES_SUCCESS;
}

/**
 * @brief Get current avg. temp reading across all PMAs
 *
 * @param[in]  device   Aries Device struct
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadPmaAvgTemp(AriesDeviceType* device)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};
    int adcCode;

    if (!ariesFirmwareIsAtLeast(device, 1, 0, 42))
    {
        return ARIES_TEMP_READ_NOT_READY;
    }

    rc = ariesReadWideRegister(device->i2cDriver,
                               ARIES_CURRENT_AVG_TEMP_ADC_CSR, 4, dataBytes);
    CHECK_SUCCESS(rc);
    adcCode = (dataBytes[3] << 24) + (dataBytes[2] << 16) +
              (dataBytes[1] << 8) + dataBytes[0];

    if (adcCode == 0 || adcCode >= 0x3ff)
    {
        return ARIES_TEMP_READ_NOT_READY;
    }

    device->currentTempC = ariesTsenseADCToDegrees(device, adcCode,
                                                   device->tempCalCodeAvg);

    return ARIES_SUCCESS;
}

AriesErrorType ariesReadPmaAvgTempDirect(AriesDeviceType* device)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int side;
    int qs;
    int adcCode;
    uint8_t calCode;
    float temperature_C = 0;

    for (side = 0; side < 2; side++)
    {
        for (qs = 0; qs < 4; qs++)
        {
            // Unfreeze FW
            rc = ariesReadWordPmaIndirect(
                device->i2cDriver, side, qs,
                ARIES_PMA_RAWLANE_DIG_FSM_FSM_OVRD_CTL + 0x4000, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[1] |= 0x40;
            rc = ariesWriteWordPmaIndirect(
                device->i2cDriver, side, qs,
                ARIES_PMA_RAWLANE_DIG_FSM_FSM_OVRD_CTL + 0x4000, dataWord);
            CHECK_SUCCESS(rc);

            // Temp Sensor Enable
            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_ANA_BG, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] |= 0x08;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_ANA_BG, dataWord);
            CHECK_SUCCESS(rc);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] |= 0x60;
            dataWord[0] &= ~0x18;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                           dataWord);
            CHECK_SUCCESS(rc);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~0x18;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                           dataWord);
            CHECK_SUCCESS(rc);
            usleep(10000);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] |= 0x80;
            dataWord[0] &= ~0x04;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                           dataWord);
            CHECK_SUCCESS(rc);

            // Temp Sensor Read
            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] |= 0x02;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                           dataWord);
            CHECK_SUCCESS(rc);
            usleep(10000);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_DIG_RTUNE_STAT,
                                          dataWord);
            CHECK_SUCCESS(rc);

            adcCode = (dataWord[1] << 8) + dataWord[0];

            if (adcCode == 0 || adcCode >= 0x3ff)
            {
                return ARIES_TEMP_READ_NOT_READY;
            }

            if (side == 0)
            {
                calCode = device->tempCalCodePmaB[qs];
            }
            else
            {
                calCode = device->tempCalCodePmaA[qs];
            }

            temperature_C += ariesTsenseADCToDegrees(device, adcCode, calCode);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~0x02;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_DIG_RTUNE_DEBUG,
                                           dataWord);
            CHECK_SUCCESS(rc);

            // Temp Sensor Disable
            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_ANA_BG, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~0x08;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_ANA_BG, dataWord);
            CHECK_SUCCESS(rc);

            rc = ariesReadWordPmaIndirect(device->i2cDriver, side, qs,
                                          ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                          dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~0xe0;
            rc = ariesWriteWordPmaIndirect(device->i2cDriver, side, qs,
                                           ARIES_PMA_SUP_ANA_RTUNE_CTRL,
                                           dataWord);
            CHECK_SUCCESS(rc);

            // Freeze FW
            rc = ariesReadWordPmaIndirect(
                device->i2cDriver, side, qs,
                ARIES_PMA_RAWLANE_DIG_FSM_FSM_OVRD_CTL, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[1] &= ~0x40;
            rc = ariesWriteWordPmaIndirect(
                device->i2cDriver, side, qs,
                ARIES_PMA_RAWLANE_DIG_FSM_FSM_OVRD_CTL, dataWord);
            CHECK_SUCCESS(rc);
        }
    }

    // Take average of all 8 sensors
    temperature_C = temperature_C / 8;

    // Convert ACD code to deg C
    device->currentTempC = temperature_C;

    return ARIES_SUCCESS;
}

/**
 * @brief Enable thermal shutdown in Aries
 *
 * @param[in]  device   Aries Device struct
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesEnableThermalShutdown(AriesDeviceType* device)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};

    dataBytes[0] = 1;
    rc = ariesWriteWideRegister(device->i2cDriver, ARIES_EN_THERMAL_SHUTDOWN, 4,
                                dataBytes);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Disable thermal shutdown in Aries
 *
 * @param[in]  device   Aries Device struct
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesDisableThermalShutdown(AriesDeviceType* device)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};

    dataBytes[0] = 0;
    rc = ariesWriteWideRegister(device->i2cDriver, ARIES_EN_THERMAL_SHUTDOWN, 4,
                                dataBytes);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Get PMA Temp reading
 *
 * @param[in]  device   Aries Device struct
 * @param[in]  side     PMA side
 * @param[in]  qs       PMA Quad Slice
 * @param[in/out] temperature_C     PMA Temperature reading
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadPmaTemp(AriesDeviceType* device, int side, int qs,
                                float* temperature_C)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};

    uint32_t pmaCsr = ARIES_MAIN_MICRO_FW_INFO +
                      ARIES_MM_PMA_TJ_ADC_CODE_OFFSET;
    int adcCode;
    uint8_t calCode;

    rc = ariesReadBlockDataMainMicroIndirect(
        device->i2cDriver, (pmaCsr + (side * 8) + (qs * 2)), 2, dataWord);
    CHECK_SUCCESS(rc);

    adcCode = (dataWord[1] << 8) + dataWord[0];

    if (adcCode == 0 || adcCode >= 0x3ff)
    {
        return ARIES_TEMP_READ_NOT_READY;
    }

    if (side == 0)
    {
        calCode = device->tempCalCodePmaB[qs];
    }
    else
    {
        calCode = device->tempCalCodePmaA[qs];
    }

    *temperature_C = ariesTsenseADCToDegrees(device, adcCode, calCode);

    return ARIES_SUCCESS;
}

/**
 * @brief Get port orientation
 *
 * @param[in]  i2cDriver   i2c Driver struct
 * @param[out] orientation   port orientation
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetPortOrientation(AriesDeviceType* device,
                                       int* orientation)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};

    rc = ariesReadWideRegister(device->i2cDriver, 0x10, 4, dataBytes);
    CHECK_SUCCESS(rc);

    // Oriention Val is bit 8
    *orientation = dataBytes[1] & (0x01);

    return ARIES_SUCCESS;
}

AriesErrorType ariesSetPortOrientation(AriesDeviceType* device,
                                       uint8_t orientation)
{
    AriesErrorType rc;
    uint8_t dataBytes[4] = {0};

    rc = ariesReadWideRegister(device->i2cDriver, 0x10, 4, dataBytes);
    CHECK_SUCCESS(rc);

    // Orientation is bit 8 in this reg
    dataBytes[1] &= 0xfe;
    dataBytes[1] |= (orientation & 0x1);

    rc = ariesWriteWideRegister(device->i2cDriver, 0x10, 4, dataBytes);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Get PMA ID
 *
 * @param[in]  absLane   Absolute lane number
 * @return     int - pma id.
 */
int ariesGetPmaNumber(int absLane)
{
    int pmaNum;
    pmaNum = absLane / 4;
    return pmaNum;
}

/**
 * @brief Get PMA lane ID
 *
 * @param[in]  absLane   Absolute lane number
 * @return     int - pma lane id.
 */
int ariesGetPmaLane(int absLane)
{
    int pmaLane;
    pmaLane = absLane % 4;
    return pmaLane;
}

/**
 * @brief Get absolute Path ID
 *
 * @param[in]  lane   start lane of link
 * @param[in]  direction   upstream/downsteram direction
 * @return     int - path id.
 */
int ariesGetPathID(int lane, int direction)
{
    int pathID;
    pathID = ((lane / 2) * 2) + direction;
    return pathID;
}

/**
 * @brief Get Path lane ID
 *
 * @param[in]  lane   start lane of link
 * @return     int - path lane id.
 */
int ariesGetPathLaneID(int lane)
{
    int pathLaneID;
    pathLaneID = lane % 2;
    return pathLaneID;
}

int ariesGetStartLane(AriesLinkType* link)
{
    int startLane;
    startLane = link->config.startLane;
    if (link->config.partNumber == ARIES_PTX08)
    {
        startLane += 4;
    }

    return startLane;
}

/**
 * @brief Dump PMA registers to a file
 *
 * @param device Aries Device struct
 * @param pma_addr Array of uint16_t register addresses to log
 * @param len Length of pma_addr array
 * @param filename File to write register dump to
 * @return AriesErrorType - Aries error code
 */
AriesErrorType ariesDumpPMARegs(AriesDeviceType* device, uint16_t* pma_addr,
                                int len, char* filename)
{
    AriesErrorType rc;
    FILE* fp;
    uint16_t dataWord;
    int i, side, qs;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        ASTERA_ERROR("Could not open file %s", filename);
        return ARIES_FAILURE;
    }
    if (device->partNumber == ARIES_PTX16)
    {
        fprintf(
            fp,
            "addr,pma_a_0,pma_a_1,pma_a_2,pma_a_3,pma_b_0,pma_b_1,pma_b_2,pma_b_3");
    }
    else
    {
        fprintf(fp, "addr,pma_a_1,pma_a_2,pma_b_1,pma_b_2");
    }

    for (i = 0; i < len; i++)
    {
        fprintf(fp, "\n0x%x", pma_addr[i]);
        if (device->partNumber == ARIES_PTX16)
        {
            for (side = 0; side < 2; side++)
            {
                for (qs = 0; qs < 4; qs++)
                {
                    rc = ariesReadWordPmaMainMicroIndirect(
                        device->i2cDriver, 1 - side, qs, pma_addr[i],
                        (uint8_t*)&dataWord);
                    if (rc != ARIES_SUCCESS)
                    {
                        fclose(fp);
                        return rc;
                    }
                    fprintf(fp, ",0x%x", dataWord);
                }
            }
        }
        else
        {
            for (side = 0; side < 2; side++)
            {
                for (qs = 1; qs < 3; qs++)
                {
                    rc = ariesReadWordPmaMainMicroIndirect(
                        device->i2cDriver, 1 - side, qs, pma_addr[i],
                        (uint8_t*)&dataWord);
                    if (rc != ARIES_SUCCESS)
                    {
                        fclose(fp);
                        return rc;
                    }
                    fprintf(fp, ",0x%x", dataWord);
                }
            }
        }
    }
    fclose(fp);
    return ARIES_SUCCESS;
}

void ariesGetQSPathInfo(int lane, int direction, int* qs, int* qsPath,
                        int* qsPathLane)
{
    int pathID = ariesGetPathID(lane, direction);
    *qs = pathID / 4;
    *qsPath = pathID % 4;
    *qsPathLane = lane % 2;
}

/**
 * @brief Get link termination status
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side    pma side - a (0) or b (1)
 * @param[in]  abslane    link lane number
 * @param[out] term    term value returned
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLinkRxTerm(AriesLinkType* link, int side, int absLane,
                                  int* term)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    AriesI2CDriverType* i2cDriver;
    i2cDriver = link->device->i2cDriver;

    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    // Read from PMAX_LANE0_DIG_ASIC_RX_OVRD_IN_3
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        i2cDriver, side, pmaNum, pmaLane, ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_3,
        dataWord);
    CHECK_SUCCESS(rc);

    // TERM_EN_OVRD_EN is bit 7
    uint8_t termEnOverrideEn = (dataWord[0] & 0x80) >> 7;
    // TERM_EN is bit 6
    uint8_t termEn = (dataWord[0] & 0x40) >> 6;

    if (termEnOverrideEn == 1)
    {
        *term = termEn;
    }
    else
    {
        // Read from LANE0_DIG_ASIC_RX_ASIC_IN_1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_LANE_DIG_ASIC_RX_ASIC_IN_1, dataWord);
        CHECK_SUCCESS(rc);
        // RX_TERM_EN is bit 2
        *term = (dataWord[0] & 0x04) >> 2;
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetLinkId(AriesBifurcationType bifMode, int lane,
                              int* linkNum)
{
    int i;
    for (i = 0; i < bifurcationModes[bifMode].numLinks; i++)
    {
        lane -= bifurcationModes[bifMode].links[i].linkWidth;
        if (lane < 0)
        {
            *linkNum = i;
            return ARIES_SUCCESS;
        }
    }

    return ARIES_FAILURE;
}

AriesErrorType ariesGetLinkCurrentSpeed(AriesLinkType* link, int lane,
                                        int direction, float* speed)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pcieGen;

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);

    // Compute actual address based on offsets above
    uint32_t address = qsOff + pathOff + ARIES_PATH_GLOBAL_CSR_OFFSET +
                       ARIES_GBL_CSR_MAC_PHY_RATE_AND_PCLK_RATE;

    rc = ariesReadByteData(link->device->i2cDriver, address, dataByte);
    CHECK_SUCCESS(rc);

    // rate val is last 3 bits of reg
    pcieGen = (dataByte[0] & 0x07) + 1;

    // Get speed from PCIE gen value
    switch (pcieGen)
    {
        case 1:
            *speed = 2.5;
            break;
        case 2:
            *speed = 5.0;
            break;
        case 3:
            *speed = 8.0;
            break;
        case 4:
            *speed = 16.0;
            break;
        case 5:
            *speed = 32.0;
            break;
        default:
            *speed = 0.0;
    }
    return ARIES_SUCCESS;
}

/**
 * @brief Get Logical Lane Num
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    link lane number
 * @param[in]  direction  link direction
 * @param[in/out] laneNum   logical lane number
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLogicalLaneNum(AriesLinkType* link, int lane,
                                      int direction, int* laneNum)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    uint32_t address = qsOff + pathOff + pathLaneOff + ARIES_LN_CAPT_NUM;

    rc = ariesReadByteData(link->device->i2cDriver, address, dataByte);
    CHECK_SUCCESS(rc);
    *laneNum = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief Get TX pre cursor value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    link lane number
 * @param[in]  direction  link direction
 * @param[in/out] txPre   tx pre cursor value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetTxPre(AriesLinkType* link, int lane, int direction,
                             int* txPre)
{
    AriesErrorType rc;
    uint8_t dataBytes3[3] = {0};
    int tmpVal;

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    // Compute Reg Offset
    uint32_t address = qsOff + pathOff + pathLaneOff +
                       ARIES_MAC_PHY_TXDEEMPH_OB;

    rc = ariesReadBlockData(link->device->i2cDriver, address, 3, dataBytes3);
    CHECK_SUCCESS(rc);
    tmpVal = (dataBytes3[0] + (dataBytes3[1] << 8) + (dataBytes3[2] << 16));
    *txPre = tmpVal & 0x3f;

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetPathHWState(AriesLinkType* link, int lane, int direction,
                                   int* state)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);

    uint32_t address = qsOff + pathOff + ARIES_RET_PTH_NEXT_STATE_OFFSET;

    rc = ariesReadBlockData(link->device->i2cDriver, address, 1, dataByte);
    CHECK_SUCCESS(rc);
    *state = dataByte[0];

    return ARIES_SUCCESS;
}

/**
 * @brief Get TX current cursor value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    link lane number
 * @param[in]  direction  link direction
 * @param[in/out] txCur   tx current cursor value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetTxCur(AriesLinkType* link, int lane, int direction,
                             int* txCur)
{
    AriesErrorType rc;
    uint8_t dataBytes3[3] = {0};
    int tmpVal;

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    // Compute Reg Offset
    uint32_t address = qsOff + pathOff + pathLaneOff +
                       ARIES_MAC_PHY_TXDEEMPH_OB;

    rc = ariesReadBlockData(link->device->i2cDriver, address, 3, dataBytes3);
    CHECK_SUCCESS(rc);
    tmpVal = (dataBytes3[0] + (dataBytes3[1] << 8) + (dataBytes3[2] << 16));
    *txCur = (tmpVal & 0xfc0) >> 6;

    return ARIES_SUCCESS;
}

/**
 * @brief Get TX port cursor value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    link lane number
 * @param[in]  direction  link direction
 * @param[in/out] txPst   tx pst value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetTxPst(AriesLinkType* link, int lane, int direction,
                             int* txPst)
{
    AriesErrorType rc;
    uint8_t dataBytes3[3] = {0};
    int tmpVal;

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    // Compute Reg Offset
    uint32_t address = qsOff + pathOff + pathLaneOff +
                       ARIES_MAC_PHY_TXDEEMPH_OB;

    rc = ariesReadBlockData(link->device->i2cDriver, address, 3, dataBytes3);
    CHECK_SUCCESS(rc);
    tmpVal = (dataBytes3[0] + (dataBytes3[1] << 8) + (dataBytes3[2] << 16));
    *txPst = (tmpVal & 0x3f000) >> 12;

    return ARIES_SUCCESS;
}

/**
 * @brief Get RX polarity code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    link lane number
 * @param[in]  direction  link direction
 * @param[in]  pinSet    location in pins array
 * @param[in/out] polarity    polarity code
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxPolarityCode(AriesLinkType* link, int lane,
                                      int direction, int pinSet, int* polarity)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    // Compute actual address based on offsets above
    uint32_t address = qsOff + pathOff + pathLaneOff + ARIES_MAC_RX_POLARITY;

    rc = ariesReadByteData(link->device->i2cDriver, address, dataByte);
    CHECK_SUCCESS(rc);
    *polarity = dataByte[0] & 0x1;

    // Check inversion value for rx pins on package
    int inversion;
    if (pinSet == 0)
    {
        inversion = link->device->pins[lane].pinSet1.rxPackageInversion;
    }
    else
    {
        inversion = link->device->pins[lane].pinSet2.rxPackageInversion;
    }
    if ((inversion == 1) && (*polarity == 1))
    {
        *polarity = 0;
    }
    else if ((inversion == 1) && (*polarity == 0))
    {
        *polarity = 1;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Get current RX att code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side    pma side - a (0) or b (1)
 * @param[in]  absLane    Absolute lane number
 * @param[in/out] code    rx att code
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxAttCode(AriesLinkType* link, int side, int absLane,
                                 int* code)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        link->device->i2cDriver, side, pmaNum, pmaLane,
        ARIES_PMA_LANE_DIG_RX_ADPTCTL_ATT_STATUS, dataWord);
    CHECK_SUCCESS(rc);

    // You need bits 7:0
    *code = dataWord[0] >> 5;
    return ARIES_SUCCESS;
}

/**
 * @brief Get current RX VGA Code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side    pma side - a (0) or b (1)
 * @param[in]  absLane    Absolute lane number
 * @param[in/out] boostCode    RX boost code
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxCtleBoostCode(AriesLinkType* link, int side,
                                       int absLane, int* boostCode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        link->device->i2cDriver, side, pmaNum, pmaLane,
        ARIES_PMA_LANE_DIG_RX_ADPTCTL_CTLE_STATUS, dataWord);
    CHECK_SUCCESS(rc);

    // Need bits 9:0
    *boostCode = (((dataWord[1] & 0x03) << 8) + dataWord[0]) >> 5;
    return ARIES_SUCCESS;
}

/**
 * @brief Get current RX VGA Code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side    pma side - a (0) or b (1)
 * @param[in]  absLane    Absolute lane number
 * @param[in/out] vgaCode    RX VGA code
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxVgaCode(AriesLinkType* link, int side, int absLane,
                                 int* vgaCode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        link->device->i2cDriver, side, pmaNum, pmaLane,
        ARIES_PMA_LANE_DIG_RX_ADPTCTL_VGA_STATUS, dataWord);
    CHECK_SUCCESS(rc);

    // Need bits 9:0
    *vgaCode = (((dataWord[1] & 0x03) << 8) + dataWord[0]) >> 6;
    return ARIES_SUCCESS;
}

/**
 * @brief Get current RX Boost Value (in dB)
 *
 * @param[in]  boostCode   boostCode
 * @param[in]  attValDb    att val (in db)
 * @param[in]  vgaCode    vga code
 * @return     float - rx Boost value
 */
float ariesGetRxBoostValueDb(int boostCode, float attValDb, int vgaCode)
{
    float AfeHfGain;
    float AfeLfGain;
    float boostValDb;

    float attVal = 1.5 + attValDb;
    float vgaVal = 0.9 * vgaCode;
    float tmpVal1;
    float tmpVal2;
    float boostVal;

    if (vgaCode <= 6)
    {
        tmpVal1 = 0.9 * (6 - vgaCode);
        if (boostCode <= 10)
        {
            tmpVal2 = 0.65 * boostCode;
        }
        else
        {
            tmpVal2 = 0.65 * 10;
        }
        // Find max between 2 values
        boostVal = tmpVal1;
        if (tmpVal1 < tmpVal2)
        {
            boostVal = tmpVal2;
        }
    }
    else
    {
        if (boostCode <= 10)
        {
            tmpVal2 = 0.65 * boostCode;
        }
        else
        {
            tmpVal2 = 0.65 * 10;
        }
        boostVal = tmpVal2;
    }
    AfeHfGain = attVal + vgaVal + boostVal;

    if (boostCode <= 10)
    {
        boostVal = 0;
    }
    else
    {
        boostVal = -0.65 * (boostCode - 10);
    }
    AfeLfGain = attValDb + vgaVal + boostVal;

    boostValDb = AfeHfGain - AfeLfGain;
    return boostValDb;
}

/**
 * @brief Get the current RX CTLE POLE code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side   PMA side
 * @param[in]  absLane    Absolute lane number
 * @param[in/out]  poleCode    POLE code captured
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxCtlePoleCode(AriesLinkType* link, int side,
                                      int absLane, int* poleCode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        link->device->i2cDriver, side, pmaNum, pmaLane,
        ARIES_PMA_LANE_DIG_RX_ADPTCTL_CTLE_STATUS, dataWord);
    CHECK_SUCCESS(rc);

    // Get bits 11:10
    *poleCode = (dataWord[1] & 0x0c) >> 2;
    return ARIES_SUCCESS;
}

/**
 * @brief Get the current RX Adapt IQ Value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side   PMA side
 * @param[in]  absLane    Absolute lane number
 * @param[in/out]  iqValue    Adapt IQ value captured
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxAdaptIq(AriesLinkType* link, int side, int absLane,
                                 int* iqValue)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    float curSpeed;
    rc = ariesGetLinkCurrentSpeed(link, absLane, side, &curSpeed);
    CHECK_SUCCESS(rc);

    if (curSpeed == 8.0)
    {
        // Check the RX_ADPT_IQ_VLD bit == 1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE, dataWord);
        CHECK_SUCCESS(rc);
        if ((dataWord[0] & 0x2) == 0x2)
        {
            // Read the Adapt IQ value
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                link->device->i2cDriver, side, pmaNum, pmaLane,
                ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ, dataWord);
            CHECK_SUCCESS(rc);
        }
        else
        {
            return ARIES_FAILURE;
        }
    }
    else if (curSpeed == 16.0)
    {
        // Check the RX_ADPT_IQ_VLD bit[1] is set
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE_B1, dataWord);
        CHECK_SUCCESS(rc);
        if ((dataWord[0] & 0x2) == 0x2)
        {
            // Read the Adapt IQ value
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                link->device->i2cDriver, side, pmaNum, pmaLane,
                ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ_B1, dataWord);
            CHECK_SUCCESS(rc);
        }
        else
        {
            return ARIES_FAILURE;
        }
    }
    else if (curSpeed == 32.0)
    {
        // Check the RX_ADPT_IQ_VLD bit == 1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE_B2, dataWord);
        CHECK_SUCCESS(rc);
        if ((dataWord[0] & 0x2) == 0x2)
        {
            // Read the Adapt IQ value
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                link->device->i2cDriver, side, pmaNum, pmaLane,
                ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ_B2, dataWord);
            CHECK_SUCCESS(rc);
        }
        else
        {
            return ARIES_FAILURE;
        }
    }
    else
    {
        dataWord[0] = 0;
        dataWord[1] = 0;
    }

    // Get bits 6:0
    *iqValue = dataWord[0] & 0x7f;
    return ARIES_SUCCESS;
}

/**
 * @brief Get the current RX Adapt IQ Value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side   PMA side
 * @param[in]  absLane    Absolute lane number
 * @param[in]  bank   Bank number
 * @param[in/out]  iqValue    Adapt IQ value captured
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxAdaptIqBank(AriesLinkType* link, int side, int absLane,
                                     int bank, int* iqValue)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    if (bank == 0)
    {
        // Read the Adapt IQ value
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ, dataWord);
        CHECK_SUCCESS(rc);
    }
    else if (bank == 1)
    {
        // Read the Adapt IQ value
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ_B1, dataWord);
        CHECK_SUCCESS(rc);
    }
    else if (bank == 2)
    {
        // Read the Adapt IQ value
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADPT_IQ_B2, dataWord);
        CHECK_SUCCESS(rc);
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Get bits 6:0
    *iqValue = dataWord[0] & 0x7f;
    return ARIES_SUCCESS;
}

/**
 * @brief Get the current RX Adapt IQ Value
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side   PMA side
 * @param[in]  absLane    Absolute lane number
 * @param[in]  bank   Bank number
 * @param[in/out]  doneValue    Adapt done value captured
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxAdaptDoneBank(AriesLinkType* link, int side,
                                       int absLane, int bank, int* doneValue)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);

    if (bank == 0)
    {
        // Check the RX_ADPT_IQ_VLD bit == 1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE, dataWord);
        CHECK_SUCCESS(rc);
    }
    else if (bank == 1)
    {
        // Check the RX_ADPT_IQ_VLD bit == 1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE_B1, dataWord);
        CHECK_SUCCESS(rc);
    }
    else if (bank == 2)
    {
        // Check the RX_ADPT_IQ_VLD bit == 1
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            link->device->i2cDriver, side, pmaNum, pmaLane,
            ARIES_PMA_RAWLANEAON_DIG_RX_ADAPT_DONE_B2, dataWord);
        CHECK_SUCCESS(rc);
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Get bits 6:0
    *doneValue = (dataWord[1] << 8) | dataWord[0];
    return ARIES_SUCCESS;
}

/**
 * @brief Get the current RX DFE code
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  side   PMA side
 * @param[in]  absLane    Absolute lane number
 * @param[in]  tapNum    DFE tap code
 * @param[in/out]  dfeCode    dfe code captured
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetRxDfeCode(AriesLinkType* link, int side, int absLane,
                                 int tapNum, int* dfeCode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int pmaNum = ariesGetPmaNumber(absLane);
    int pmaLane = ariesGetPmaLane(absLane);
    int address;
    int tapVal;

    switch (tapNum)
    {
        case 1:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP1_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get Bits 13:0
            tapVal = (((dataWord[1] & 0x3f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal;
            if (tapVal >= 256)
            {
                // dfe_tap1_code is stored in two's complement format
                *dfeCode = tapVal - 512;
            }
            break;

        case 2:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP2_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 12:0
            tapVal = (((dataWord[1] & 0x1f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 128;
            break;

        case 3:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP3_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        case 4:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP4_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        case 5:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP5_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        case 6:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP6_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        case 7:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP7_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        case 8:
            // Get PMA Tap address from a util function
            address = ARIES_PMA_LANE_DIG_RX_ADPTCTL_DFE_TAP8_STATUS;
            rc = ariesReadWordPmaLaneMainMicroIndirect(link->device->i2cDriver,
                                                       side, pmaNum, pmaLane,
                                                       address, dataWord);
            CHECK_SUCCESS(rc);
            // Get bits 11:0
            tapVal = (((dataWord[1] & 0x0f) << 8) + dataWord[0]) >> 5;
            *dfeCode = tapVal - 64;
            break;

        default:
            ASTERA_ERROR("Invalid DFE Tag");
            return ARIES_INVALID_ARGUMENT;
            break;
    }
    return ARIES_SUCCESS;
}

/**
 * @brief Get the last speed at which EQ was run (e.g. 3 for Gen-3)
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  speed    last captures speed
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqSpeed(AriesLinkType* link, int lane, int direction,
                                   int* speed)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;

    // Reading gp_ctrl_sts_struct.last_eq_pcie_gen
    int address = link->device->pm_gp_ctrl_sts_struct_addr +
                  ARIES_CTRL_STS_STRUCT_LAST_EQ_PCIE_GEN;

    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *speed = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief Get the deskew status string for the given lane
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    deskew status for lane
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetDeskewStatus(AriesLinkType* link, int lane,
                                    int direction, int* status)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    uint32_t address = qsOff + pathOff + pathLaneOff + ARIES_DESKEW_STATUS;

    rc = ariesReadByteData(link->device->i2cDriver, address, dataByte);
    CHECK_SUCCESS(rc);
    *status = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief Get the number of clocks of deskew applied for the given lane
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    deskew clocks for lane
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetDeskewClks(AriesLinkType* link, int lane, int direction,
                                  int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Based on the lane info, determine QS and Path info
    int qs;
    int qsPath;
    int qsPathLane;
    ariesGetQSPathInfo(lane, direction, &qs, &qsPath, &qsPathLane);

    // Compute reg address
    // Calculate QS, Path, Path lane relative offsets first,
    // and then compute actual address
    int qsOff = ARIES_QS_0_CSR_OFFSET + (qs * ARIES_QS_STRIDE);
    int pathOff = ARIES_PATH_WRAPPER_0_CSR_OFFSET +
                  (qsPath * ARIES_PATH_WRP_STRIDE);
    int pathLaneOff = ARIES_PATH_LANE_0_CSR_OFFSET +
                      (qsPathLane * ARIES_PATH_LANE_STRIDE);

    uint32_t address = qsOff + pathOff + pathLaneOff + ARIES_DSK_CC_DELTA;

    rc = ariesReadByteData(link->device->i2cDriver, address, dataByte);
    CHECK_SUCCESS(rc);
    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the final pre-cursor request
 * the link partner made. If the last request was a Perest request, this value
 * will be 0.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    Pre-cursor request value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqReqPre(AriesLinkType* link, int lane,
                                    int direction, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PRE_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PRE_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_pre_lnX
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

AriesErrorType ariesGetPathFWState(AriesLinkType* link, int lane, int direction,
                                   int* state)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int offset = ARIES_CTRL_STS_STRUCT_FW_STATE;

    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *state = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the final cursor request
 * the link partner made. If the last request was a Perest request, this value
 * will be 0.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    cursor request value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqReqCur(AriesLinkType* link, int lane,
                                    int direction, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_CUR_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_CUR_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_cur_lnX
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the final post-cursor request
 * the link partner made. If the last request was a Perest request, this value
 * will be 0.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    Post-cursor request value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqReqPst(AriesLinkType* link, int lane,
                                    int direction, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PST_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PST_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_pst_lnX
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the final preset request
 * the link partner made. A value of 0xf will indicate the final request was a
 * coefficient request.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  direction   PMA side
 * @param[in/out]  val    Preset request value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqReqPreset(AriesLinkType* link, int lane,
                                       int direction, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PRESET_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FINAL_REQ_PRESET_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_preset_lnX
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                            address, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the Preset value for the
 * specified request.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  reqNum    Request num
 * @param[in/out]  val    Preset value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqPresetReq(AriesLinkType* link, int lane,
                                       int direction, int reqNum, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_PRESET_REQS_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_PRESET_REQS_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_preset_ln
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset + reqNum;
    rc = ariesReadBlockDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                             address, 1, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the FOM value for the
 * specified request.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  reqNum    Request num
 * @param[in/out]  val    FOM value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqPresetReqFOM(AriesLinkType* link, int lane,
                                          int direction, int reqNum, int* val)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FOMS_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_FOMS_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_preset_ln
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset + reqNum;
    rc = ariesReadBlockDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                             address, 1, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief For the last round of Equalization, get the number of
 * reset requests issued.
 *
 * @param[in]  link   Link struct created by user
 * @param[in]  lane    Absolute lane number
 * @param[in]  reqNum    Request num
 * @param[in/out]  val    Number of reset requests
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLastEqNumPresetReq(AriesLinkType* link, int lane,
                                          int direction, int* val)
{
    AriesErrorType rc;
    int pathID = ((lane / 2) * 2) + direction;
    int pathLane = lane % 2;
    uint8_t dataByte[1] = {0};
    int offset;

    if (pathLane == 0)
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_NUM_PRESET_REQS_LN0;
    }
    else
    {
        offset = ARIES_CTRL_STS_STRUCT_LAST_EQ_NUM_PRESET_REQS_LN1;
    }

    // Reading gp_ctrl_sts_struct.last_eq_final_req_preset_ln
    int address = link->device->pm_gp_ctrl_sts_struct_addr + offset;
    rc = ariesReadBlockDataPathMicroIndirect(link->device->i2cDriver, pathID,
                                             address, 1, dataByte);
    CHECK_SUCCESS(rc);

    *val = dataByte[0];
    return ARIES_SUCCESS;
}

/**
 * @brief Get the format ID at offset in the print buffer
 *
 * @param[in]  link         Link struct created by user
 * @param[in]  loggerType   Logger type (main or which path)
 * @param[in]  offset       Print buffer offset
 * @param[in/out]  fmtID    Format ID from logger
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLoggerFmtID(AriesLinkType* link,
                                   AriesLTSSMLoggerEnumType loggerType,
                                   int offset, int* fmtID)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int address;

    // Main Micro
    if (loggerType == ARIES_LTSSM_LINK_LOGGER)
    {
        address = link->device->mm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_PRINT_BUFFER_OFFSET + offset;
        rc = ariesReadByteDataMainMicroIndirect(link->device->i2cDriver,
                                                address, dataByte);
        CHECK_SUCCESS(rc);
        *fmtID = dataByte[0];
    }
    else
    {
        address = link->device->pm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_PRINT_BUFFER_OFFSET + offset;
        rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver,
                                                loggerType, address, dataByte);
        CHECK_SUCCESS(rc);
        *fmtID = dataByte[0];
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Get the write pointer location in the LTSSM logger
 *
 * @param[in]  link               Link struct created by user
 * @param[in]  loggerType         Logger type (main or which path)
 * @param[in/out]  writeOffset    Current write offset
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLoggerWriteOffset(AriesLinkType* link,
                                         AriesLTSSMLoggerEnumType loggerType,
                                         int* writeOffset)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int address;
    int data0;
    int data1;

    // Main Micro
    if (loggerType == ARIES_LTSSM_LINK_LOGGER)
    {
        address = link->device->mm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_WR_PTR_OFFSET;
        rc = ariesReadByteDataMainMicroIndirect(link->device->i2cDriver,
                                                address, dataByte);
        CHECK_SUCCESS(rc);
        data0 = dataByte[0];
        rc = ariesReadByteDataMainMicroIndirect(link->device->i2cDriver,
                                                (address + 1), dataByte);
        CHECK_SUCCESS(rc);
        data1 = dataByte[0];

        *writeOffset = (data1 << 8) | data0;
    }
    else
    {
        address = link->device->pm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_WR_PTR_OFFSET;
        rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver,
                                                loggerType, address, dataByte);
        CHECK_SUCCESS(rc);
        data0 = dataByte[0];
        rc = ariesReadByteDataPathMicroIndirect(
            link->device->i2cDriver, loggerType, (address + 1), dataByte);
        CHECK_SUCCESS(rc);
        data1 = dataByte[0];

        *writeOffset = (data1 << 8) | data0;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Check if one batch mode in the LTSSM logger is enabled or not
 *
 * @param[in]  link               Link struct created by user
 * @param[in]  loggerType         Logger type (main or which path)
 * @param[in/out]  oneBatchModeEn    One batch mode enabled or not
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetLoggerOneBatchModeEn(AriesLinkType* link,
                                            AriesLTSSMLoggerEnumType loggerType,
                                            int* oneBatchModeEn)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int address;

    if (loggerType == ARIES_LTSSM_LINK_LOGGER)
    {
        address = link->device->mm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_ONE_BATCH_MODE_EN_OFFSET;
        rc = ariesReadByteDataMainMicroIndirect(link->device->i2cDriver,
                                                address, dataByte);
        CHECK_SUCCESS(rc);
        *oneBatchModeEn = dataByte[0];
    }
    else
    {
        address = link->device->pm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_ONE_BATCH_MODE_EN_OFFSET;
        rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver,
                                                loggerType, address, dataByte);
        CHECK_SUCCESS(rc);
        *oneBatchModeEn = dataByte[0];
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Check if one batch write in the LTSSM logger is enabled or not
 *
 * @param[in]  link               Link struct created by user
 * @param[in]  loggerType         Logger type (main or which path)
 * @param[in/out]  oneBatchWrEn    One batch write enabled or not
 * @return     AriesErrorType - Aries error code
 *
 */
AriesErrorType ariesGetLoggerOneBatchWrEn(AriesLinkType* link,
                                          AriesLTSSMLoggerEnumType loggerType,
                                          int* oneBatchWrEn)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int address;

    if (loggerType == ARIES_LTSSM_LINK_LOGGER)
    {
        address = link->device->mm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_ONE_BATCH_WR_EN_OFFSET;
        rc = ariesReadByteDataMainMicroIndirect(link->device->i2cDriver,
                                                address, dataByte);
        CHECK_SUCCESS(rc);
        *oneBatchWrEn = dataByte[0];
    }
    else
    {
        address = link->device->pm_print_info_struct_addr +
                  ARIES_PRINT_INFO_STRUCT_ONE_BATCH_WR_EN_OFFSET;
        rc = ariesReadByteDataPathMicroIndirect(link->device->i2cDriver,
                                                loggerType, address, dataByte);
        CHECK_SUCCESS(rc);
        *oneBatchWrEn = dataByte[0];
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Calculate CRC-8 byte from polynomial
 *
 * @param[in] msg  Polynomial to calculate PEC
 * @param[in] length Length of polynomial
 * @return     uint8_t - crc-8 byte
 */
uint8_t ariesGetPecByte(uint8_t* polynomial, uint8_t length)
{
    uint8_t crc;
    int byteIndex;
    int bitIndex;

    // Shift polynomial by 1 so that it is 8 bit
    // Take this extra bit into account later
    uint8_t poly = ARIES_CRC8_POLYNOMIAL >> 1;
    crc = polynomial[0];

    for (byteIndex = 1; byteIndex < length; byteIndex++)
    {
        uint8_t nextByte = polynomial[byteIndex];
        for (bitIndex = 7; bitIndex >= 0; bitIndex--)
        {
            // Check if MSB in CRC is 1
            if (crc & 0x80)
            {
                // Perform subtraction of first 8 bits
                crc = (crc ^ poly) << 1;
                // Add final bit of mod2 subtraction and place in pos 0 of CRC
                crc = crc + (((nextByte >> bitIndex) & 1) ^ 1);
            }
            else
            {
                // Shift out 0
                crc = crc << 1;
                // Shift in next bit
                crc = crc + ((nextByte >> bitIndex) & 1);
            }
        }
    }
    return crc;
}

/**
 * @brief Capture the min FoM value seen for a given lane.
 *
 * @param[in] device  Device struct containing i2c driver
 * @param[in] side    PMA side
 * @param[in] pathID  Path ID
 * @param[in] lane    lane inside path
 * @param[in] regOffset  address offset for FoM reg
 * @param[in,out] data   2-byte data returned, containing FoM value
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesGetMinFoMVal(AriesDeviceType* device, int side, int pathID,
                                 int lane, int regOffset, uint8_t* data)
{
    AriesErrorType rc;
    AriesErrorType lc;
    uint8_t dataByte[1] = {0};
    uint8_t dataWord[2] = {0};

    int address = regOffset;
    if (lane < 4)
    {
        address = regOffset + (lane * ARIES_PMA_LANE_STRIDE);
    }

    lc = ariesLock(device->i2cDriver);
    CHECK_SUCCESS(lc);

    // Set up reg address
    dataWord[0] = address & 0xff;
    dataWord[1] = (address >> 8) & 0xff;
    rc = ariesWriteBlockData(device->i2cDriver, ARIES_MM_ASSIST_REG_ADDR_OFFSET,
                             2, dataWord);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Path ID is represented in upper 4 bits of this byte
    dataByte[0] = pathID << 4;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_MM_ASSIST_PATH_ID_OFFSET,
                            dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Set up PMA side
    dataByte[0] = ARIES_MM_RD_PID_IND_PMA0 + side;
    rc = ariesWriteByteData(device->i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET,
                            dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Get data (LSB and MSB)
    rc = ariesReadByteData(device->i2cDriver, ARIES_MM_ASSIST_DATA_OFFSET,
                           dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    data[0] = dataByte[0];
    rc = ariesReadByteData(device->i2cDriver, ARIES_MM_ASSIST_STS_OFFSET,
                           dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(device->i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    data[1] = dataByte[0];

    lc = ariesUnlock(device->i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetPinMap(AriesDeviceType* device)
{
    // All pins are same size
    size_t pinSize = sizeof(device->pins[0].pinSet1.rxPin);
    if (device->partNumber == ARIES_PTX16)
    {
        device->pins[0].lane = 0;
        strncpy(device->pins[0].pinSet1.rxPin, "B_PER0", pinSize);
        strncpy(device->pins[0].pinSet1.txPin, "A_PET0", pinSize);
        device->pins[0].pinSet1.rxPackageInversion = 1;
        device->pins[0].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[0].pinSet2.rxPin, "A_PER0", pinSize);
        strncpy(device->pins[0].pinSet2.txPin, "B_PET0", pinSize);
        device->pins[0].pinSet2.rxPackageInversion = 1;
        device->pins[0].pinSet2.txPackageInsersion = 1;

        device->pins[1].lane = 1;
        strncpy(device->pins[1].pinSet1.rxPin, "B_PER1", pinSize);
        strncpy(device->pins[1].pinSet1.txPin, "A_PET1", pinSize);
        device->pins[1].pinSet1.rxPackageInversion = 1;
        device->pins[1].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[1].pinSet2.rxPin, "A_PER1", pinSize);
        strncpy(device->pins[1].pinSet2.txPin, "B_PET1", pinSize);
        device->pins[1].pinSet2.rxPackageInversion = 0;
        device->pins[1].pinSet2.txPackageInsersion = 0;

        device->pins[2].lane = 2;
        strncpy(device->pins[2].pinSet1.rxPin, "B_PER2", pinSize);
        strncpy(device->pins[2].pinSet1.txPin, "A_PET2", pinSize);
        device->pins[2].pinSet1.rxPackageInversion = 0;
        device->pins[2].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[2].pinSet2.rxPin, "A_PER2", pinSize);
        strncpy(device->pins[2].pinSet2.txPin, "B_PET2", pinSize);
        device->pins[2].pinSet2.rxPackageInversion = 1;
        device->pins[2].pinSet2.txPackageInsersion = 0;

        device->pins[3].lane = 3;
        strncpy(device->pins[3].pinSet1.rxPin, "B_PER3", pinSize);
        strncpy(device->pins[3].pinSet1.txPin, "A_PET3", pinSize);
        device->pins[3].pinSet1.rxPackageInversion = 0;
        device->pins[3].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[3].pinSet2.rxPin, "A_PER3", pinSize);
        strncpy(device->pins[3].pinSet2.txPin, "B_PET3", pinSize);
        device->pins[3].pinSet2.rxPackageInversion = 1;
        device->pins[3].pinSet2.txPackageInsersion = 1;

        device->pins[4].lane = 4;
        strncpy(device->pins[4].pinSet1.rxPin, "B_PER4", pinSize);
        strncpy(device->pins[4].pinSet1.txPin, "A_PET4", pinSize);
        device->pins[4].pinSet1.rxPackageInversion = 1;
        device->pins[4].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[4].pinSet2.rxPin, "A_PER4", pinSize);
        strncpy(device->pins[4].pinSet2.txPin, "B_PET4", pinSize);
        device->pins[4].pinSet2.rxPackageInversion = 0;
        device->pins[4].pinSet2.txPackageInsersion = 1;

        device->pins[5].lane = 5;
        strncpy(device->pins[5].pinSet1.rxPin, "B_PER5", pinSize);
        strncpy(device->pins[5].pinSet1.txPin, "A_PET5", pinSize);
        device->pins[5].pinSet1.rxPackageInversion = 1;
        device->pins[5].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[5].pinSet2.rxPin, "A_PER5", pinSize);
        strncpy(device->pins[5].pinSet2.txPin, "B_PET5", pinSize);
        device->pins[5].pinSet2.rxPackageInversion = 0;
        device->pins[5].pinSet2.txPackageInsersion = 0;

        device->pins[6].lane = 6;
        strncpy(device->pins[6].pinSet1.rxPin, "B_PER6", pinSize);
        strncpy(device->pins[6].pinSet1.txPin, "A_PET6", pinSize);
        device->pins[6].pinSet1.rxPackageInversion = 0;
        device->pins[6].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[6].pinSet2.rxPin, "A_PER6", pinSize);
        strncpy(device->pins[6].pinSet2.txPin, "B_PET6", pinSize);
        device->pins[6].pinSet2.rxPackageInversion = 1;
        device->pins[6].pinSet2.txPackageInsersion = 1;

        device->pins[7].lane = 7;
        strncpy(device->pins[7].pinSet1.rxPin, "B_PER7", pinSize);
        strncpy(device->pins[7].pinSet1.txPin, "A_PET7", pinSize);
        device->pins[7].pinSet1.rxPackageInversion = 0;
        device->pins[7].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[7].pinSet2.rxPin, "A_PER7", pinSize);
        strncpy(device->pins[7].pinSet2.txPin, "B_PET7", pinSize);
        device->pins[7].pinSet2.rxPackageInversion = 1;
        device->pins[7].pinSet2.txPackageInsersion = 1;

        device->pins[8].lane = 8;
        strncpy(device->pins[8].pinSet1.rxPin, "B_PER8", pinSize);
        strncpy(device->pins[8].pinSet1.txPin, "A_PET8", pinSize);
        device->pins[8].pinSet1.rxPackageInversion = 1;
        device->pins[8].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[8].pinSet2.rxPin, "A_PER8", pinSize);
        strncpy(device->pins[8].pinSet2.txPin, "B_PET8", pinSize);
        device->pins[8].pinSet2.rxPackageInversion = 1;
        device->pins[8].pinSet2.txPackageInsersion = 0;

        device->pins[9].lane = 9;
        strncpy(device->pins[9].pinSet1.rxPin, "B_PER9", pinSize);
        strncpy(device->pins[9].pinSet1.txPin, "A_PET9", pinSize);
        device->pins[9].pinSet1.rxPackageInversion = 1;
        device->pins[9].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[9].pinSet2.rxPin, "A_PER9", pinSize);
        strncpy(device->pins[9].pinSet2.txPin, "B_PET9", pinSize);
        device->pins[9].pinSet2.rxPackageInversion = 1;
        device->pins[9].pinSet2.txPackageInsersion = 0;

        device->pins[10].lane = 10;
        strncpy(device->pins[10].pinSet1.rxPin, "B_PER10", pinSize);
        strncpy(device->pins[10].pinSet1.txPin, "A_PET10", pinSize);
        device->pins[10].pinSet1.rxPackageInversion = 0;
        device->pins[10].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[10].pinSet2.rxPin, "A_PER10", pinSize);
        strncpy(device->pins[10].pinSet2.txPin, "B_PET10", pinSize);
        device->pins[10].pinSet2.rxPackageInversion = 0;
        device->pins[10].pinSet2.txPackageInsersion = 0;

        device->pins[11].lane = 11;
        strncpy(device->pins[11].pinSet1.rxPin, "B_PER11", pinSize);
        strncpy(device->pins[11].pinSet1.txPin, "A_PET11", pinSize);
        device->pins[11].pinSet1.rxPackageInversion = 0;
        device->pins[11].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[11].pinSet2.rxPin, "A_PER11", pinSize);
        strncpy(device->pins[11].pinSet2.txPin, "B_PET11", pinSize);
        device->pins[11].pinSet2.rxPackageInversion = 0;
        device->pins[11].pinSet2.txPackageInsersion = 1;

        device->pins[12].lane = 12;
        strncpy(device->pins[12].pinSet1.rxPin, "B_PER12", pinSize);
        strncpy(device->pins[12].pinSet1.txPin, "A_PET12", pinSize);
        device->pins[12].pinSet1.rxPackageInversion = 1;
        device->pins[12].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[12].pinSet2.rxPin, "A_PER12", pinSize);
        strncpy(device->pins[12].pinSet2.txPin, "B_PET12", pinSize);
        device->pins[12].pinSet2.rxPackageInversion = 1;
        device->pins[12].pinSet2.txPackageInsersion = 1;

        device->pins[13].lane = 13;
        strncpy(device->pins[13].pinSet1.rxPin, "B_PER13", pinSize);
        strncpy(device->pins[13].pinSet1.txPin, "A_PET13", pinSize);
        device->pins[13].pinSet1.rxPackageInversion = 1;
        device->pins[13].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[13].pinSet2.rxPin, "A_PER13", pinSize);
        strncpy(device->pins[13].pinSet2.txPin, "B_PET13", pinSize);
        device->pins[13].pinSet2.rxPackageInversion = 1;
        device->pins[13].pinSet2.txPackageInsersion = 1;

        device->pins[14].lane = 14;
        strncpy(device->pins[14].pinSet1.rxPin, "B_PER14", pinSize);
        strncpy(device->pins[14].pinSet1.txPin, "A_PET14", pinSize);
        device->pins[14].pinSet1.rxPackageInversion = 0;
        device->pins[14].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[14].pinSet2.rxPin, "A_PER14", pinSize);
        strncpy(device->pins[14].pinSet2.txPin, "B_PET14", pinSize);
        device->pins[14].pinSet2.rxPackageInversion = 0;
        device->pins[14].pinSet2.txPackageInsersion = 0;

        device->pins[15].lane = 15;
        strncpy(device->pins[15].pinSet1.rxPin, "B_PER15", pinSize);
        strncpy(device->pins[15].pinSet1.txPin, "A_PET15", pinSize);
        device->pins[15].pinSet1.rxPackageInversion = 0;
        device->pins[15].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[15].pinSet2.rxPin, "A_PER15", pinSize);
        strncpy(device->pins[15].pinSet2.txPin, "B_PET15", pinSize);
        device->pins[15].pinSet2.rxPackageInversion = 1;
        device->pins[15].pinSet2.txPackageInsersion = 0;
    }
    else if (device->partNumber == ARIES_PTX08)
    {
        device->pins[0].lane = 0;
        strncpy(device->pins[0].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[0].pinSet1.txPin, "", pinSize);
        device->pins[0].pinSet1.rxPackageInversion = 1;
        device->pins[0].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[0].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[0].pinSet2.txPin, "", pinSize);
        device->pins[0].pinSet2.rxPackageInversion = 1;
        device->pins[0].pinSet2.txPackageInsersion = 1;

        device->pins[1].lane = 1;
        strncpy(device->pins[1].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[1].pinSet1.txPin, "", pinSize);
        device->pins[1].pinSet1.rxPackageInversion = 1;
        device->pins[1].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[1].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[1].pinSet2.txPin, "", pinSize);
        device->pins[1].pinSet2.rxPackageInversion = 0;
        device->pins[1].pinSet2.txPackageInsersion = 0;

        device->pins[2].lane = 2;
        strncpy(device->pins[2].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[2].pinSet1.txPin, "", pinSize);
        device->pins[2].pinSet1.rxPackageInversion = 0;
        device->pins[2].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[2].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[2].pinSet2.txPin, "", pinSize);
        device->pins[2].pinSet2.rxPackageInversion = 1;
        device->pins[2].pinSet2.txPackageInsersion = 0;

        device->pins[3].lane = 3;
        strncpy(device->pins[3].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[3].pinSet1.txPin, "", pinSize);
        device->pins[3].pinSet1.rxPackageInversion = 0;
        device->pins[3].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[3].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[3].pinSet2.txPin, "", pinSize);
        device->pins[3].pinSet2.rxPackageInversion = 1;
        device->pins[3].pinSet2.txPackageInsersion = 1;

        device->pins[4].lane = 4;
        strncpy(device->pins[4].pinSet1.rxPin, "A_PER0", pinSize);
        strncpy(device->pins[4].pinSet1.txPin, "B_PET0", pinSize);
        device->pins[4].pinSet1.rxPackageInversion = 1;
        device->pins[4].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[4].pinSet2.rxPin, "B_PER0", pinSize);
        strncpy(device->pins[4].pinSet2.txPin, "A_PET0", pinSize);
        device->pins[4].pinSet2.rxPackageInversion = 0;
        device->pins[4].pinSet2.txPackageInsersion = 1;

        device->pins[5].lane = 5;
        strncpy(device->pins[5].pinSet1.rxPin, "A_PER1", pinSize);
        strncpy(device->pins[5].pinSet1.txPin, "B_PET1", pinSize);
        device->pins[5].pinSet1.rxPackageInversion = 1;
        device->pins[5].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[5].pinSet2.rxPin, "B_PER1", pinSize);
        strncpy(device->pins[5].pinSet2.txPin, "A_PET1", pinSize);
        device->pins[5].pinSet2.rxPackageInversion = 0;
        device->pins[5].pinSet2.txPackageInsersion = 0;

        device->pins[6].lane = 6;
        strncpy(device->pins[6].pinSet1.rxPin, "A_PER2", pinSize);
        strncpy(device->pins[6].pinSet1.txPin, "B_PET2", pinSize);
        device->pins[6].pinSet1.rxPackageInversion = 0;
        device->pins[6].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[6].pinSet2.rxPin, "B_PER2", pinSize);
        strncpy(device->pins[6].pinSet2.txPin, "A_PET2", pinSize);
        device->pins[6].pinSet2.rxPackageInversion = 1;
        device->pins[6].pinSet2.txPackageInsersion = 1;

        device->pins[7].lane = 7;
        strncpy(device->pins[7].pinSet1.rxPin, "A_PER3", pinSize);
        strncpy(device->pins[7].pinSet1.txPin, "B_PET3", pinSize);
        device->pins[7].pinSet1.rxPackageInversion = 0;
        device->pins[7].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[7].pinSet2.rxPin, "B_PER3", pinSize);
        strncpy(device->pins[7].pinSet2.txPin, "A_PET3", pinSize);
        device->pins[7].pinSet2.rxPackageInversion = 1;
        device->pins[7].pinSet2.txPackageInsersion = 1;

        device->pins[8].lane = 8;
        strncpy(device->pins[8].pinSet1.rxPin, "A_PER4", pinSize);
        strncpy(device->pins[8].pinSet1.txPin, "B_PET4", pinSize);
        device->pins[8].pinSet1.rxPackageInversion = 1;
        device->pins[8].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[8].pinSet2.rxPin, "B_PER4", pinSize);
        strncpy(device->pins[8].pinSet2.txPin, "A_PET4", pinSize);
        device->pins[8].pinSet2.rxPackageInversion = 1;
        device->pins[8].pinSet2.txPackageInsersion = 0;

        device->pins[9].lane = 9;
        strncpy(device->pins[9].pinSet1.rxPin, "A_PER5", pinSize);
        strncpy(device->pins[9].pinSet1.txPin, "B_PET5", pinSize);
        device->pins[9].pinSet1.rxPackageInversion = 1;
        device->pins[9].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[9].pinSet2.rxPin, "B_PER5", pinSize);
        strncpy(device->pins[9].pinSet2.txPin, "A_PET5", pinSize);
        device->pins[9].pinSet2.rxPackageInversion = 1;
        device->pins[9].pinSet2.txPackageInsersion = 0;

        device->pins[10].lane = 10;
        strncpy(device->pins[10].pinSet1.rxPin, "A_PER6", pinSize);
        strncpy(device->pins[10].pinSet1.txPin, "B_PET6", pinSize);
        device->pins[10].pinSet1.rxPackageInversion = 0;
        device->pins[10].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[10].pinSet2.rxPin, "B_PER6", pinSize);
        strncpy(device->pins[10].pinSet2.txPin, "A_PET6", pinSize);
        device->pins[10].pinSet2.rxPackageInversion = 0;
        device->pins[10].pinSet2.txPackageInsersion = 0;

        device->pins[11].lane = 11;
        strncpy(device->pins[11].pinSet1.rxPin, "A_PER7", pinSize);
        strncpy(device->pins[11].pinSet1.txPin, "B_PET7", pinSize);
        device->pins[11].pinSet1.rxPackageInversion = 0;
        device->pins[11].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[11].pinSet2.rxPin, "B_PER7", pinSize);
        strncpy(device->pins[11].pinSet2.txPin, "A_PET7", pinSize);
        device->pins[11].pinSet2.rxPackageInversion = 0;
        device->pins[11].pinSet2.txPackageInsersion = 1;

        device->pins[12].lane = 12;
        strncpy(device->pins[12].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[12].pinSet1.txPin, "", pinSize);
        device->pins[12].pinSet1.rxPackageInversion = 1;
        device->pins[12].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[12].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[12].pinSet2.txPin, "", pinSize);
        device->pins[12].pinSet2.rxPackageInversion = 1;
        device->pins[12].pinSet2.txPackageInsersion = 1;

        device->pins[13].lane = 13;
        strncpy(device->pins[13].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[13].pinSet1.txPin, "", pinSize);
        device->pins[13].pinSet1.rxPackageInversion = 1;
        device->pins[13].pinSet1.txPackageInsersion = 0;
        strncpy(device->pins[13].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[13].pinSet2.txPin, "", pinSize);
        device->pins[13].pinSet2.rxPackageInversion = 1;
        device->pins[13].pinSet2.txPackageInsersion = 1;

        device->pins[14].lane = 14;
        strncpy(device->pins[14].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[14].pinSet1.txPin, "", pinSize);
        device->pins[14].pinSet1.rxPackageInversion = 0;
        device->pins[14].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[14].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[14].pinSet2.txPin, "", pinSize);
        device->pins[14].pinSet2.rxPackageInversion = 0;
        device->pins[14].pinSet2.txPackageInsersion = 0;

        device->pins[15].lane = 15;
        strncpy(device->pins[15].pinSet1.rxPin, "", pinSize);
        strncpy(device->pins[15].pinSet1.txPin, "", pinSize);
        device->pins[15].pinSet1.rxPackageInversion = 0;
        device->pins[15].pinSet1.txPackageInsersion = 1;
        strncpy(device->pins[15].pinSet2.rxPin, "", pinSize);
        strncpy(device->pins[15].pinSet2.txPin, "", pinSize);
        device->pins[15].pinSet2.rxPackageInversion = 1;
        device->pins[15].pinSet2.txPackageInsersion = 0;
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Read numBytes bytes of data starting at startAddr
 */
AriesErrorType ariesEepromReadBlockData(AriesDeviceType* device,
                                        uint8_t* values, int startAddr,
                                        uint8_t numBytes)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int addrMSB;
    int addr;
    int currentPage = 0;
    bool firstRead = true;
    int valIndex = 0;

    if ((numBytes <= 0) || (startAddr < 0) || (startAddr >= 262144) ||
        ((startAddr + numBytes - 1) >= 262144))
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Perform legacy mode reads here
    for (addr = startAddr; addr < (startAddr + numBytes); addr++)
    {
        addrMSB = floor(addr / 65536);
        // Set page and address on the bus with first read
        // Or whenever there is a page update
        if (firstRead || (addrMSB != currentPage))
        {
            // Set updated page address
            rc = ariesI2CMasterSetPage(device->i2cDriver, addrMSB);
            CHECK_SUCCESS(rc);
            currentPage = addrMSB;

            // Send EEPROM address 0 after page update
            dataByte[0] = (addr >> 8) & 0xff;
            rc = ariesI2CMasterSendByte(device->i2cDriver, dataByte, 2);
            CHECK_SUCCESS(rc);
            dataByte[0] = addr & 0xff;
            rc = ariesI2CMasterSendByte(device->i2cDriver, dataByte, 1);
            CHECK_SUCCESS(rc);
            firstRead = false;
        }

        // Perform byte read and store in values array
        rc = ariesI2CMasterReceiveByte(device->i2cDriver, dataByte);
        CHECK_SUCCESS(rc);

        values[valIndex] = dataByte[0];
        valIndex++;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Read numBytes bytes from the EEPROM starting at startAddr and
 * calculate a running checksum (e.g. add the bytes as you read them):
 * uint8_t checksum = (checksum + new_byte) % 256
 */
AriesErrorType ariesEepromCalcChecksum(AriesDeviceType* device, int startAddr,
                                       int numBytes, uint8_t* checksum)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t runningChecksum = 0;

    if ((numBytes <= 0) || (startAddr < 0) || (startAddr >= 262144) ||
        ((startAddr + numBytes - 1) >= 262144))
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Calculate expected checksum
    int addr;
    for (addr = startAddr; addr < (startAddr + numBytes); addr++)
    {
        rc = ariesEepromReadBlockData(device, dataByte, addr, 1);
        CHECK_SUCCESS(rc);
        runningChecksum = (runningChecksum + dataByte[0]) & 0xff;
    }
    *checksum = runningChecksum;
    return ARIES_SUCCESS;
}

/**
 * @brief Sort Array in ascending order
 *
 * @param[in] arr Array to be sorted
 * @param[in] size Num elements in array
 * @return None
 */
void ariesSortArray(uint16_t* arr, int size)
{
    int i, j, temp;
    for (i = 0; i < (size - 1); i++)
    {
        for (j = 0; j < (size - i - 1); j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Get median of all elements in the array
 *
 * @param[in] arr Array to be sorted
 * @param[in] size Num elements in array
 * @return None
 */
uint16_t ariesGetMedian(uint16_t* arr, int size)
{
    ariesSortArray(arr, size);
    // Capture middle element
    int ind = ((size + 1) / 2) - 1;
    return arr[ind];
}

/**
 * @brief This loads an binary file into the mem[] array
 */
AriesErrorType ariesLoadBinFile(const char* filename, uint8_t* mem)
{
    FILE* fin;
    int numBytesRead;

    // Check if file is valid
    if (strlen(filename) == 0)
    {
        ASTERA_ERROR("Can't load a file without the filename");
        return ARIES_INVALID_ARGUMENT;
    }
    fin = fopen(filename, "rb");
    if (fin == NULL)
    {
        ASTERA_ERROR("Can't open file '%s' for reading", filename);
        return ARIES_FAILURE;
    }

    // Read in bytes from file
    numBytesRead = fread(mem, 1, ARIES_EEPROM_MAX_NUM_BYTES, fin);

    if (numBytesRead != ARIES_EEPROM_MAX_NUM_BYTES)
    {
        ASTERA_ERROR(
            "We did not read out the expected ARIES_EEPROM_MAX_NUM_BYTES: %d bytes from file '%s' was %d",
            ARIES_EEPROM_MAX_NUM_BYTES, filename, numBytesRead);
        fclose(fin);
        return ARIES_FAILURE;
    }
    else if (ferror(fin))
    {
        ASTERA_ERROR("There was some error in reading from file '%s'",
                     filename);
        fclose(fin);
        return ARIES_FAILURE;
    }

    // Close file
    fclose(fin);

    return ARIES_SUCCESS;
}

/**
 * @brief This loads an intel hex file into the mem[] array
 */
AriesErrorType ariesLoadIhxFile(const char* filename, uint8_t* mem)
{
    AriesErrorType rc;
    char line[1000];
    FILE* fin;
    int addr;
    int n;
    int status;
    uint8_t bytes[256] = {0};
    int byte;
    int indx = 0;
    int total = 0;
    int lineno = 1;
    int minAddr = 0xffff;
    int maxAddr = 0;

    // Check if file is valid
    if (strlen(filename) == 0)
    {
        ASTERA_ERROR("Can't load a file without the filename");
        return ARIES_INVALID_ARGUMENT;
    }
    fin = fopen(filename, "r");
    if (fin == NULL)
    {
        ASTERA_ERROR("Can't open file '%s' for reading", filename);
        return ARIES_INVALID_ARGUMENT;
    }

    while (!feof(fin) && !ferror(fin))
    {
        line[0] = '\0';
        if (fgets(line, 1000, fin) == NULL)
        {
            ASTERA_ERROR("Error reading data from file");
            fclose(fin);
            return ARIES_FAILURE;
        }
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        if (line[strlen(line) - 1] == '\r')
        {
            line[strlen(line) - 1] = '\0';
        }

        rc = ariesParseIhxLine(line, bytes, &addr, &n, &status);
        if (rc != ARIES_SUCCESS)
        {
            ASTERA_ERROR("Error: '%s', line: %d", filename, lineno);
        }
        else
        {
            if (status == 0)
            { /* data */
                for (byte = 0; byte <= (n - 1); byte++)
                {
                    mem[indx] = bytes[byte] & 0xff;
                    total++;
                    if (addr < minAddr)
                        minAddr = addr;
                    if (addr > maxAddr)
                        maxAddr = addr;
                    addr++;
                    indx++;
                }
            }
            if (status == 1)
            { /* end of file */
                break;
            }
        }
        lineno++;
    }
    fclose(fin);
    return ARIES_SUCCESS;
}

/**
 * @brief This is used by loadFile to get each line of intex hex
 */
AriesErrorType ariesParseIhxLine(char* line, uint8_t* bytes, int* addr,
                                 int* num, int* status)
{
    int sum;
    int cksum;
    int len;
    char* ptr;
    int val;

    *num = 0;
    // First char
    if (line[0] != ':')
    {
        return ARIES_FAILURE;
    }

    if (strlen(line) < 11)
    {
        return ARIES_FAILURE;
    }
    ptr = line + 1;

    if (!sscanf(ptr, "%02x", (unsigned int*)&len))
    {
        return ARIES_FAILURE;
    }
    ptr += 2;

    if (strlen(line) < (size_t)(11 + (len * 2)))
    {
        return ARIES_FAILURE;
    }
    if (!sscanf(ptr, "%04x", (unsigned int*)addr))
    {
        return ARIES_FAILURE;
    }
    ptr += 4;

    if (!sscanf(ptr, "%02x", (unsigned int*)status))
    {
        return ARIES_FAILURE;
    }
    ptr += 2;
    sum = (len & 0xff) + ((*addr >> 8) & 0xff) + (*addr & 0xff) +
          (*status & 0xff);

    while (*num != len)
    {
        int ret = sscanf(ptr, "%02x", (unsigned int*)&val);
        bytes[*num] = (uint8_t)val;
        if (!ret)
        {
            return ARIES_FAILURE;
        }
        ptr += 2;
        sum += bytes[*num] & 0xff;
        (*num)++;
        if (*num >= 256)
        {
            return ARIES_FAILURE;
        }
    }
    if (!sscanf(ptr, "%02x", (unsigned int*)&cksum))
    {
        return ARIES_FAILURE;
    }
    if (((sum & 0xff) + (cksum & 0xff)) & 0xff)
    {
        return ARIES_FAILURE; /* checksum error */
    }
    return ARIES_SUCCESS;
}

AriesErrorType ariesI2cMasterSoftReset(AriesI2CDriverType* i2cDriver)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t i2cInitCtrl;

    // Enable Bit bang mode
    dataByte[0] = 3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);

    rc = ariesReadByteData(i2cDriver, ARIES_I2C_MST_INIT_CTRL_ADDRESS,
                           dataByte);
    CHECK_SUCCESS(rc);
    i2cInitCtrl = dataByte[0];

    i2cInitCtrl =
        ARIES_MAIN_MICRO_EXT_CSR_I2C_MST_INIT_CTRL_BIT_BANG_MODE_EN_MODIFY(
            i2cInitCtrl, 1);
    dataByte[0] = i2cInitCtrl;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_INIT_CTRL_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);

    // Start Sequence
    // SDA = 1, SCL = 1
    dataByte[0] = 3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);
    // SDA = 0, SCL = 1
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);
    // SDA = 0, SCL = 0
    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);
    // SDA = 1, SCL = 0
    dataByte[0] = 2;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);

    int i = 0;
    for (i = 0; i < 9; i++)
    {
        // SDA = 1, SCL = 1
        dataByte[0] = 3;
        rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                                dataByte);
        CHECK_SUCCESS(rc);

        // SDA = 1 SCL = 0
        dataByte[0] = 2;
        rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                                dataByte);
        CHECK_SUCCESS(rc);
    }

    // Stop Sequence
    // SDA = 0, SCL = 0
    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);
    // SDA = 0, SCL = 1
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);
    // SDA = 1, SCL = 1
    dataByte[0] = 3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_BB_OUTPUT_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);

    // Disable BB mode
    i2cInitCtrl =
        ARIES_MAIN_MICRO_EXT_CSR_I2C_MST_INIT_CTRL_BIT_BANG_MODE_EN_MODIFY(
            i2cInitCtrl, 0);
    dataByte[0] = i2cInitCtrl;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_INIT_CTRL_ADDRESS,
                            dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetEEPROMFirstBlock(AriesI2CDriverType* i2cDriver,
                                        int* blockStartAddr)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    // Set Page = 0
    rc = ariesI2CMasterSetPage(i2cDriver, 0);
    CHECK_SUCCESS(rc);

    // Set address = 0
    dataByte[0] = 0;
    rc = ariesI2CMasterSendByte(i2cDriver, dataByte, 2);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0;
    rc = ariesI2CMasterSendByte(i2cDriver, dataByte, 1);
    CHECK_SUCCESS(rc);

    // The first block SHOULD start at address zero, but check up to
    // maxAddrToCheck bytes to find it.
    int addr = 0;
    int maxAddrToCheck = 50;

    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

    while (addr < maxAddrToCheck)
    {
        rc = ariesI2CMasterReceiveByte(i2cDriver, dataByte);
        CHECK_SUCCESS(rc);
        byte0 = dataByte[0];
        if (byte0 == 0xa5)
        {
            rc = ariesI2CMasterReceiveByte(i2cDriver, dataByte);
            CHECK_SUCCESS(rc);
            byte1 = dataByte[0];
            if (byte1 == 0x5a)
            {
                rc = ariesI2CMasterReceiveByte(i2cDriver, dataByte);
                CHECK_SUCCESS(rc);
                byte2 = dataByte[0];
                if (byte2 == 0xa5)
                {
                    rc = ariesI2CMasterReceiveByte(i2cDriver, dataByte);
                    CHECK_SUCCESS(rc);
                    byte3 = dataByte[0];
                    if (byte3 == 0x5a)
                    {
                        *blockStartAddr = addr;
                        break;
                    }
                }
            }
        }
        addr++;
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetEEPROMBlockType(AriesI2CDriverType* i2cDriver,
                                       int blockStartAddr, uint8_t* blockType)
{
    AriesErrorType rc;
    int addr;
    addr = blockStartAddr + 4;

    rc = ariesEEPROMGetRandomByte(i2cDriver, addr, blockType);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesGetEEPROMBlockCrcByte(AriesI2CDriverType* i2cDriver,
                                          int blockStartAddr, int blockLength,
                                          uint8_t* crcByte)
{
    AriesErrorType rc;
    int addr;
    addr = blockStartAddr + blockLength + 11;

    rc = ariesEEPROMGetRandomByte(i2cDriver, addr, crcByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesEEPROMGetRandomByte(AriesI2CDriverType* i2cDriver, int addr,
                                        uint8_t* value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    int addrMSB = floor(addr / 65536);

    rc = ariesI2CMasterSetPage(i2cDriver, addrMSB);
    CHECK_SUCCESS(rc);

    // Send address
    // Write command
    dataByte[0] = 0x10;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_IC_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    // Prepare Flag Byte
    dataByte[0] = 0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    uint8_t addr15To8;
    uint8_t addr7To0;
    addr15To8 = (addr >> 8) & 0xff;
    addr7To0 = addr & 0xff;
    dataByte[0] = addr15To8;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = addr7To0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    dataByte[0] = 3;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_DATA1_ADDR, dataByte);
    CHECK_SUCCESS(rc);

    dataByte[0] = 0x1;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    usleep(ARIES_I2C_MASTER_CMD_RST);
    dataByte[0] = 0x0;
    rc = ariesWriteByteData(i2cDriver, ARIES_I2C_MST_CMD_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    rc = ariesReadByteData(i2cDriver, ARIES_I2C_MST_DATA0_ADDR, dataByte);
    CHECK_SUCCESS(rc);
    *value = dataByte[0];

    return ARIES_SUCCESS;
}

AriesErrorType ariesEEPROMGetBlockLength(AriesI2CDriverType* i2cDriver,
                                         int blockStartAddr, int* blockLength)
{
    AriesErrorType rc;
    int addrMSB = floor(blockStartAddr / 65536);

    rc = ariesI2CMasterSetPage(i2cDriver, addrMSB);
    CHECK_SUCCESS(rc);

    int addrBlockLengthLSB = blockStartAddr + 5;
    uint8_t blockLengthLSB;
    rc = ariesEEPROMGetRandomByte(i2cDriver, addrBlockLengthLSB,
                                  &blockLengthLSB);
    CHECK_SUCCESS(rc);

    int addrBlockLengthMSB = blockStartAddr + 6;
    uint8_t blockLengthMSB;
    rc = ariesEEPROMGetRandomByte(i2cDriver, addrBlockLengthMSB,
                                  &blockLengthMSB);
    CHECK_SUCCESS(rc);

    *blockLength = (blockLengthMSB << 8) + blockLengthLSB;
    return ARIES_SUCCESS;
}

void ariesGetCrcBytesImage(uint8_t* image, uint8_t* crcBytes,
                           uint8_t* numCrcBytes)
{
    int numBlocks = 0;

    // Get First Block
    int addr = 0;
    int blockStartAddr = 0;
    int maxAddrToCheck = 50;
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    while (addr < maxAddrToCheck)
    {
        byte0 = image[addr];
        if (byte0 == 0xa5)
        {
            addr++;
            byte1 = image[addr];
            if (byte1 == 0x5a)
            {
                addr++;
                byte2 = image[addr];
                if (byte2 == 0xa5)
                {
                    addr++;
                    byte3 = image[addr];
                    if (byte3 == 0x5a)
                    {
                        blockStartAddr = addr - 3;
                        break;
                    }
                }
            }
        }
        addr++;
    }

    while (numBlocks < ARIES_EEPROM_MAX_NUM_CRC_BLOCKS)
    {
        // Get Block Type
        uint8_t blockType = image[(blockStartAddr + 4)];
        if (blockType != 0xff)
        {
            // Get Block Length
            uint8_t blockLengthLSB = image[(blockStartAddr + 5)];
            uint8_t blockLengthMSB = image[(blockStartAddr + 6)];
            uint16_t blockLength = (blockLengthMSB << 8) + blockLengthLSB;

            uint8_t crcByte = image[(blockStartAddr + blockLength + 11)];
            crcBytes[numBlocks] = crcByte;

            blockStartAddr += blockLength + 13;
            numBlocks++;
        }
        else
        {
            // Last Page
            break;
        }
    }

    *numCrcBytes = numBlocks;
}

AriesErrorType ariesPipeRxAdapt(AriesDeviceType* device, int side, int lane)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = lane / 4;
    int qsLane = lane % 4;

    // Clear Rx Req Override Enable
    rc = ariesPMAPCSRxReqBlock(device, side, lane, false);
    CHECK_SUCCESS(rc);

    // Set PIPE Rx EQ In Progress
    rc = ariesPipeRxEqInProgressSet(device, side, lane, true);
    CHECK_SUCCESS(rc);

    // Clear PhyStatus
    rc = ariesPipePhyStatusClear(device, side, lane);
    CHECK_SUCCESS(rc);

    // Set PIPE Rx EQ Eval
    rc = ariesPipeRxEqEval(device, side, lane, true);
    CHECK_SUCCESS(rc);

    // Wait for PhyStatus == 1
    rc = ariesPipePhyStatusToggle(device, side, lane);
    CHECK_SUCCESS(rc);

    // Clear PIPE Rx EQ Eval
    rc = ariesPipeRxEqEval(device, side, lane, false);
    CHECK_SUCCESS(rc);

    // Set Tx Data Enable
    rc = ariesPMARxDataEnSet(device, side, lane, true);
    CHECK_SUCCESS(rc);

    int i;
    for (i = 0; i < 20; i++)
    {
        rc = ariesPMAPCSRxReqBlock(device, side, lane, true);
        CHECK_SUCCESS(rc);

        rc = ariesReadWordPmaLaneMainMicroIndirect(
            device->i2cDriver, side, qs, qsLane,
            ARIES_PMA_LANE_DIG_ASIC_RX_ASIC_OUT_0, dataWord);
        CHECK_SUCCESS(rc);
        uint8_t rxvalid = (dataWord[0] >> 1) & 1; // VALID is bit 1

        if (rxvalid)
        {
            break;
        }
        else
        {
            rc = ariesPMAPCSRxReqBlock(device, side, lane, false);
            CHECK_SUCCESS(rc);
        }
    }
    // Check for no-adapt case
    if (i == 20)
    {
        ASTERA_ERROR(
            "Side:%d, Lane:%2d, RxValid=0! Confirm link partner transmitter is enabled at the correct data rate",
            side, lane);
    }

    rc = ariesPMAPCSRxReqBlock(device, side, lane, true);
    CHECK_SUCCESS(rc);

    rc = ariesPipeRxEqInProgressSet(device, side, lane, false);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeFomGet(AriesDeviceType* device, int side, int lane,
                               int* fom)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};

    rc = ariesReadRetimerRegister(device->i2cDriver, side, lane,
                                  ARIES_RET_PTH_LN_PHY_MAC_FOMFEEDBACK_ADDR, 1,
                                  dataByte);
    CHECK_SUCCESS(rc);
    fom[0] = dataByte[0];
    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRxStandbySet(AriesDeviceType* device, int side,
                                     int lane, bool value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    if (value)
    {
        dataByte[0] = 3; // en=1, val=1
    }
    else
    {
        dataByte[0] = 2; // en=1, val=0
    }
    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_MAC_PHY_RXSTANDBY_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeTxElecIdleSet(AriesDeviceType* device, int side,
                                      int lane, bool value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t adjSide = 0;
    uint8_t lane_base = lane & 0xfe; // for access to ret_pth_gbl register, use
                                     // a ret_pth_ln offset of 0

    // txdatavalid and txelecidle for pid N (N even) is captured in path N+1
    adjSide = side ? 0 : 1;

    // The MAC must always have TxDataValid asserted when TxElecIdle transitions
    // to either asserted or deasserted
    dataByte[0] = 0xf;
    rc = ariesWriteRetimerRegister(device->i2cDriver, adjSide, lane_base,
                                   ARIES_RET_PTH_GBL_MAC_PHY_TXDATAVALID_ADDR,
                                   1, dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = (3 << 2) | (value << 1) | value;
    rc = ariesWriteRetimerRegister(device->i2cDriver, adjSide, lane_base,
                                   ARIES_RET_PTH_GBL_MAC_PHY_TXELECIDLE_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);
    dataByte[0] = 0x0;
    rc = ariesWriteRetimerRegister(device->i2cDriver, adjSide, lane_base,
                                   ARIES_RET_PTH_GBL_MAC_PHY_TXDATAVALID_ADDR,
                                   1, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRxEqEval(AriesDeviceType* device, int side, int lane,
                                 bool value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    if (value == true)
    {
        dataByte[0] = 3; // en=1, val=1
    }
    else
    {
        dataByte[0] = 2; // en=1, val=0
    }
    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_MAC_PHY_RXEQEVAL_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRxEqInProgressSet(AriesDeviceType* device, int side,
                                          int lane, bool value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    // self.csr.__dict__[f'qs_{qs}'].__dict__[f'pth_wrap_{pth_wrap}']
    // .__dict__[f'ret_pth_ln{ret_ln}'].mac_phy_rxeqinprogress = 3
    if (value == true)
    {
        dataByte[0] = 3; // en=1, val=1
    }
    else
    {
        dataByte[0] = 2; // en=1, val=0
    }

    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_MAC_PHY_RXEQINPROGRESS, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipePhyStatusClear(AriesDeviceType* device, int side,
                                       int lane)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t adjSide = 0;

    // PhyStatus for pid N (N even) is captured in path N+1
    adjSide = side ? 0 : 1;

    // Write 1 to bits 2:1 to clear these flags
    dataByte[0] = 0x6;
    rc = ariesWriteRetimerRegister(device->i2cDriver, adjSide, lane,
                                   ARIES_RET_PTH_LN_PHY_MAC_PHYSTATUS_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipePhyStatusGet(AriesDeviceType* device, int side,
                                     int lane, bool* value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t adjSide = 0;

    // PhyStatus for pid N (N even) is captured in path N+1
    adjSide = side ? 0 : 1;

    rc = ariesReadRetimerRegister(device->i2cDriver, adjSide, lane,
                                  ARIES_RET_PTH_LN_PHY_MAC_PHYSTATUS_ADDR, 1,
                                  dataByte);
    CHECK_SUCCESS(rc);

    if ((dataByte[0] & 0x6) == 0)
    {
        *value = 0;
    }
    else
    {
        *value = 1;
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipePhyStatusToggle(AriesDeviceType* device, int side,
                                        int lane)
{
    AriesErrorType rc;
    uint8_t count = 0;
    bool phyStatus = 0;

    while (count < 100)
    {
        rc = ariesPipePhyStatusGet(device, side, lane, &phyStatus);
        CHECK_SUCCESS(rc);
        if (phyStatus == 1)
        {
            break;
        }
        count++;
    }

    if (phyStatus == 0)
    {
        ASTERA_ERROR("Side:%d, Lane:%2d, PhyStatus=0", side, lane);
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipePowerdownSet(AriesDeviceType* device, int side,
                                     int lane, int value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t curPowerState;
    uint8_t lane_base = lane & 0xfe; // for access to ret_pth_gbl register, use
                                     // a ret_pth_ln offset of 0
    // Put paths into new Pstate if not already
    rc = ariesReadRetimerRegister(device->i2cDriver, side, lane_base,
                                  ARIES_RET_PTH_GBL_MAC_PHY_POWERDOWN_ADDR, 1,
                                  dataByte);
    CHECK_SUCCESS(rc);
    curPowerState = dataByte[0] & 0xf;
    if (curPowerState != value)
    {
        // If we are transitioning P0 -> P1
        if (curPowerState == 0 && value == 2)
        {
            // Put Transmitters into electrical idle
            rc = ariesPipeTxElecIdleSet(device, side, lane, true);
            CHECK_SUCCESS(rc);
        }
        // Clear PhyStatus
        rc = ariesPipePhyStatusClear(device, side, lane);
        CHECK_SUCCESS(rc);
        // Set Powerdown
        dataByte[0] = (1 << 4) | (value & 0xf); // bit 4 is en
        rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane_base,
                                       ARIES_RET_PTH_GBL_MAC_PHY_POWERDOWN_ADDR,
                                       1, dataByte);
        CHECK_SUCCESS(rc);
        // Wait for PhyStatus == 1
        rc = ariesPipePhyStatusToggle(device, side, lane);
        CHECK_SUCCESS(rc);
        // If we are transitioning P1 -> P0
        if (curPowerState == 2 && value == 0)
        {
            // Take Transmitters out of electrical idle
            rc = ariesPipeTxElecIdleSet(device, side, lane, false);
            CHECK_SUCCESS(rc);
        }
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipePowerdownCheck(AriesDeviceType* device, int side,
                                       int lane, int value)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = lane / 4;
    int qsLane = lane % 4;

    // Read the txX_pstate register
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_TX_ASIC_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    uint8_t tx_pstate = (dataWord[0] >> 6) & 0x3; // PSTATE is bits 7:6
    uint8_t tx_pstate_expect = 0;
    if (value == 0) // P0
    {
        tx_pstate_expect = 0;
    }
    else if (value == 2) // P1
    {
        tx_pstate_expect = 2;
    }
    else
    {
        ASTERA_ERROR("Side:%d, Lane:%2d, unsupported POWERDOWN value %d", side,
                     lane, value);
    }
    // check if value matches expectation
    if (tx_pstate != tx_pstate_expect)
    {
        ASTERA_ERROR(
            "Side:%d, Lane:%2d, txX_pstate (%d) does not match expected value (%d)",
            side, lane, tx_pstate, tx_pstate_expect);
    }

    // Read the rxX_pstate register
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_ASIC_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    uint8_t rx_pstate = (dataWord[0] >> 5) & 0x3; // PSTATE is bits 6:5
    uint8_t rx_pstate_expect = 0;
    if (value == 0) // P0
    {
        rx_pstate_expect = 0;
    }
    else if (value == 2) // P1
    {
        rx_pstate_expect = 2;
    }
    else
    {
        ASTERA_ERROR("Side:%d, Lane:%2d, unsupported POWERDOWN value %d", side,
                     lane, value);
    }
    // check if value matches expectation
    if (rx_pstate != rx_pstate_expect)
    {
        ASTERA_ERROR(
            "Side:%d, Lane:%2d, rxX_pstate (%d) does not match expected value (%d)",
            side, lane, rx_pstate, rx_pstate_expect);
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRateChange(AriesDeviceType* device, int side, int lane,
                                   int rate)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t pipeRate;
    uint8_t curPipeRate;
    if ((rate > 5) || (rate < 1))
    {
        ASTERA_ERROR("rate argument must be 1, 2, ..., or 5");
        return ARIES_INVALID_ARGUMENT;
    }
    pipeRate = rate - 1;
    // Check if rate needs to be updated
    rc = ariesReadRetimerRegister(
        device->i2cDriver, side, lane,
        ARIES_RET_PTH_GBL_MAC_PHY_RATE_AND_PCLK_RATE_ADDR, 1, dataByte);
    CHECK_SUCCESS(rc);
    // PIPE rate is 0, 1, .., 4 for Gen1, Gen2, ..., Gen5
    curPipeRate = dataByte[0] & 0x7;
    if (curPipeRate != pipeRate)
    {
        // Put receveiers into standby (both receivers in the path)
        rc = ariesPipeRxStandbySet(device, side, lane, true);
        CHECK_SUCCESS(rc);
        rc = ariesPipeRxStandbySet(device, side, lane + 1, true);
        CHECK_SUCCESS(rc);
        // Put Transmitters into electrical idle
        rc = ariesPipeTxElecIdleSet(device, side, lane, true);
        CHECK_SUCCESS(rc);
        // Clear PhyStatus
        rc = ariesPipePhyStatusClear(device, side, lane);
        CHECK_SUCCESS(rc);
        // Change rate on the PIPE interface
        // Bit 7 and bit 3 are override enable for pclk_rate and rate,
        // respectively
        dataByte[0] = 0x88 | (pipeRate << 4) | (pipeRate & 0xf);
        rc = ariesWriteRetimerRegister(
            device->i2cDriver, side, lane,
            ARIES_RET_PTH_GBL_MAC_PHY_RATE_AND_PCLK_RATE_ADDR, 1, dataByte);
        CHECK_SUCCESS(rc);
        // Wait for PhyStatus == 1
        rc = ariesPipePhyStatusToggle(device, side, lane);
        CHECK_SUCCESS(rc);
        // // Take Receivers out of standby (Now done in test_mode_rx_config)
        // rc = ariesPipeRxStandbySet(device, side, lane_base, false);
        // CHECK_SUCCESS(rc);
        // rc = ariesPipeRxStandbySet(device, side, lane_base + 1, false);
        // CHECK_SUCCESS(rc);
        // // Take Transmitters out of electrical idle
        // rc = ariesPipeTxElecIdleSet(device, side, lane, false);
        // CHECK_SUCCESS(rc);
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRateCheck(AriesDeviceType* device, int side, int lane,
                                  int rate)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = lane / 4;
    int qsLane = lane % 4;
    if ((rate > 5) || (rate < 1))
    {
        ASTERA_ERROR("rate argument must be 1, 2, ..., or 5");
        return ARIES_INVALID_ARGUMENT;
    }

    // Read the txX_rate register
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_TX_ASIC_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    uint8_t tx_rate = dataWord[1] & 0x7; // RATE is bits 10:8
    uint8_t tx_rate_expect = 0;
    if (rate == 1) // Gen1
    {
        tx_rate_expect = 3;
    }
    else if (rate == 2) // Gen2
    {
        tx_rate_expect = 2;
    }
    else if (rate == 3) // Gen3
    {
        tx_rate_expect = 2;
    }
    else if (rate == 4) // Gen4
    {
        tx_rate_expect = 1;
    }
    else if (rate == 5) // Gen5
    {
        tx_rate_expect = 0;
    }
    // check if value matches expectation
    if (tx_rate != tx_rate_expect)
    {
        ASTERA_ERROR(
            "Side:%d, Lane:%2d, txX_rate (%d) does not match expected value (%d) for Gen%d",
            side, lane, tx_rate, tx_rate_expect, rate);
    }

    // Read the rxX_rate register
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_ASIC_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    uint8_t rx_rate = (dataWord[0] & 0x80) >> 7;
    rx_rate |= (dataWord[1] & 0x3) << 1; // RATE is bits 9:7
    uint8_t rx_rate_expect = 0;
    if (rate == 1) // Gen1
    {
        rx_rate_expect = 3;
    }
    else if (rate == 2) // Gen2
    {
        rx_rate_expect = 2;
    }
    else if (rate == 3) // Gen3
    {
        rx_rate_expect = 1;
    }
    else if (rate == 4) // Gen4
    {
        rx_rate_expect = 0;
    }
    else if (rate == 5) // Gen5
    {
        rx_rate_expect = 0;
    }
    // check if value matches expectation
    if (rx_rate != rx_rate_expect)
    {
        if (rate != 3) // Suppress this check for Gen3
        {
            ASTERA_ERROR(
                "Side:%d, Lane:%2d, rxX_rate (%d) does not match expected value (%d) for Gen%d",
                side, lane, rx_rate, rx_rate_expect, rate);
        }
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeDeepmhasisSet(AriesDeviceType* device, int side,
                                      int lane, int de, int preset, int pre,
                                      int main, int pst)
{
    AriesErrorType rc;
    uint8_t dataByte3[3] = {0};
    uint32_t deemph;
    if (de != ARIES_PIPE_DEEMPHASIS_DE_NONE)
    {
        deemph = de;
        // ASTERA_INFO("Side:%d, Lane:%2d, Setting de-emphasis value to 0x%x
        // for Gen1/2", side, lane, deemph);
    }
    else if (preset != ARIES_PIPE_DEEMPHASIS_PRESET_NONE)
    {
        switch (preset)
        {
            case 4:
                deemph = (48 << 6) | (0 << 0) | (0 << 12);
                break;
            case 1:
                deemph = (40 << 6) | (0 << 0) | (8 << 12);
                break;
            case 0:
                deemph = (36 << 6) | (0 << 0) | (12 << 12);
                break;
            case 9:
                deemph = (40 << 6) | (8 << 0) | (0 << 12);
                break;
            case 8:
                deemph = (36 << 6) | (6 << 0) | (6 << 12);
                break;
            case 7:
                deemph = (34 << 6) | (5 << 0) | (9 << 12);
                break;
            case 5:
                deemph = (44 << 6) | (4 << 0) | (0 << 12);
                break;
            case 6:
                deemph = (42 << 6) | (6 << 0) | (0 << 12);
                break;
            case 3:
                deemph = (42 << 6) | (0 << 0) | (6 << 12);
                break;
            case 2:
                deemph = (38 << 6) | (0 << 0) | (10 << 12);
                break;
            case 10:
                deemph = (32 << 6) | (0 << 0) | (16 << 12);
                break;
            default:
                deemph = (44 << 6) | (4 << 0) | (0 << 12);
        }
        // ASTERA_INFO("Side:%d, Lane:%2d, Setting de-emphasis value to 0x%x
        // (Preset %d) for Gen3/4/5", side, lane, deemph, preset);
    }
    else
    {
        deemph = (main << 6) | (pre << 0) | (pst << 12);
        // ASTERA_INFO("Side:%d, Lane:%2d, Setting de-emphasis value to 0x%x
        // (custom)", side, lane, deemph);
    }
    // self.csr.__dict__['qs_' + str(qs)].__dict__['pth_wrap_' +
    // str(pth_wrap)].__dict__['ret_pth_ln'+str(ret_ln)].mac_phy_txdeemph = ((1
    // << 18) | deemph) # en is bit 18
    dataByte3[2] = (1 << 2) | ((deemph >> 16) & 0xff);
    dataByte3[1] = ((deemph >> 8) & 0xff);
    dataByte3[0] = (deemph & 0xff);
    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_MAC_PHY_TXDEEMPH_ADDR, 3,
                                   dataByte3);
    CHECK_SUCCESS(rc);

    // Read back
    // rc = ariesReadRetimerRegister(device->i2cDriver, side, lane,
    //    ARIES_RET_PTH_LN_MAC_PHY_TXDEEMPH_OB_ADDR, 3, dataByte3);
    // CHECK_SUCCESS(rc);
    // uint32_t value = (dataByte3[2]<<16) | (dataByte3[1]<<8) | (dataByte3[0]);
    // ASTERA_INFO("Side:%d, Lane:%2d, de-emphasis read-back: 0x%x", side,
    // lane, value);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRxPolaritySet(AriesDeviceType* device, int side,
                                      int lane, int value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    // self.csr.__dict__['qs_' + str(qs)].__dict__['pth_wrap_' +
    // str(pth_wrap)].__dict__['ret_pth_ln' + str(ret_ln)].mac_phy_rxpolarity =
    // (1 << 1) | polarity # en is bit 1
    dataByte[0] = (1 << 1) | (value & 1); // bit 1 is enable
    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_MAC_PHY_RXPOLARITY_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeRxTermSet(AriesDeviceType* device, int side, int lane,
                                  bool value)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    // self.csr.__dict__['qs_' + str(qs)].__dict__['pth_wrap_' +
    // str(pth_wrap)].__dict__['ret_pth_ln' + str(ret_ln)].pcs_rx_termination =
    // rx_termination
    dataByte[0] = value ? 1 : 0;
    rc = ariesWriteRetimerRegister(device->i2cDriver, side, lane,
                                   ARIES_RET_PTH_LN_PCS_RX_TERMINATION_ADDR, 1,
                                   dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPipeBlkAlgnCtrlSet(AriesDeviceType* device, int side,
                                       int lane, bool value, bool enable)
{
    AriesErrorType rc;
    uint8_t lane_base = lane & 0xfe; // for access to ret_pth_gbl register, use
                                     // a ret_pth_ln offset of 0
    uint8_t dataByte[1] = {0};
    // self.csr.__dict__['qs_' + str(qs)].__dict__['pth_wrap_' +
    // str(pth_wrap)].ret_pth_gbl.mac_phy_blockaligncontrol = (1 << 1) |
    // blockaligncontrol # en is bit 1
    dataByte[0] = (enable << 1) | value; // bit 1 is enable
    rc = ariesWriteRetimerRegister(
        device->i2cDriver, side, lane_base,
        ARIES_RET_PTH_GBL_MAC_PHY_BLOCKALIGNCONTROL_ADDR, 1, dataByte);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMABertPatChkSts(AriesDeviceType* device, int side,
                                     int lane, int* ecount)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;
    int ecountVal = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    // Double-read required
    rc = ariesReadWordPmaLaneMainMicroIndirect(device->i2cDriver, side, qs,
                                               qsLane, 0x108d, dataWord);
    CHECK_SUCCESS(rc);
    rc = ariesReadWordPmaLaneMainMicroIndirect(device->i2cDriver, side, qs,
                                               qsLane, 0x108d, dataWord);
    CHECK_SUCCESS(rc);

    ecountVal = (dataWord[1] << 8) | dataWord[0];
    if (ecountVal >= 32768)
    {
        ecountVal = ecountVal >> 1;
        if (ecountVal == (32768 - 1))
        {
            ASTERA_INFO("Side:%d, Lane:%2d, Error Count saturated", side, lane);
        }
        ecountVal = ecountVal * 128;
    }
    ecount[0] = ecountVal;

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMABertPatChkToggleSync(AriesDeviceType* device, int side,
                                            int lane)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    // bert_pat_chk_toggle_sync(ipid, lane=pma_ln)
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);
    // ipid.__dict__['LANE'+lane_str+'_DIG_RX_LBERT_CTL'].SYNC = 1
    dataWord[0] |= (1 << 4);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);
    // ipid.__dict__['LANE'+lane_str+'_DIG_RX_LBERT_CTL'].SYNC = 0
    dataWord[0] &= ~(1 << 4);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMABertPatChkDetectCorrectPolarity(AriesDeviceType* device,
                                                       int side, int lane)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;
    int ecount[1];
    int invert_ovrd_en = 0;
    int invert = 0;
    int cur_polarity = 0;
    int new_polarity = 0;

    qs = lane / 4;
    qsLane = lane % 4;

    // Read the error counter
    rc = ariesPMABertPatChkSts(device, side, lane, ecount);
    CHECK_SUCCESS(rc);

    if (ecount[0] == 4194176)
    {
        ASTERA_INFO("Side:%d, Lane:%2d, Invert polarity", side, lane);
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            device->i2cDriver, side, qs, qsLane,
            ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_0, dataWord);
        CHECK_SUCCESS(rc);
        invert_ovrd_en = (dataWord[0] >> 3) & 0x1;
        invert = (dataWord[0] >> 2) & 0x1;
        if (invert_ovrd_en)
        {
            cur_polarity = invert;
        }
        else
        {
            cur_polarity = 0;
        }
        new_polarity = 1 - cur_polarity;
        // Set new polarity
        rc = ariesPMARxInvertSet(device, side, lane, new_polarity, 1);
        CHECK_SUCCESS(rc);
        // CLear error counter
        rc = ariesPMABertPatChkToggleSync(device, side, lane);
        CHECK_SUCCESS(rc);
        rc = ariesPMABertPatChkToggleSync(device, side, lane);
        CHECK_SUCCESS(rc);
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMARxInvertSet(AriesDeviceType* device, int side, int lane,
                                   bool invert, bool override)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    if (invert)
    {
        dataWord[0] |= (1 << 2);
    }
    else
    {
        dataWord[0] &= ~(1 << 2);
    }
    if (override)
    {
        dataWord[0] |= (1 << 3);
    }
    else
    {
        dataWord[0] &= ~(1 << 3);
    }
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMABertPatChkConfig(AriesDeviceType* device, int side,
                                        int lane, AriesPRBSPatternType mode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0, 0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);

    dataWord[0] &= ~(0xF);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);

    dataWord[0] |= (mode & 0xF);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_RX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);

    int i = 0;
    for (i = 0; i < 100; i++)
    {
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            device->i2cDriver, side, qs, qsLane,
            ARIES_PMA_LANE_DIG_RX_LBERT_CTL, dataWord);
        CHECK_SUCCESS(rc);
        if ((dataWord[0] & 0xF) != mode)
        {
            ASTERA_WARN(
                "Unable to set pattern check mode to %d on lane %d, retrying",
                mode, lane);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~(0xF);
            dataWord[0] |= (mode & 0xF);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_DIG_RX_LBERT_CTL, dataWord);
            CHECK_SUCCESS(rc);
        }
        else
        {
            break;
        }
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMABertPatGenConfig(AriesDeviceType* device, int side,
                                        int lane, AriesPRBSPatternType mode)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    // ASTERA_INFO("Side:%d, Lane:%2d, PRBS generator mode: %d", side, lane,
    // mode);

    // As per datasheet Table 11-340: when changing modes, you must change to
    // disabled (0) first
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_TX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);
    dataWord[0] &= ~(0xF);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_TX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);
    dataWord[0] |= (mode & 0xF);
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane, ARIES_PMA_LANE_DIG_TX_LBERT_CTL,
        dataWord);
    CHECK_SUCCESS(rc);
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        rc = ariesReadWordPmaLaneMainMicroIndirect(
            device->i2cDriver, side, qs, qsLane,
            ARIES_PMA_LANE_DIG_TX_LBERT_CTL, dataWord);
        CHECK_SUCCESS(rc);
        if ((dataWord[0] & 0xF) != (mode & 0xF))
        {
            ASTERA_WARN(
                "Unable to set pattern check mode to %d on lane %d, retrying",
                mode & 0xF, lane);
            dataWord[0] &= ~(0xF);
            dataWord[0] |= (mode & 0xF);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_DIG_TX_LBERT_CTL, dataWord);
            CHECK_SUCCESS(rc);
        }
    }

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMARxDataEnSet(AriesDeviceType* device, int side, int lane,
                                   bool value)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    if (value == false)
    {
        dataWord[0] &= ~(1 << 4);
        dataWord[0] &= ~(1 << 5);
    }
    else
    {
        dataWord[0] |= (1 << 4);
        dataWord[0] |= (1 << 5);
    }
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_RX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMATxDataEnSet(AriesDeviceType* device, int side, int lane,
                                   bool enable)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_TX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);
    if (enable == false)
    {
        dataWord[0] &= ~(1 << 6); // DATA_EN_OVRD_VAL = 0
        dataWord[0] &= ~(1 << 7); // DATA_EN_OVRD_EN = 0
    }
    else
    {
        dataWord[0] |= (1 << 6); // DATA_EN_OVRD_VAL = 1
        dataWord[0] |= (1 << 7); // DATA_EN_OVRD_EN = 1
    }
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_LANE_DIG_ASIC_TX_OVRD_IN_0, dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMAPCSRxRecalBankOvrdSet(AriesDeviceType* device, int side,
                                             int lane, bool enable)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = lane / 4;
    int qsLane = lane % 4;

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_RAWLANE_DIG_PCS_XF_RX_RECAL_BANK_OVRD, dataWord);
    CHECK_SUCCESS(rc);
    if (!enable) // Gen-5
    {
        dataWord[0] |= (1 << 1);
        dataWord[0] &= ~(1 << 0);
    }
    else // Gen-3/4
    {
        dataWord[0] |= (1 << 1);
        dataWord[0] |= (1 << 0);
    }
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_RAWLANE_DIG_PCS_XF_RX_RECAL_BANK_OVRD, dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}
AriesErrorType ariesPMAPCSRxReqBlock(AriesDeviceType* device, int side,
                                     int lane, bool enable)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;
    rc = ariesReadWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_RAWLANE_DIG_PCS_XF_ATE_OVRD_IN, dataWord);
    CHECK_SUCCESS(rc);
    if (enable)
    {
        dataWord[0] |= (1 << 5);
    }
    else
    {
        dataWord[0] &= ~(1 << 5);
    }
    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        device->i2cDriver, side, qs, qsLane,
        ARIES_PMA_RAWLANE_DIG_PCS_XF_ATE_OVRD_IN, dataWord);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

AriesErrorType ariesPMAVregVrefSet(AriesDeviceType* device, int side, int lane,
                                   int rate)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};
    int qs = 0;
    int qsLane = 0;

    qs = lane / 4;
    qsLane = lane % 4;

    // for A0 devices changing between G1-2 to G3-5
    if (device->revNumber == 1)
    {
        if (rate <= 2)
        {
            // Vreg
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] |= (1 << 7);
            dataWord[0] &= ~(1 << 6);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);

            // Vref
            // There is one control bit per PMA instance
            if ((qsLane % 4) == 0)
            {
                rc = ariesReadWordPmaMainMicroIndirect(
                    device->i2cDriver, side, qs,
                    ARIES_PMA_SUP_ANA_SWITCH_MISC_MEAS, dataWord);
                CHECK_SUCCESS(rc);
                dataWord[0] &= ~(1 << 5);
                dataWord[0] &= ~(1 << 4);
                rc = ariesWriteWordPmaMainMicroIndirect(
                    device->i2cDriver, side, qs,
                    ARIES_PMA_SUP_ANA_SWITCH_MISC_MEAS, dataWord);
                CHECK_SUCCESS(rc);
            }
        }
        else
        {
            // Vreg
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~(1 << 7);
            dataWord[0] |= (1 << 6);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);

            // Vref
            // There is one control bit per PMA instance
            if ((qsLane % 4) == 0)
            {
                rc = ariesReadWordPmaMainMicroIndirect(
                    device->i2cDriver, side, qs,
                    ARIES_PMA_SUP_ANA_SWITCH_MISC_MEAS, dataWord);
                CHECK_SUCCESS(rc);
                dataWord[0] |= (1 << 5);
                dataWord[0] &= ~(1 << 4);
                rc = ariesWriteWordPmaMainMicroIndirect(
                    device->i2cDriver, side, qs,
                    ARIES_PMA_SUP_ANA_SWITCH_MISC_MEAS, dataWord);
                CHECK_SUCCESS(rc);
            }
        }
    }
    // for B0 devices changing between G1-3 to G4-5
    else
    {
        if (rate <= 3)
        {
            // Vreg
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~(1 << 5);
            dataWord[0] &= ~(1 << 4);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc)
        }
        else
        {
            // Vreg
            rc = ariesReadWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc);
            dataWord[0] &= ~(1 << 5);
            dataWord[0] |= (1 << 4);
            rc = ariesWriteWordPmaLaneMainMicroIndirect(
                device->i2cDriver, side, qs, qsLane,
                ARIES_PMA_LANE_ANA_RX_VREG_CTRL2, dataWord);
            CHECK_SUCCESS(rc)
        }
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Read multiple data bytes from Aries over I2C. Purposely force an error
 * in the transaction by injecting an intervening I2C transaction in the middle
 * between the Write and Read.
 */
AriesErrorType ariesReadBlockDataForceError(AriesI2CDriverType* i2cDriver,
                                            uint32_t address, uint8_t numBytes,
                                            uint8_t* values)
{
    AriesErrorType rc;
    AriesErrorType lc;
    int readBytes;

    uint8_t writeBuf[3] = {0}; // Max of (2):Intel and (3):Astera
    uint8_t readBuf[16] = {0}; // Max of (2+4):Intel and (16):Astera

    uint8_t wrCmdCode;
    uint8_t rdCmdCode;
    uint8_t pecEn;
    uint8_t rsvd;
    uint8_t funcCode;
    uint8_t start;
    uint8_t end;

    uint8_t addr15To8; // Get bits 15:8
    uint8_t addr7To0;  // Get bits 7:0

    uint8_t tryIndex;
    uint8_t readTryCount = 3;

    uint8_t currBytes = 0;
    uint8_t remainingBytes = numBytes;
    if (i2cDriver->i2cFormat == ARIES_I2C_FORMAT_INTEL)
    {
        // Addresses in this format can be 16 bit only
        if (address > 0xffff)
        {
            ASTERA_ERROR("Address cannot be more than 16 bits in Intel format");
            return ARIES_INVALID_ARGUMENT;
        }

        // If byte count is greater than 4, perform multiple iterations
        while (remainingBytes > 0)
        {
            if (remainingBytes > 4)
            {
                currBytes = 4;
                remainingBytes -= 4;
            }
            else
            {
                currBytes = remainingBytes;
                remainingBytes = 0;
            }

            pecEn = 0;
            rsvd = 0;
            funcCode = 0;
            start = 1;
            end = 0;

            if (i2cDriver->pecEnable == ARIES_I2C_PEC_ENABLE)
            {
                pecEn = 1;
            }

            ASTERA_TRACE("Reading from address: 0x%08x", address);

            // Construct command code
            wrCmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                        (start << 1) + (end << 0);

            addr15To8 = (address & 0xff00) >> 8;
            addr7To0 = address & 0xff;

            // Construct data buffer
            writeBuf[0] = addr7To0;
            writeBuf[1] = addr15To8;

            ASTERA_TRACE("Write:");
            ASTERA_TRACE("  cmdCode = 0x%02x", wrCmdCode);
            ASTERA_TRACE("  byteCount = 0x02");
            ASTERA_TRACE("  writeBuf[0] = 0x%02x", writeBuf[0]);
            ASTERA_TRACE("  writeBuf[1] = 0x%02x", writeBuf[1]);

            // Set up I2C lock, to keep write and read atomic
            // Do not unblock here on return of error since this error code
            // would mean that the block call did not happen
            lc = ariesLock(i2cDriver);
            CHECK_SUCCESS(lc);

            // Perform read operation
            // Try upto 3 times before issuing a block read failure
            for (tryIndex = 0; tryIndex < readTryCount; tryIndex++)
            {
                rc = asteraI2CWriteBlockData(i2cDriver->handle, wrCmdCode, 2,
                                             writeBuf);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }

                // Inject an intervening transaction
                rc = asteraI2CWriteBlockData(i2cDriver->handle, 0, 0, writeBuf);
                if (rc != ARIES_SUCCESS)
                {
                    ASTERA_WARN("Received expected non-zero return code "
                                "during invalid transaction");
                }

                start = 0;
                end = 1;
                funcCode = 0;

                rdCmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                            (start << 1) + (end << 0);

                ASTERA_TRACE("Read:");
                ASTERA_TRACE("  cmdCode = 0x%02x", rdCmdCode);

                readBytes = asteraI2CReadBlockData(i2cDriver->handle, rdCmdCode,
                                                   currBytes, readBuf);
                if (readBytes != currBytes)
                {
                    ASTERA_TRACE("Expected %d bytes but got %d", currBytes,
                                 readBytes);
                    ASTERA_TRACE("Perform read again...");

                    if (tryIndex == (readTryCount - 1))
                    {
                        ASTERA_ERROR("Incorrect num. bytes returned by read");
                        lc = ariesUnlock(i2cDriver);
                        CHECK_SUCCESS(lc);
                        return ARIES_I2C_BLOCK_READ_FAILURE;
                    }
                }
                else
                {
                    break;
                }
            }

            // Unlock previous lock set before write
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);

            // Print the values
            ASTERA_TRACE("Values read:");
            int pos;
            for (pos = 0; pos < currBytes; pos++)
            {
                values[pos] = readBuf[2 + pos];
                ASTERA_TRACE("  readBuf[%d] = 0x%02x", pos, values[pos]);
            }
            // Increment iteration count
            values += currBytes;
            address += currBytes;
        }
    }
    else if (i2cDriver->i2cFormat == ARIES_I2C_FORMAT_ASTERA)
    {
        // If byte count is greater than 16, perform multiple iterations
        while (remainingBytes > 0)
        {
            if (remainingBytes > 16)
            {
                currBytes = 16;
                remainingBytes -= 16;
            }
            else
            {
                currBytes = remainingBytes;
                remainingBytes = 0;
            }

            pecEn = 0;
            rsvd = 0;
            funcCode = 2;
            start = 1;
            end = 0;

            if (i2cDriver->pecEnable == ARIES_I2C_PEC_ENABLE)
            {
                pecEn = 1;
            }

            ASTERA_TRACE("Reading from address: 0x%08x", address);

            // Construct command code
            wrCmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                        (start << 1) + (end << 0);

            // Construct Config & Offset Upper byte
            uint8_t config;
            uint8_t addr16;
            addr16 = (address & 0x10000) >> 16;  // Get 16bit of addr
            addr15To8 = (address & 0xff00) >> 8; // Get bits 15:8
            addr7To0 = address & 0xff;           // Get bits 7:0

            uint8_t cfg_type = 0;
            uint8_t bdcst = 0;
            uint8_t burstLen = currBytes - 1;

            config = (cfg_type << 6) + (bdcst << 4) + (burstLen << 1) +
                     (addr16 << 0);

            // Construct data buffer
            writeBuf[0] = config;
            writeBuf[1] = addr15To8;
            writeBuf[2] = addr7To0;

            ASTERA_TRACE("Write:");
            ASTERA_TRACE("  cmdCode = 0x%02x", wrCmdCode);
            ASTERA_TRACE("  byteCount = 0x03");
            ASTERA_TRACE("  writeBuf[0] = 0x%02x", writeBuf[0]);
            ASTERA_TRACE("  writeBuf[1] = 0x%02x", writeBuf[1]);
            ASTERA_TRACE("  writeBuf[2] = 0x%02x", writeBuf[2]);

            // Set up I2C lock, to keep write and read atomic
            // Do not unblock here on return of error since this error code
            // would mean that the block call did not happen
            lc = ariesLock(i2cDriver);
            CHECK_SUCCESS(lc);

            // Perform read operation
            // Try upto 3 times before issuing a block read failure
            for (tryIndex = 0; tryIndex < readTryCount; tryIndex++)
            {
                // First write address you wish to read from
                rc = asteraI2CWriteBlockData(i2cDriver->handle, wrCmdCode, 3,
                                             writeBuf);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }

                // Inject an intervening transaction
                rc = asteraI2CWriteBlockData(i2cDriver->handle, 0, 0, writeBuf);
                if (rc != ARIES_SUCCESS)
                {
                    ASTERA_WARN("Received expected non-zero return code "
                                "during invalid transaction");
                }

                funcCode = 2;
                start = 0;
                end = 1;
                rdCmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                            (start << 1) + (end << 0);

                ASTERA_TRACE("Read:");
                ASTERA_TRACE("  cmdCode = 0x%02x", rdCmdCode);

                // Then issue read
                // First byte returned is length
                readBytes = asteraI2CReadBlockData(i2cDriver->handle, rdCmdCode,
                                                   currBytes, readBuf);
#ifdef SMBUS_BLOCK_READ_UNSUPPORTED
                readBytes -= 1;
                int i;
                for (i = 0; i < readBytes; i++)
                {
                    readBuf[i] = readBuf[i + 1];
                }
#endif
                if (readBytes != currBytes)
                {
                    ASTERA_TRACE("Expected %d bytes but got %d", currBytes,
                                 readBytes);
                    ASTERA_TRACE("Perform read again...");

                    if (tryIndex == (readTryCount - 1))
                    {
                        ASTERA_ERROR("Incorrect num. bytes returned by read");
                        lc = ariesUnlock(i2cDriver);
                        CHECK_SUCCESS(lc);
                        return ARIES_I2C_BLOCK_READ_FAILURE;
                    }
                }
                else
                {
                    break;
                }
            }

            // Unlock previous lock set before write
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);

            // Print the values
            ASTERA_TRACE("Values read:");
            int pos;
            for (pos = 0; pos < currBytes; pos++)
            {
                values[pos] = readBuf[pos];
                ASTERA_TRACE("  readBuf[%d] = 0x%02x", pos, values[pos]);
            }
            values += currBytes;
            address += currBytes;
        }
    }
    else if (i2cDriver->i2cFormat == ARIES_I2C_FORMAT_CMBND)
    {
        return ariesWriteReadBlockData(i2cDriver, address, numBytes, values);
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }
    return ARIES_SUCCESS;
}

/**
 * @brief Convert the PMA ADC code to degrees Celsius
 *
 * @param[in]  device   Aries Device struct
 * @param[in]  adcCode     Temperature ACD code from PMA
 * @param[in]  calCode       PMA calibration code
 * @return     temperature_C Temperature in Degrees Celsius
 */
float ariesTsenseADCToDegrees(AriesDeviceType* device, int adcCode,
                              uint8_t calCode)
{
    float temperature_C;

    temperature_C = device->tempCalCodeRefTempC +
                    ((adcCode - (calCode + 250)) * -0.32);

    return temperature_C;
}

#ifdef __cplusplus
}
#endif
