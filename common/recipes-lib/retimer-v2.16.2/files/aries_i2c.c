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
 * @file aries_i2c.c
 * @brief Implementation of public functions for the SDK I2C interface.
 */

#include "aries_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set Slave address to user-specified value: new7bitSmbusAddr
 *
 * @param[in]  handle            Handle to I2C driver
 * @param[in]  new7bitSmbusAddr  Desired I2C (7-bit) address of Retimer
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesRunArp(int handle, uint8_t new7bitSmbusAddr)
{
    AriesErrorType rc;
    uint8_t dataByte[1] = {0};
    uint8_t dataBytes19[19] = {0};

    dataByte[0] = 0xc9;
    rc = asteraI2CWriteBlockData(handle, 0x2, 1, dataByte);
    CHECK_SUCCESS(rc);

    dataByte[0] = 0xc0;
    rc = asteraI2CWriteBlockData(handle, 0x1, 1, dataByte);
    CHECK_SUCCESS(rc);

    // This read will get first device repsonse in ARP mode
    rc = asteraI2CReadBlockData(handle, 0x3, 19, dataBytes19);
    if (rc != 19)
    {
        return ARIES_I2C_BLOCK_READ_FAILURE;
    }

    // The PEC byte is calculated from 20 bytes of data including the ARP slave
    // address byte (0x61<<1 = 0xc2), the command code (0x4), and the remainder
    // of the Assign Address command.
    uint8_t new8bitSmbusAddr = new7bitSmbusAddr << 1;
    uint8_t pec_data[21] = {
        0xc2, 4, 17, 1, 9, 29, 250, 0, 1, 0, 21,
        0,    0, 0,  0, 0, 0,  0,   1, 0, 0}; // last byte is new 8-bit SMBus
                                              // address
    pec_data[19] = new8bitSmbusAddr;
    uint8_t crc = ariesGetPecByte(pec_data, 21);

    // Program slave address of device to 0x55
    int i;
    for (i = 0; i < 18; i++)
    {
        dataBytes19[i] = pec_data[i + 2];
    }
    dataBytes19[18] = crc; // PEC byte

    rc = asteraI2CWriteBlockData(handle, 0x4, 19, dataBytes19);
    CHECK_SUCCESS(rc);

    return ARIES_SUCCESS;
}

/**
 * @brief Write multiple bytes of data at specified address to Aries over I2C.
 * This function returns a negative error code if unsuccessful, else zero on
 * success.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      17-bit address to write
 * @param[in]  numBytes     Number of bytes to write
 * @param[in]  values       Pointer to array of data (bytes) you wish to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteBlockData(AriesI2CDriverType* i2cDriver,
                                   uint32_t address, uint8_t numBytes,
                                   uint8_t* values)
{
    AriesErrorType rc;
    // Max of (2+currBytes):Intel and (3+currBytes):Astera
    uint8_t writeBuf[7] = {0};
    uint8_t wrBufLength;

    uint8_t cmdCode;
    uint8_t pecEn;
    uint8_t rsvd;
    uint8_t funcCode;
    uint8_t start;
    uint8_t end;

    uint8_t addr15To8;
    uint8_t addr7To0;

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
            funcCode = 1;
            start = 1;
            end = 1;

            if (i2cDriver->pecEnable == ARIES_I2C_PEC_ENABLE)
            {
                pecEn = 1;
            }

            ASTERA_TRACE("Reading from address: 0x%08x", address);

            wrBufLength = 2 + currBytes;

            // Construct Command code
            cmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                      (start << 1) + (end << 0);

            addr15To8 = (address & 0xff00) >> 8; // Get bits 15:8
            addr7To0 = address & 0xff;           // Get bits 7:0

            // Construct data buffer
            writeBuf[0] = addr7To0;
            writeBuf[1] = addr15To8;

            ASTERA_TRACE("Write:");
            ASTERA_TRACE("  cmdCode = 0x%02x", cmdCode);
            ASTERA_TRACE("  writeBuf[0] = 0x%02x", writeBuf[0]);
            ASTERA_TRACE("  writeBuf[1] = 0x%02x", writeBuf[1]);

            // Fill up data buffer
            int pos;
            int bufPos;
            for (pos = 0; pos < currBytes; pos++)
            {
                bufPos = pos + 2;
                writeBuf[bufPos] = values[pos];
                ASTERA_TRACE("  writeBuf[%d] = 0x%02x", bufPos,
                             writeBuf[bufPos]);
            }

            rc = asteraI2CWriteBlockData(i2cDriver->handle, cmdCode,
                                         wrBufLength, writeBuf);
            CHECK_SUCCESS(rc);

            // Increment iteration count
            values += currBytes;
            address += currBytes;
        }
    }
    else if (i2cDriver->i2cFormat == ARIES_I2C_FORMAT_ASTERA ||
             i2cDriver->i2cFormat == ARIES_I2C_FORMAT_CMBND)
    {
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
            funcCode = 3;
            start = 1;
            end = 1;

            if (i2cDriver->pecEnable == ARIES_I2C_PEC_ENABLE)
            {
                pecEn = 1;
            }

            ASTERA_TRACE("Writing to address: 0x%08x", address);

            wrBufLength = 3 + currBytes;

            // Construct Command code
            cmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                      (start << 1) + (end << 0);

            // Construct Config & Offset Upper byte
            uint8_t config;
            uint8_t addr16;
            addr16 = (address & 0x10000) >> 16;  // Get 16bit of addr
            addr15To8 = (address & 0xff00) >> 8; // Get bits 15:8
            addr7To0 = address & 0xff;           // Get bits 7:0

            uint8_t cfg_type = 1;
            uint8_t bdcst = 0;
            uint8_t burstLen = currBytes - 1;

            config = (cfg_type << 6) + (bdcst << 4) + (burstLen << 1) +
                     (addr16 << 0);

            // Construct data buffer
            writeBuf[0] = config;
            writeBuf[1] = addr15To8;
            writeBuf[2] = addr7To0;

            ASTERA_TRACE("Write:");
            ASTERA_TRACE("  cmdCode = 0x%02x", cmdCode);
            ASTERA_TRACE("  byteCount = 0x%02x", wrBufLength);
            ASTERA_TRACE("  writeBuf[0] = 0x%02x", writeBuf[0]);
            ASTERA_TRACE("  writeBuf[1] = 0x%02x", writeBuf[1]);
            ASTERA_TRACE("  writeBuf[2] = 0x%02x", writeBuf[2]);

            // Fill up data buffer
            int pos;
            int bufPos;
            for (pos = 0; pos < currBytes; pos++)
            {
                bufPos = pos + 3;
                writeBuf[bufPos] = values[pos];
                ASTERA_TRACE("  writeBuf[%d] = 0x%02x", bufPos,
                             writeBuf[bufPos]);
            }

            // This translates to actual low level library call
            // Function definition part of user application
            rc = asteraI2CWriteBlockData(i2cDriver->handle, cmdCode,
                                         wrBufLength, writeBuf);
            CHECK_SUCCESS(rc);
            values += currBytes;
            address += currBytes;
        }
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }
    return ARIES_SUCCESS;
}

/**
 * @brief Write a data byte at specified address to Aries over I2C.
 * This function returns a negative error code if unsuccessful, else zero on
 * success.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  address    17-bit address to write
 * @param[in]  value      Pointer to single element array of data you wish
 *                        to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteByteData(AriesI2CDriverType* i2cDriver,
                                  uint32_t address, uint8_t* value)
{
    return ariesWriteBlockData(i2cDriver, address, 1, value);
}

/**
 * @brief Read multiple bytes of data at specified address from Aries over I2C.
 * This function returns a negative error code if unsuccessful, else zero on
 * success.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      17-bit address from which to read
 * @param[in]  numBytes     Number of bytes to read
 * @param[out] values       Pointer to array of read data (bytes)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadBlockData(AriesI2CDriverType* i2cDriver,
                                  uint32_t address, uint8_t numBytes,
                                  uint8_t* values)
{
    AriesErrorType rc;
    AriesErrorType lc;
    int readBytes;

    uint8_t writeBuf[3] = {0}; // Max of (2):Intel and (3):Astera
    uint8_t readBuf[18] = {0}; // Max of (2+4):Intel and (16):Astera, or 18 if
                               // using I2C block read

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
                                                   currBytes + 1 + pecEn,
                                                   readBuf);
#ifdef SMBUS_BLOCK_READ_UNSUPPORTED
                readBytes -= 1 + pecEn;
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
#ifdef SMBUS_BLOCK_READ_UNSUPPORTED
            // Verify PEC checksum manually if using I2C block write
            if (pecEn)
            {
                // Include write portion of read (addr and wr bit, and cmd Code)
                // and extra read portion (addr and rd bit)
                uint8_t rdStreamLen = 4 + currBytes + pecEn;
                uint8_t rdStream[21];
                uint8_t rdPec;

                rdStream[0] = (i2cDriver->slaveAddr << 1) + 0;
                rdStream[1] = rdCmdCode;
                rdStream[2] = (i2cDriver->slaveAddr << 1) + 1;

                int pos;
                for (pos = 3; pos < rdStreamLen; pos++)
                {
                    rdStream[pos] = readBuf[pos - 3];
                }

                rdPec = ariesGetPecByte(rdStream, rdStreamLen);
                if (rdPec != 0)
                {
                    ASTERA_ERROR("PEC value not as expected");
                    return ARIES_I2C_BLOCK_READ_FAILURE;
                }
            }
            int i;
            for (i = 0; i < readBytes; i++)
            {
                readBuf[i] = readBuf[i + 1];
            }
#endif
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
 * @brief Read a data byte at specified address from Aries over I2C.
 * This function returns a negative error code if unsuccessful, else zero on
 * success.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  address    17-bit address from which to read
 * @param[out] values     Pointer to single element array of read data (byte)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadByteData(AriesI2CDriverType* i2cDriver,
                                 uint32_t address, uint8_t* values)
{
    return ariesReadBlockData(i2cDriver, address, 1, values);
}

/**
 * @brief Read multiple data bytes to Aries over I2C using combined transaction
 * format. This function returns a negative error code if unsuccessful, else
 * zero on success.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      17-bit address to read
 * @param[in]  numBytes     Number of bytes to read
 * @param[in]  values       Pointer to array of data (bytes) you wish to read
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteReadBlockData(AriesI2CDriverType* i2cDriver,
                                       uint32_t address, uint8_t numBytes,
                                       uint8_t* values)
{
    int readBytes;

    uint8_t writeReadBuf[16]; // Max of (currBytes):Astera

    uint8_t wrrdCmdCode;
    uint8_t pecEn;
    uint8_t rsvd;
    uint8_t funcCode;
    uint8_t start;
    uint8_t end;

    uint8_t config;
    uint8_t cfg_type;
    uint8_t bdcst;
    uint8_t burstLen;
    uint8_t addr16;
    uint8_t addr15To8;
    uint8_t addr7To0;

    uint8_t currBytes = 0;
    uint8_t remainingBytes = numBytes;
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
        wrrdCmdCode = (pecEn << 7) + (rsvd << 5) + (funcCode << 2) +
                      (start << 1) + (end << 1);

        // Construct Config & Offset Upper byte
        addr16 = (address & 0x10000) >> 16;  // Get 16bit of addr
        addr15To8 = (address & 0xff00) >> 8; // Get bits 15:8
        addr7To0 = address & 0xff;           // Get bits 7:0

        cfg_type = 0;
        bdcst = 0;
        burstLen = currBytes - 1;

        config = (cfg_type << 6) + (bdcst << 4) + (burstLen << 1) +
                 (addr16 << 0);

        // Construct data buffer
        writeReadBuf[0] = config;
        writeReadBuf[1] = addr15To8;
        writeReadBuf[2] = addr7To0;

        ASTERA_TRACE("WriteRead:");
        ASTERA_TRACE("  cmdCode = 0x%02x", wrrdCmdCode);
        ASTERA_TRACE("  byteCount = 0x03");
        ASTERA_TRACE("  writeReadBuf[0] = 0x%02x", writeReadBuf[0]);
        ASTERA_TRACE("  writeReadBuf[1] = 0x%02x", writeReadBuf[1]);
        ASTERA_TRACE("  writeReadBuf[2] = 0x%02x", writeReadBuf[2]);

        // Perform combined write read operation
        readBytes = asteraI2CWriteReadBlockData(i2cDriver->handle, wrrdCmdCode,
                                                3, writeReadBuf);
        if (readBytes != currBytes)
        {
            ASTERA_TRACE("Expected %d bytes but got %d", currBytes, readBytes);
            return ARIES_I2C_BLOCK_READ_FAILURE;
        }

        // Print the values
        ASTERA_TRACE("Values read:");
        int pos;
        for (pos = 0; pos < currBytes; pos++)
        {
            values[pos] = writeReadBuf[pos];
            ASTERA_TRACE("  writeReadBuf[%d] = 0x%02x", pos, values[pos]);
        }
        values += currBytes;
        address += currBytes;
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Read multiple (up to eight) data bytes from micro SRAM over I2C for
 * A0. Returns a negative error code, or zero on success
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  microIndStructOffset   Micro Indirect Struct Offset
 * @param[in]  address    16-bit address from which to read
 * @param[in]  numBytes  Number of bytes to read
 * @param[out] values     Pointer to single element array of read data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadBlockDataMainMicroIndirectA0(
    AriesI2CDriverType* i2cDriver, uint32_t microIndStructOffset,
    uint32_t address, uint8_t numBytes, uint8_t* values)
{
    AriesErrorType rc;
    AriesErrorType lc;
    uint8_t dataByte[1] = {0};
    uint8_t eepromAddrBytes[3] = {0};
    int byteIndex;

    // Grab lock
    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // No multi-byte indirect support here. Hence read a byte at a time
    for (byteIndex = 0; byteIndex < numBytes; byteIndex++)
    {
        // Write eeprom addr
        int eepromAccAddr = address - AL_MAIN_SRAM_DMEM_OFFSET + byteIndex;
        eepromAddrBytes[0] = eepromAccAddr & 0xff;
        eepromAddrBytes[1] = (eepromAccAddr >> 8) & 0xff;
        eepromAddrBytes[2] = (eepromAccAddr >> 16) & 0xff;
        rc = ariesWriteBlockData(i2cDriver, microIndStructOffset, 3,
                                 eepromAddrBytes);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Write eeprom cmd
        dataByte[0] = AL_TG_RD_LOC_IND_SRAM;
        rc = ariesWriteByteData(i2cDriver, (microIndStructOffset + 4),
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Test successfull access
        uint8_t status = 0xff;
        uint8_t count = 0;
        while ((status != 0) && (count < 0xff))
        {
            rc = ariesReadByteData(i2cDriver, (microIndStructOffset + 4),
                                   dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            status = dataByte[0];
            count += 1;
        }

        if (status != 0)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return ARIES_FAILURE_SRAM_IND_ACCESS_TIMEOUT;
        }
        else
        {
            rc = ariesReadByteData(i2cDriver, (microIndStructOffset + 3),
                                   dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            values[(byteIndex)] = dataByte[0];
        }
    }

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

/**
 * @brief Read multiple (up to eight) data bytes from micro SRAM over I2C.
 * Returns a negative error code, or zero on success
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  microIndStructOffset Micro Indirect Struct Offset
 * @param[in]  address    16-bit address from which to read
 * @param[in]  numBytes  Number of bytes to read
 * @param[out] values     Pointer to single element array of read data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadBlockDataMainMicroIndirectMPW(
    AriesI2CDriverType* i2cDriver, uint32_t microIndStructOffset,
    uint32_t address, uint8_t numBytes, uint8_t* values)
{
    AriesErrorType rc;
    AriesErrorType lc;

    if (numBytes > 8)
    {
        return ARIES_INVALID_ARGUMENT;
    }
    else
    {
        // Set aries transaction lock
        lc = ariesLock(i2cDriver);
        CHECK_SUCCESS(lc);

        uint8_t curOffset = 0;
        uint8_t remainingBytes = numBytes;
        uint8_t tmpValue[1];
        while (remainingBytes > 0)
        {
            uint8_t curRemainingBytes;
            if (remainingBytes > 4)
            {
                curRemainingBytes = 4;
                remainingBytes -= 4;
            }
            else
            {
                curRemainingBytes = remainingBytes;
                remainingBytes = 0;
            }

            // Set indirect access command
            tmpValue[0] = ((curRemainingBytes - 1) << 5) |
                          ((((address + curOffset) >> 16) & 0x1) << 1);
            rc = ariesWriteByteData(i2cDriver, microIndStructOffset, tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Set address upper byte
            tmpValue[0] = ((address + curOffset) >> 8) & 0xff;
            rc = ariesWriteByteData(i2cDriver, (microIndStructOffset + 1),
                                    tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Set address lower byte (this triggers indirect access)
            tmpValue[0] = (address + curOffset) & 0xff;
            rc = ariesWriteByteData(i2cDriver, (microIndStructOffset + 2),
                                    tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Test successfull access
            uint8_t status = 0;
            uint8_t rdata[1];
            uint8_t count = 0;
            while ((status == 0) && (count < 0xff))
            {
                rc = ariesReadByteData(i2cDriver, (microIndStructOffset + 0xb),
                                       rdata);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                status = rdata[0] & 0x1;
                count += 1;
            }
            if ((status == 0) || (count == 0xff))
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return ARIES_FAILURE_SRAM_IND_ACCESS_TIMEOUT;
            }
            else
            {
                uint8_t i;
                for (i = 0; i < curRemainingBytes; i++)
                {
                    rc = ariesReadByteData(
                        i2cDriver, (microIndStructOffset + 3 + i), tmpValue);
                    if (rc != ARIES_SUCCESS)
                    {
                        lc = ariesUnlock(i2cDriver);
                        CHECK_SUCCESS(lc);
                        return rc;
                    }
                    values[curOffset + i] = tmpValue[0];
                }
            }
            curOffset += curRemainingBytes;
        }

        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);

        return ARIES_SUCCESS;
    }
}

/**
 * @brief Write multiple data bytes to specified address from micro SRAM over
 * I2C for A0. Returns a negative error code, or zero on success
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  microIndStructOffset Micro Indirect Struct Offset
 * @param[in]  address    16-bit address from which to write
 * @param[in]  numBytes  Number of bytes to write
 * @param[out] values      Pointer to array of data (bytes) you wish to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteBlockDataMainMicroIndirectA0(
    AriesI2CDriverType* i2cDriver, uint32_t microIndStructOffset,
    uint32_t address, uint8_t numBytes, uint8_t* values)
{
    AriesErrorType rc;
    AriesErrorType lc;
    int byteIndex;
    uint8_t dataByte[1] = {0};

    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // No multi-byte indirect support here. Hence read a byte at a time
    for (byteIndex = 0; byteIndex < numBytes; byteIndex++)
    {
        // Write eeprom addr
        uint8_t eepromAddrBytes[3];
        int eepromAccAddr = address - AL_MAIN_SRAM_DMEM_OFFSET + byteIndex;
        eepromAddrBytes[0] = eepromAccAddr & 0xff;
        eepromAddrBytes[1] = (eepromAccAddr >> 8) & 0xff;
        eepromAddrBytes[2] = (eepromAccAddr >> 16) & 0xff;
        rc = ariesWriteBlockData(i2cDriver, microIndStructOffset, 3,
                                 eepromAddrBytes);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Write data
        dataByte[0] = values[byteIndex];
        rc = ariesWriteByteData(i2cDriver, (microIndStructOffset + 3),
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Write eeprom cmd
        uint8_t eepromCmdByte[1];
        eepromCmdByte[0] = AL_TG_WR_LOC_IND_SRAM;
        rc = ariesWriteByteData(i2cDriver, (microIndStructOffset + 4),
                                eepromCmdByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Test successfull access
        uint8_t status = 0xff;
        uint8_t rdata[1];
        uint8_t count = 0;
        while ((status != 0) && (count < 0xff))
        {
            rc = ariesReadByteData(i2cDriver, (microIndStructOffset + 4),
                                   rdata);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            status = rdata[0];
            count += 1;
        }

        if (status != 0)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return ARIES_FAILURE_SRAM_IND_ACCESS_TIMEOUT;
        }
    }

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

/**
 * @brief Write multiple (up to eight) data bytes to specified address from
 * micro SRAM over I2C. Returns a negative error code, or zero on success
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  microIndStructOffset Micro Indirect Struct Offset
 * @param[in]  address    16-bit address from which to write
 * @param[in]  numBytes  Number of bytes to write
 * @param[out] values      Pointer to array of data (bytes) you wish to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteBlockDataMainMicroIndirectMPW(
    AriesI2CDriverType* i2cDriver, uint32_t microIndStructOffset,
    uint32_t address, uint8_t numBytes, uint8_t* values)
{
    uint8_t tmpValue[1];
    uint8_t curOffset = 0;
    AriesErrorType rc;
    AriesErrorType lc;
    if (numBytes > 8)
    {
        return ARIES_INVALID_ARGUMENT;
    }
    else
    {
        lc = ariesLock(i2cDriver);
        CHECK_SUCCESS(lc);

        uint8_t remainingBytes = numBytes;
        while (remainingBytes > 0)
        {
            uint8_t curRemainingBytes;
            if (remainingBytes > 4)
            {
                curRemainingBytes = 4;
                remainingBytes -= 4;
            }
            else
            {
                curRemainingBytes = remainingBytes;
                remainingBytes = 0;
            }

            // Set indirect access command
            tmpValue[0] = ((curRemainingBytes - 1) << 5) |
                          ((((address + curOffset) >> 16) & 0x1) << 1) | 0x1;
            rc = ariesWriteByteData(i2cDriver, microIndStructOffset, tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Set the data byte(s)
            uint8_t i;
            for (i = 0; i < curRemainingBytes; i++)
            {
                tmpValue[0] = values[curOffset + i];
                rc = ariesWriteByteData(i2cDriver, microIndStructOffset + 3 + i,
                                        tmpValue);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
            }

            // Set address upper byte
            tmpValue[0] = ((address + curOffset) >> 8) & 0xff;
            rc = ariesWriteByteData(i2cDriver, microIndStructOffset + 1,
                                    tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Set address lower byte (this triggers indirect access)
            tmpValue[0] = (address + curOffset) & 0xff;
            rc = ariesWriteByteData(i2cDriver, microIndStructOffset + 2,
                                    tmpValue);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }

            // Test successfull access
            uint8_t status = 0;
            uint8_t rdata[1];
            uint8_t count = 0;
            while ((status == 0) && (count < 0xff))
            {
                rc = ariesReadByteData(i2cDriver, microIndStructOffset + 0xb,
                                       rdata);
                if (rc != ARIES_SUCCESS)
                {
                    lc = ariesUnlock(i2cDriver);
                    CHECK_SUCCESS(lc);
                    return rc;
                }
                status = rdata[0] & 0x1;
                count += 1;
            }
            if ((status == 0) || (count == 0xff))
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return ARIES_FAILURE_SRAM_IND_ACCESS_TIMEOUT;
            }
            curOffset += curRemainingBytes;
        }

        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);

        return ARIES_SUCCESS;
    }
}

/**
 * @brief Read one byte of data from specified address from Main micro SRAM
 * over I2C. Returns a negative error code, else the number byte data read.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  address    16-bit address from which to read
 * @param[out] value      Pointer to single element array of read data
 *                        (one byte)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadByteDataMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                                  uint32_t address,
                                                  uint8_t* value)
{
    AriesErrorType rc;
#ifdef ARIES_MPW
    rc = ariesReadBlockDataMainMicroIndirectMPW(i2cDriver, 0xe00, address, 1,
                                                value);
#else
    rc = ariesReadBlockDataMainMicroIndirectA0(i2cDriver, 0xd99, address, 1,
                                               value);
#endif
    return rc;
}

/**
 * @brief Read multiple (up to eight) data bytes from specified address from
 * Main micro SRAM over I2C. Returns a negative error code, else zero on
 * success
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      16-bit address from which to read
 * @param[in]  numBytes  Number of bytes to read (maximum 8 bytes)
 * @param[out] values       Pointer to array storing up to 8 bytes of data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesReadBlockDataMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                        uint32_t address, uint8_t numBytes,
                                        uint8_t* values)
{
    AriesErrorType rc;
#ifdef ARIES_MPW
    rc = ariesReadBlockDataMainMicroIndirectMPW(i2cDriver, 0xe00, address,
                                                numBytes, values);
#else
    rc = ariesReadBlockDataMainMicroIndirectA0(i2cDriver, 0xd99, address,
                                               numBytes, values);
#endif
    return rc;
}

/**
 * @brief Write a data byte at specified address to Main micro SRAM Aries
 * over I2C. Returns a negative error code, else zero on success.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  address    16-bit address to write
 * @param[in]  value      Byte data to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesWriteByteDataMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                        uint32_t address, uint8_t* value)
{
    AriesErrorType rc;
#ifdef ARIES_MPW
    rc = ariesWriteBlockDataMainMicroIndirectMPW(i2cDriver, 0xe00, address, 1,
                                                 value);
#else
    rc = ariesWriteBlockDataMainMicroIndirectA0(i2cDriver, 0xd99, address, 1,
                                                value);
#endif
    return rc;
}

/**
 * @brief Write multiple (up to eight) data bytes at specified address to
 * Main micro SRAM over I2C. Returns a negative error code, else zero on
 * success.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      16-bit address to write
 * @param[in]  numBytes  Number of bytes to write (maximum 16 bytes)
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesWriteBlockDataMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                         uint32_t address, uint8_t numBytes,
                                         uint8_t* values)
{
    AriesErrorType rc;
#ifdef ARIES_MPW
    rc = ariesWriteBlockDataMainMicroIndirectMPW(i2cDriver, 0xe00, address,
                                                 numBytes, values);
#else
    rc = ariesWriteBlockDataMainMicroIndirectA0(i2cDriver, 0xd99, address,
                                                numBytes, values);
#endif
    return rc;
}

/**
 * @brief Read multiple (up to eight) data bytes at specified address from
 * Path micro SRAM over I2C. Returns a negative error code, else the number
 * of bytes read.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  pathID       Path micro ID (e.g. 0, 1, ..., 15)
 * @param[in]  address      16-bit address from which to read
 * @param[in]  numBytes  Number of bytes to read (maximum 16 bytes)
 * @param[out] values       Pointer to array storing up to 16 bytes of data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesReadBlockDataPathMicroIndirect(AriesI2CDriverType* i2cDriver,
                                        uint8_t pathID, uint32_t address,
                                        uint8_t numBytes, uint8_t* values)
{
    if (pathID > 15)
    {
        return ARIES_INVALID_ARGUMENT;
    }

    AriesErrorType rc;
    uint32_t microIndStructOffset = 0x4200 + (pathID * ARIES_PATH_WRP_STRIDE);

    rc = ariesReadBlockDataMainMicroIndirectMPW(i2cDriver, microIndStructOffset,
                                                address, numBytes, values);

    return rc;
}

/**
 * @brief Read a data byte at specified address from Path micro SRAM over I2C.
 * Returns a negative error code, else zero on success.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  pathID     Path micro ID (e.g. 0, 1, ..., 15)
 * @param[in]  address    16-bit address from which to read
 * @param[out] value      Pointer to array of one byte of read data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadByteDataPathMicroIndirect(AriesI2CDriverType* i2cDriver,
                                                  uint8_t pathID,
                                                  uint32_t address,
                                                  uint8_t* value)
{
    AriesErrorType rc;
    rc = ariesReadBlockDataPathMicroIndirect(i2cDriver, pathID, address, 1,
                                             value);
    return rc;
}

/**
 * @brief Write multiple (up to eight) data bytes at specified address to
 * Path micro SRAM over I2C. Returns a negative error code, else zero on
 * success.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  pathID       Path micro ID (e.g. 0, 1, ..., 15)
 * @param[in]  address      16-bit address to write
 * @param[in]  numBytes  Number of bytes to write (maximum 16 bytes)
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesWriteBlockDataPathMicroIndirect(AriesI2CDriverType* i2cDriver,
                                         uint8_t pathID, uint32_t address,
                                         uint8_t numBytes, uint8_t* values)
{
    if (pathID > 15)
    {
        return ARIES_INVALID_ARGUMENT;
    }

    AriesErrorType rc;
    uint32_t microIndStructOffset = 0x4200 + (pathID * ARIES_PATH_WRP_STRIDE);

    rc = ariesWriteBlockDataMainMicroIndirectMPW(
        i2cDriver, microIndStructOffset, address, numBytes, values);

    return rc;
}

/**
 * @brief Write one byte of data byte at specified address to Path micro SRAM
 * Aries over I2C. Returns a negative error code, else zero on success.
 *
 * @param[in]  i2cDriver  I2C driver responsible for the transaction(s)
 * @param[in]  pathID     Path micro ID (e.g. 0, 1, ..., 15)
 * @param[in]  address    16-bit address to write
 * @param[in]  value      Byte data to write
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesWriteByteDataPathMicroIndirect(AriesI2CDriverType* i2cDriver,
                                        uint8_t pathID, uint32_t address,
                                        uint8_t* value)
{
    AriesErrorType rc;
    rc = ariesWriteBlockDataPathMicroIndirect(i2cDriver, pathID, address, 1,
                                              value);
    return rc;
}

/**
 * @brief Read 2 bytes of data from PMA register over I2C
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0) or A (1)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  address      16-bit address from which to read
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadWordPmaIndirect(AriesI2CDriverType* i2cDriver, int side,
                                        int quadSlice, uint16_t address,
                                        uint8_t* values)
{
    uint8_t cmd;
    uint8_t addr15To8;
    uint8_t addr7To0;
    uint8_t dataByte[1] = {0};
    AriesErrorType rc;
    AriesErrorType lc;
    int regAddr;

    // Set value for command register based on PMA side
    // A = 1, B = 0
    cmd = 0;
    if (side == 0) // B
    {
        cmd |= (0x1 << 1);
    }
    else if (side == 1) // A
    {
        cmd |= (0x2 << 1);
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Split address into 2 bytes
    addr15To8 = (address >> 8) & 0xff;
    addr7To0 = address & 0xff;

    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // Write Cmd reg
    dataByte[0] = cmd;
    regAddr = ARIES_PMA_QS0_CMD_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write upper bytes of address
    dataByte[0] = addr15To8;
    regAddr = ARIES_PMA_QS0_ADDR_1_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write lower bytes of address
    dataByte[0] = addr7To0;
    regAddr = ARIES_PMA_QS0_ADDR_0_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    usleep(ARIES_PMA_REG_ACCESS_TIME_US);

    // Read data (lower and upper bits)
    // Lower bits at data0 and upper bits at data1
    regAddr = ARIES_PMA_QS0_DATA_0_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesReadByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    values[0] = dataByte[0];

    regAddr = ARIES_PMA_QS0_DATA_1_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesReadByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    values[1] = dataByte[0];

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

/**
 * @brief Write 2 bytes of data from PMA register over I2C
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1), or broadcast to both (2)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  address      16-bit address to write
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteWordPmaIndirect(AriesI2CDriverType* i2cDriver,
                                         int side, int quadSlice,
                                         uint16_t address, uint8_t* values)
{
    uint8_t cmd;
    uint8_t addr15To8;
    uint8_t addr7To0;
    uint8_t dataByte[1] = {0};
    AriesErrorType rc;
    AriesErrorType lc;
    int regAddr;

    // Set value for command register based on PMA side
    // A = 1, B = 0, Both = 2
    cmd = 1;
    if (side == 0)
    {
        cmd |= (0x1 << 1);
    }
    else if (side == 1)
    {
        cmd |= (0x2 << 1);
    }
    else if (side == 2)
    {
        cmd |= (0x3 << 1);
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }

    // Split address into 2 bytes
    addr15To8 = (address >> 8) & 0xff;
    addr7To0 = address & 0xff;

    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // Write command reg
    dataByte[0] = cmd;
    regAddr = ARIES_PMA_QS0_CMD_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write data word, LSByte (data0) and MSByte (data1)
    dataByte[0] = values[0];
    regAddr = ARIES_PMA_QS0_DATA_0_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    dataByte[0] = values[1];
    regAddr = ARIES_PMA_QS0_DATA_1_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write address (upper bits)
    dataByte[0] = addr15To8;
    regAddr = ARIES_PMA_QS0_ADDR_1_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write address (lower bits)
    dataByte[0] = addr7To0;
    regAddr = ARIES_PMA_QS0_ADDR_0_ADDRESS + (quadSlice * ARIES_QS_STRIDE);
    rc = ariesWriteByteData(i2cDriver, regAddr, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    usleep(ARIES_PMA_REG_ACCESS_TIME_US);

    return ARIES_SUCCESS;
}

/**
 * @brief Read 2 bytes of data from PMA lane register over I2C
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0) or A (1)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  lane         PMA lane number
 * @param[in]  regOffset    16-bit ref offset from which to read
 * @param[in]  values       Byte array to store read data
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadWordPmaLaneIndirect(AriesI2CDriverType* i2cDriver,
                                            int side, int quadSlice, int lane,
                                            uint16_t regOffset, uint8_t* values)
{
    AriesErrorType rc;
    uint16_t address;
    if (lane < 4)
    {
        // 0x200 is the lane offset in a PMA
        address = regOffset + (lane * ARIES_PMA_LANE_STRIDE);
    }
    else
    {
        // This is not a lane type read
        // Treat this as a regular Pma read
        address = regOffset;
    }

    rc = ariesReadWordPmaIndirect(i2cDriver, side, quadSlice, address, values);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Write 2 bytes of data to PMA lane register over I2C
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1), or broadcast to both (2)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  lane         PMA lane number
 * @param[in]  regOffset    16-bit ref offset from which to read
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteWordPmaLaneIndirect(AriesI2CDriverType* i2cDriver,
                                             int side, int quadSlice, int lane,
                                             uint16_t regOffset,
                                             uint8_t* values)
{
    AriesErrorType rc;
    uint16_t address;
    if (lane < 4)
    {
        // 0x200 is the lane offset in a PMA
        address = regOffset + (lane * ARIES_PMA_LANE_STRIDE);
    }
    else
    {
        // This is not a lane type read
        // Treat this as a regular Pma read
        address = regOffset;
    }

    rc = ariesWriteWordPmaIndirect(i2cDriver, side, quadSlice, address, values);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Read 2 bytes of data from PMA register over I2C using the
 * 'main-micro-assisted' indirect method. This method is necessary during
 * mission-mode (non-PRBS-test-mode) operation.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0) or A (1)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  pmaAddr      16-bit PMA reg offset from which to read
 * @param[in,out]  data       Byte array which will be contain data read
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadWordPmaMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                                 int side, int quadSlice,
                                                 uint16_t pmaAddr,
                                                 uint8_t* data)
{
    AriesErrorType rc;
    AriesErrorType lc;

    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // Write address (3 bytes)
    uint8_t addr[3];
    uint32_t address = ((uint32_t)(quadSlice * 4) << 20) | (uint32_t)pmaAddr;
    addr[0] = address & 0xff;
    addr[1] = (address >> 8) & 0xff;
    addr[2] = (address >> 16) & 0xff;
    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_REG_ADDR_OFFSET, 3,
                             addr);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Set command
    uint8_t dataByte[1] = {0};
    if (side == 0)
    {
        dataByte[0] = ARIES_MM_RD_PID_IND_PMA0;
    }
    else
    {
        dataByte[0] = ARIES_MM_RD_PID_IND_PMA1;
    }

    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET, 1,
                             dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Check access status
    int count = 0;
    int status = 1;
    while ((status != 0) && (count < 100))
    {
        rc = ariesReadBlockData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET, 1,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }
        status = dataByte[0];

        usleep(ARIES_MM_STATUS_TIME);
        count += 1;
    }

    if (status != 0)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return ARIES_PMA_MM_ACCESS_FAILURE;
    }

    // Read 2 bytes of data
    rc = ariesReadBlockData(i2cDriver, ARIES_MM_ASSIST_DATA_OFFSET, 1,
                            dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    data[0] = dataByte[0];
    rc = ariesReadBlockData(i2cDriver, ARIES_MM_ASSIST_STS_OFFSET, 1, dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    data[1] = dataByte[0];

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

/**
 * @brief Write 2 bytes of data to PMA register over I2C using the
 * 'main-micro-assisted' indirect method. This method is necessary during
 * mission-mode (non-PRBS-test-mode) operation.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1), or broadcast to both (2)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  pmaAddr      16-bit PMA reg offset from which to read
 * @param[in,out]  data       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteWordPmaMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                                  int side, int quadSlice,
                                                  uint16_t pmaAddr,
                                                  uint8_t* data)
{
    AriesErrorType rc;
    AriesErrorType lc;

    uint8_t dataByte[1] = {0};

    lc = ariesLock(i2cDriver);
    CHECK_SUCCESS(lc);

    // Write address (3 bytes)
    uint8_t addr[3];
    uint32_t address = ((uint32_t)(quadSlice * 4) << 20) | (uint32_t)pmaAddr;
    addr[0] = address & 0xff;
    addr[1] = (address >> 8) & 0xff;
    addr[2] = (address >> 16) & 0xff;
    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_REG_ADDR_OFFSET, 3,
                             addr);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Write 2 bytes of data
    dataByte[0] = data[0];
    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_DATA_OFFSET, 1,
                             dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }
    dataByte[0] = data[1];
    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_STS_OFFSET, 1,
                             dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Set command
    if (side == 0)
    {
        dataByte[0] = ARIES_MM_WR_PID_IND_PMA0;
    }
    else if (side == 1)
    {
        dataByte[0] = ARIES_MM_WR_PID_IND_PMA1;
    }
    else
    {
        dataByte[0] = ARIES_MM_WR_PID_IND_PMAX;
    }

    rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET, 1,
                             dataByte);
    if (rc != ARIES_SUCCESS)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return rc;
    }

    // Check access status
    int count = 0;
    int status = 1;
    while ((status != 0) && (count < 100))
    {
        rc = ariesReadBlockData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET, 1,
                                dataByte);
        CHECK_SUCCESS(rc);
        status = dataByte[0];

        usleep(ARIES_MM_STATUS_TIME);
        count += 1;
    }

    if (status != 0)
    {
        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
        return ARIES_PMA_MM_ACCESS_FAILURE;
    }

    lc = ariesUnlock(i2cDriver);
    CHECK_SUCCESS(lc);

    return ARIES_SUCCESS;
}

/**
 * @brief Read 2 bytes of data from PMA lane register over I2C using the
 * 'main-micro-assisted' indirect method. This method is necessary during
 * mission-mode (non-PRBS-test-mode) operation.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0) or A (1)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  lane         PMA lane number
 * @param[in]  regOffset      16-bit PMA reg offset from which to read
 * @param[in,out]  values       Byte array which will be contain data read
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesReadWordPmaLaneMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                          int side, int quadSlice, int lane,
                                          uint16_t regOffset, uint8_t* values)
{
    AriesErrorType rc;
    uint16_t address;
    if (lane < 4)
    {
        // 0x200 is the lane offset in a PMA
        address = regOffset + (lane * ARIES_PMA_LANE_STRIDE);
    }
    else
    {
        // This is not a lane type read
        // Treat this as a regular Pma read
        address = regOffset;
    }

    rc = ariesReadWordPmaMainMicroIndirect(i2cDriver, side, quadSlice, address,
                                           values);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Write 2 bytes of data to PMA lane register over I2C using the
 * 'main-micro-assisted' indirect method. This method is necessary during
 * mission-mode (non-PRBS-test-mode) operation.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1), or broadcast to both (2)
 * @param[in]  quadSlice    PMA num: 0, 1, 2, or 3
 * @param[in]  lane         PMA lane number
 * @param[in]  regOffset    16-bit ref offset from which to read
 * @param[in]  values       Byte array which will be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType
    ariesWriteWordPmaLaneMainMicroIndirect(AriesI2CDriverType* i2cDriver,
                                           int side, int quadSlice, int lane,
                                           uint16_t regOffset, uint8_t* values)
{
    AriesErrorType rc;
    uint16_t address;
    if (lane < 4)
    {
        // 0x200 is the lane offset in a PMA
        address = regOffset + (lane * ARIES_PMA_LANE_STRIDE);
    }
    else
    {
        // This is not a lane type read
        // Treat this as a regular Pma read
        address = regOffset;
    }

    rc = ariesWriteWordPmaMainMicroIndirect(i2cDriver, side, quadSlice, address,
                                            values);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Read Modify Write 2 bytes of data to PMA lane register over I2C using
 * the 'main-micro-assisted' indirect method. This method is necessary during
 * mission-mode (non-PRBS-test-mode) operation.
 *
 * @param[in] i2cDriver     I2C driver responsible for the transaction(s)
 * @param[in] side          PMA Side B (0), A (1), or broadcast to both (2)
 * @param[in] quadSlice     PMA num: 0, 1, 2, or 3
 * @param[in] quadSliceLane PMA lane number
 * @param[in] pmaAddr       Address you wish to write to
 * @param[in] offset        Offset from the start of the word you want to write
 * in
 * @param[in] value         The value to write
 * @param[in] width         The width of the field you are writing to
 * @return      AriesErrorType - Aries error code
 */
AriesErrorType ariesReadWriteWordPmaLaneMainMicroIndirect(
    AriesI2CDriverType* i2cDriver, int side, int quadSlice, int quadSliceLane,
    uint16_t pmaAddr, int offset, uint16_t value, int width)
{
    AriesErrorType rc;
    uint8_t dataWord[2] = {0};

    rc = ariesReadWordPmaLaneMainMicroIndirect(
        i2cDriver, side, quadSlice, quadSliceLane, pmaAddr, dataWord);
    CHECK_SUCCESS(rc)

    uint16_t mask = 0xFFFF;
    mask <<= (width + offset);
    mask += (1 << offset) - 1;

    dataWord[0] &= mask;
    dataWord[1] &= mask >> 8;

    uint16_t orMask = 0;
    uint16_t valueMask = ((1 << width) - 1);
    orMask = (value & valueMask) << offset;

    if (value >= (1 << width))
    {
        ASTERA_WARN(
            "Trying to write a value greater than the width of the bitfield. Truncation occurred.");
    }

    dataWord[0] |= orMask;
    dataWord[1] |= orMask >> 8;

    rc = ariesWriteWordPmaLaneMainMicroIndirect(
        i2cDriver, side, quadSlice, quadSliceLane, pmaAddr, dataWord);
    CHECK_SUCCESS(rc)

    return ARIES_SUCCESS;
}

/**
 * @brief Read N bytes of data from a Retimer (gbl, ln0, or ln1) CSR.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1)
 * @param[in]  lane         Absolute lane number (15:0)
 * @param[in]  baseAddr     16-bit base address
 * @param[in]  numBytes     Number of bytes to read
 * @param[in]  data         Byte array which will be contain data read
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadRetimerRegister(AriesI2CDriverType* i2cDriver, int side,
                                        int lane, uint16_t baseAddr,
                                        uint8_t numBytes, uint8_t* data)
{
    AriesErrorType rc;
    uint8_t ret_pth_ln = lane % 2;
    uint8_t pth_wrap;
    uint8_t qs = floor(lane / 4);
    uint32_t addr;
    if (side == 1) // Side "A" - Even paths
    {
        pth_wrap = (2 * floor((lane % 4) / 2));
    }
    else if (side == 0) // Side "B" - Odd paths
    {
        pth_wrap = (2 * floor((lane % 4) / 2)) + 1;
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }
    addr = baseAddr + ret_pth_ln * ARIES_PATH_LANE_STRIDE +
           pth_wrap * ARIES_PATH_WRP_STRIDE + qs * ARIES_QS_STRIDE;
    rc = ariesReadBlockData(i2cDriver, addr, numBytes, data);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Write N bytes of data to a Retimer (gbl, ln0, or ln1) CSR.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  side         PMA Side B (0), A (1)
 * @param[in]  lane         Absolute lane number (15:0)
 * @param[in]  baseAddr     16-bit base address
 * @param[in]  numBytes     Number of bytes to write
 * @param[in]  data         Byte array containing data to be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteRetimerRegister(AriesI2CDriverType* i2cDriver,
                                         int side, int lane, uint16_t baseAddr,
                                         uint8_t numBytes, uint8_t* data)
{
    AriesErrorType rc;
    uint8_t ret_pth_ln = lane % 2;
    uint8_t pth_wrap;
    uint8_t qs = floor(lane / 4);
    uint32_t addr;
    if (side == 1) // Side "A" - Even paths
    {
        pth_wrap = (2 * floor((lane % 4) / 2));
    }
    else if (side == 0) // Side "B" - Odd paths
    {
        pth_wrap = (2 * floor((lane % 4) / 2)) + 1;
    }
    else
    {
        return ARIES_INVALID_ARGUMENT;
    }
    addr = baseAddr + ret_pth_ln * ARIES_PATH_LANE_STRIDE +
           pth_wrap * ARIES_PATH_WRP_STRIDE + qs * ARIES_QS_STRIDE;
    rc = ariesWriteBlockData(i2cDriver, addr, numBytes, data);
    CHECK_SUCCESS(rc);
    return ARIES_SUCCESS;
}

/**
 * @brief Read the contents of an Aries wide register.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      16-bit base address
 * @param[in]  width        Size of wide register in bytes
 * @param[in]  values       Byte array containing data to be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesReadWideRegister(AriesI2CDriverType* i2cDriver,
                                     uint32_t address, uint8_t width,
                                     uint8_t* values)
{
    if (i2cDriver->mmWideRegisterValid)
    {
        AriesErrorType rc;
        AriesErrorType lc;
        uint8_t dataByte[1] = {0};
        uint8_t addr[3];

        lc = ariesLock(i2cDriver);
        CHECK_SUCCESS(lc);

        // Write address (3 bytes)
        addr[0] = address & 0xff;
        addr[1] = (address >> 8) & 0xff;
        addr[2] = (address >> 16) & 0x01;
        rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_SPARE_0_OFFSET, 3,
                                 addr);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Set command based on width
        if (width == 2)
        {
            dataByte[0] = ARIES_MM_RD_WIDE_REG_2B;
        }
        else if (width == 3)
        {
            dataByte[0] = ARIES_MM_RD_WIDE_REG_3B;
        }
        else if (width == 4)
        {
            dataByte[0] = ARIES_MM_RD_WIDE_REG_4B;
        }
        else if (width == 5)
        {
            dataByte[0] = ARIES_MM_RD_WIDE_REG_5B;
        }
        else
        {
            return ARIES_INVALID_ARGUMENT;
        }

        rc = ariesWriteByteData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Check access status
        int count = 0;
        int status = 0xff;
        while ((status != 0) && (count < 100))
        {
            rc = ariesReadByteData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET,
                                   dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            status = dataByte[0];

            usleep(ARIES_MM_STATUS_TIME);
            count += 1;
        }

        if (status != 0)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return ARIES_PMA_MM_ACCESS_FAILURE;
        }

        // Read N bytes of data based on width
        rc = ariesReadBlockData(i2cDriver, ARIES_MM_ASSIST_SPARE_3_OFFSET,
                                width, values);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
    }
    else
    {
        return ariesReadBlockData(i2cDriver, address, width, values);
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Write the contents of an Aries wide register.
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @param[in]  address      16-bit base address
 * @param[in]  width        Size of wide register in bytes
 * @param[in]  values       Byte array containing data to be written
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesWriteWideRegister(AriesI2CDriverType* i2cDriver,
                                      uint32_t address, uint8_t width,
                                      uint8_t* values)
{
    if (i2cDriver->mmWideRegisterValid)
    {
        AriesErrorType rc;
        AriesErrorType lc;
        uint8_t dataByte[1] = {0};
        uint8_t addr[3];

        lc = ariesLock(i2cDriver);
        CHECK_SUCCESS(lc);

        // Write address (3 bytes)
        addr[0] = address & 0xff;
        addr[1] = (address >> 8) & 0xff;
        addr[2] = (address >> 16) & 0x01;
        rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_SPARE_0_OFFSET, 3,
                                 addr);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Write N bytes of data based on width
        rc = ariesWriteBlockData(i2cDriver, ARIES_MM_ASSIST_SPARE_3_OFFSET,
                                 width, values);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Set command based on width
        if (width == 2)
        {
            dataByte[0] = ARIES_MM_WR_WIDE_REG_2B;
        }
        else if (width == 3)
        {
            dataByte[0] = ARIES_MM_WR_WIDE_REG_3B;
        }
        else if (width == 4)
        {
            dataByte[0] = ARIES_MM_WR_WIDE_REG_4B;
        }
        else if (width == 5)
        {
            dataByte[0] = ARIES_MM_WR_WIDE_REG_5B;
        }
        else
        {
            return ARIES_INVALID_ARGUMENT;
        }

        rc = ariesWriteByteData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET,
                                dataByte);
        if (rc != ARIES_SUCCESS)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return rc;
        }

        // Check access status
        int count = 0;
        int status = 0xff;
        while ((status != 0) && (count < 100))
        {
            rc = ariesReadByteData(i2cDriver, ARIES_MM_ASSIST_CMD_OFFSET,
                                   dataByte);
            if (rc != ARIES_SUCCESS)
            {
                lc = ariesUnlock(i2cDriver);
                CHECK_SUCCESS(lc);
                return rc;
            }
            status = dataByte[0];

            usleep(ARIES_MM_STATUS_TIME);
            count += 1;
        }

        if (status != 0)
        {
            lc = ariesUnlock(i2cDriver);
            CHECK_SUCCESS(lc);
            return ARIES_PMA_MM_ACCESS_FAILURE;
        }

        lc = ariesUnlock(i2cDriver);
        CHECK_SUCCESS(lc);
    }
    else
    {
        return ariesWriteBlockData(i2cDriver, address, width, values);
    }

    return ARIES_SUCCESS;
}

/**
 * @brief Set lock on bus (Aries transaction)
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesLock(AriesI2CDriverType* i2cDriver)
{
    AriesErrorType rc;

    if (i2cDriver->lock >= 1)
    {
        i2cDriver->lock++;
    	rc = ARIES_SUCCESS;
    }
    else
    {
        rc = asteraI2CBlock(i2cDriver->handle);
        i2cDriver->lock = 1;
    }

    return rc;
}

/**
 * @brief Unlock bus after lock (Aries transaction)
 *
 * @param[in]  i2cDriver    I2C driver responsible for the transaction(s)
 * @return     AriesErrorType - Aries error code
 */
AriesErrorType ariesUnlock(AriesI2CDriverType* i2cDriver)
{
    AriesErrorType rc;

    if (i2cDriver->lock > 1)
    {
        i2cDriver->lock--;
	rc = ARIES_SUCCESS;
    }
    else
    {
        rc = asteraI2CUnblock(i2cDriver->handle);
        i2cDriver->lock = 0;
    }

    return rc;
}

#ifdef __cplusplus
}
#endif
