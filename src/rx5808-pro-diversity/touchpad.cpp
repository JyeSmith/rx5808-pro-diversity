#include <Arduino.h>
#include <avr/pgmspace.h>

#include "touchpad.h"
#include <Wire.h>

// Cirque's 7-bit I2C Slave Address
#define TOUCHPAD_SLAVE_ADDR  0x2A

// Masks for Cirque Register Access Protocol (RAP)
#define TOUCHPAD_WRITE_MASK  0x80
#define TOUCHPAD_READ_MASK   0xA0
        
namespace TouchPad {

    relData_t touchData;
      
    void setup() {

        Pinnacle_Init();

    }  
    
    void update() {

        if(isDataAvailable()) {
          
            Pinnacle_getRelative(&touchData);
            
//            Serial.print(touchData.buttonPrimary);
//            Serial.print('\t');
//            Serial.print(touchData.buttonSecondary);
//            Serial.print('\t');
//            Serial.print(touchData.buttonAuxiliary);
//            Serial.print('\t');  
//            Serial.print(touchData.xDelta);
//            Serial.print('\t');
//            Serial.print(touchData.yDelta);
//            Serial.print('\t');
//            Serial.print(touchData.xSign);
//            Serial.print('\t');
//            Serial.println(touchData.ySign);
          
        }
    }

    /*  Pinnacle-based TM0XX0XX Functions  */
    void Pinnacle_Init() {
      
      // Host clears SW_CC flag
      Pinnacle_ClearFlags();
    
      // Feed Enable
      RAP_Write(0x04, 0b00000001);
      
    }
    
    // Reads X, Y, and Scroll-Wheel deltas from Pinnacle, as well as button states
    // NOTE: this function should be called immediately after DR is asserted (HIGH)
    void Pinnacle_getRelative(relData_t * result) {
      
      uint8_t data[3] = { 0,0,0 };
    
      RAP_ReadBytes(0x12, data, 3);
    
      Pinnacle_ClearFlags();
    
      result->buttonPrimary = data[0] & 0b00000001;
      result->buttonSecondary = data[0] & 0b00000010;
      result->buttonAuxiliary = data[0] & 0b00000100;
      result->xDelta = (int8_t)data[1];
      result->yDelta = (int8_t)data[2];
      result->xSign = data[0] & 0b00010000;
      result->ySign = data[0] & 0b00100000;
    
    }
    
    // Clears Status1 register flags (SW_CC and SW_DR)
    void Pinnacle_ClearFlags() {
      
      RAP_Write(0x02, 0x00);
      
      delayMicroseconds(50);
      
    }
    
    /*  RAP Functions */
    // Reads <count> Pinnacle registers starting at <address>
    //void RAP_ReadBytes(byte address, byte * data, byte count)
    void RAP_ReadBytes(byte address, byte * data, uint8_t count) {
      
      byte cmdByte = TOUCHPAD_READ_MASK | address;   // Form the READ command byte
      byte i = 0;
    
      Wire.beginTransmission(TOUCHPAD_SLAVE_ADDR);   // Set up an I2C-write to the I2C slave (Pinnacle)
      Wire.write(cmdByte);                  // Signal a RAP-read operation starting at <address>
      Wire.endTransmission(true);           // I2C stop condition
    
      Wire.requestFrom((uint8_t)TOUCHPAD_SLAVE_ADDR, count);  // Read <count> bytes from I2C slave
      
      while(Wire.available())
      {
        data[i++] = Wire.read();
      }
      
    }
    
    // Writes single-byte <data> to <address>
    void RAP_Write(byte address, byte data) {
      
      byte cmdByte = TOUCHPAD_WRITE_MASK | address;  // Form the WRITE command byte
    
      Wire.beginTransmission(TOUCHPAD_SLAVE_ADDR);   // Set up an I2C-write to the I2C slave (Pinnacle)
      Wire.write(cmdByte);                   // Signal a RAP-write operation at <address>
      Wire.write(data);                      // Write <data> to I2C slave
      Wire.endTransmission(true);           // I2C stop condition
    }

    bool isDataAvailable() {
      
      byte data[1] = { 0 };
      RAP_ReadBytes(0x02, data, 1);
    
      return data[0] & 0b00000100;
      
    } 
    
}