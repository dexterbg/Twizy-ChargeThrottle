/**
 * ==========================================================================
 * Twizy charge current throttle
 * ==========================================================================
 * 
 * Author: Michael Balzer <dexter@dexters-web.de>
 * 
 * Libraries used:
 *  - MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
 * 
 * License:
 *  This is free software under GNU Lesser General Public License (LGPL)
 *  https://www.gnu.org/licenses/lgpl.html
 *  
 */
#define TWIZY_CT_VERSION "V2.0 (2019-05-17)"

#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include "TwizyChargeThrottle_config.h"

#if TWIZY_DEBUG >= 1
  #define DEBUG1(cmd) cmd
#else
  #define DEBUG1(cmd)
#endif

// CAN interface:
MCP_CAN CAN(TWIZY_CAN_CS_PIN);

// CAN msg buffer:
unsigned long msgId;
byte msgLen;
byte msgBuf[8];

// Operation mode:
bool charging = false;

// Current level:
byte level = 0;

// Throttling level:
#ifdef FIXED_LEVEL
  const byte throttle = FIXED_LEVEL;
#else
  byte throttle = 0;
  #define READ_LEVEL() \
    ( digitalRead(IN_LEVEL_B1) \
    | digitalRead(IN_LEVEL_B2) << 1 \
    | digitalRead(IN_LEVEL_B4) << 2)
#endif


#if TWIZY_CAN_IRQ_PIN > 0
  volatile bool msgReceived = false;
  void canISR() {
    msgReceived = true;
  }
#endif


void setup() {
  
  Serial.begin(SERIAL_SPEED);
  Serial.println(F("TwizyChargeThrottle " TWIZY_CT_VERSION));
  
  // Init CAN interface:
  
  #if TWIZY_CAN_IRQ_PIN > 0
    pinMode(TWIZY_CAN_IRQ_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(TWIZY_CAN_IRQ_PIN), canISR, FALLING);
  #endif

  while (CAN.begin(MCP_STDEXT, CAN_500KBPS, TWIZY_CAN_MCP_FREQ) != CAN_OK) {
    Serial.println(F("Setup: waiting for CAN connection..."));
    delay(1000);
  }
  
  // Set ID filters:
  
  CAN.init_Mask(0, 0, 0x07FF0000);
  CAN.init_Filt(0, 0, 0x01550000); // BMS → BMS: current control
  CAN.init_Filt(1, 0, 0x05970000); // CHG → BMS: operation mode
  
  CAN.init_Mask(1, 0, 0x07FF0000);
  CAN.init_Filt(2, 0, 0x00000000);
  CAN.init_Filt(3, 0, 0x00000000);
  CAN.init_Filt(4, 0, 0x00000000);
  CAN.init_Filt(5, 0, 0x00000000);
  
  CAN.setMode(MCP_NORMAL);

  // Init I/O:

  #ifndef FIXED_LEVEL
    pinMode(IN_LEVEL_B1, INPUT);
    pinMode(IN_LEVEL_B2, INPUT);
    pinMode(IN_LEVEL_B4, INPUT);
    throttle = READ_LEVEL();
  #endif

  pinMode(OUT_CHARGING_PIN, OUTPUT);
  pinMode(OUT_THROTTLING_PIN, OUTPUT);
  digitalWrite(OUT_CHARGING_PIN, LOW);
  digitalWrite(OUT_THROTTLING_PIN, LOW);

  Serial.println(F("Setup done."));

  Serial.print(F("Throttling to current level: "));
  Serial.println((char)('0' + throttle));
}


void loop() {
  while (true) {

    #if TWIZY_CAN_IRQ_PIN > 0
      // Wait for interrupt signal:
      while (!msgReceived);
      msgReceived = false;
    #endif

    // Process CAN read buffer:
    while (CAN.readMsgBuf(&msgId, &msgLen, msgBuf) == CAN_OK)
    {
      if (msgId == 0x597)
      {
        // ID 0x597: operation mode
        
        bool _charging = ((msgBuf[1] & 0x60) == 0x20);
        
        if (_charging && !charging)
        {
          digitalWrite(OUT_CHARGING_PIN, HIGH);
          #ifndef FIXED_LEVEL
            throttle = READ_LEVEL();
          #endif
          DEBUG1(Serial.println(F("Charge START")));
        }
        else if (!_charging && charging)
        {
          digitalWrite(OUT_CHARGING_PIN, LOW);
          digitalWrite(OUT_THROTTLING_PIN, LOW);
          DEBUG1(Serial.println(F("Charge STOP")));
        }
        
        charging = _charging;
        level = 0;
      }
      else if (charging)
      {
        // ID 0x155: charge current control

        byte _level = msgBuf[0];
        
        if (_level == 0xFF)
          continue; // init phase, skip
        
        if (throttle != 0 && _level > throttle)
        {
          // overwrite:
          msgBuf[0] = throttle;
          CAN.sendMsgBuf(0x155, 0, 8, msgBuf);
          digitalWrite(OUT_THROTTLING_PIN, HIGH);
        }
        else
        {
          digitalWrite(OUT_THROTTLING_PIN, LOW);
        }

        #if TWIZY_DEBUG > 1
          if (_level != level)
          {
            Serial.print(F("Level "));
            if (_level > throttle) {
              Serial.print((char)('0' + _level));
              Serial.print('>');
              Serial.println((char)('0' + throttle));
            } else {
              Serial.println((char)('0' + _level));
            }
          }
        #endif
        
        level = _level;
      }
    }

    #ifndef FIXED_LEVEL
      throttle = READ_LEVEL();
    #endif

  } // while (true)
}

