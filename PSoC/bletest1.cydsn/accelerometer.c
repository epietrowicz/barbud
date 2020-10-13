/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include "accelerometer.h" // Interface for this file

// Standard Libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include <project.h>

#define ACCEL_ADDR 0x18

static bool accelerometerWriteRegister(uint8_t reg, uint8_t value);
static uint8_t accelerometerReadRegister(uint8_t reg);

void accelerometerInit(void){
    accelerometerWriteRegister(0x20, 0b01111111); //write CTRL_REG1
    accelerometerWriteRegister(0x21, 0b00000100); //write CTRL_REG2
    accelerometerWriteRegister(0x22, 0b10000000); //write CTRL_REG3
    accelerometerWriteRegister(0x23, 0b00010000); //write CTRL_REG4
    accelerometerWriteRegister(0x24, 0b00000000); //write CTRL_REG5
    accelerometerWriteRegister(0x25, 0b00000000); //write CTRL_REG6
    accelerometerWriteRegister(0x26, 0b00000000); //write REFERENCE
    //single click configuration
    //accelerometerWriteRegister(0x38, 0b00010101); //write CLICK_CFG
    //double click configuration
    accelerometerWriteRegister(0x38, 0b00101010); //write CLICK_CFG
    
    // play with this value!
    accelerometerWriteRegister(0x3A, 0b00100000); //write CLICK_THS
    //time limit of 0x33 is sensitive and seemingly most reliable
    accelerometerWriteRegister(0x3B, 0x33); //write TIME_LIMIT
    
    //latency of 0x15 seems to be really good for double clicking
    accelerometerWriteRegister(0x3C, 0x15); //write TIME_LATENCY
    
    //window of 42 seems to be good for double clicking, 0xff works well too
    accelerometerWriteRegister(0x3D, 0x42); //write TIME_WINDOW
}


static uint8_t accelerometerReadRegister(uint8_t reg){
    uint8_t wrbuf[1];
    uint8_t rdbuf[1];
    wrbuf[0] = reg;
    I2C_Start();
    I2C_I2CMasterClearStatus();
    
    I2C_I2CMasterWriteBuf(ACCEL_ADDR, (uint8_t *)wrbuf, sizeof(wrbuf), I2C_I2C_MODE_NO_STOP);
    while(0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)){}
    
    I2C_I2CMasterReadBuf(ACCEL_ADDR, (uint8_t *)rdbuf, sizeof(rdbuf), I2C_I2C_MODE_REPEAT_START);
    while(0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT)){}
    I2C_Stop();
    return rdbuf[0];
}

static bool accelerometerWriteRegister(uint8_t reg, uint8_t value){
    bool success = false;
    uint8 wrbuf[2];
    wrbuf[0] = reg;
    wrbuf[1] = value;
    
    I2C_Start();
    
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(ACCEL_ADDR, wrbuf, sizeof(wrbuf), I2C_I2C_MODE_COMPLETE_XFER);
    
    while(0u == (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT)){}
    
    if(0u == (I2C_I2C_MSTAT_ERR_XFER & I2C_I2CMasterStatus())){
        if(I2C_I2CMasterGetWriteBufSize() == sizeof(wrbuf)){
            success = true;
        }
    } else {
        success = false;
    }
    
    I2C_I2CMasterClearStatus();
    I2C_Stop();
    return success;
}








