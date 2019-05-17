/**
 * ==========================================================================
 * Twizy charge current throttle
 * ==========================================================================
 */
#ifndef _TwizyChargeThrottle_config_h
#define _TwizyChargeThrottle_config_h

// Charge current throttle levels:
//  7 = 35 A = 2,3 kW (full power, no throttling)
//  6 = 30 A = 2,1 kW
//  5 = 25 A = 1,7 kW
//  4 = 20 A = 1,4 kW
//  3 = 15 A = 1,0 kW
//  2 = 10 A = 0,7 kW
//  1 =  5 A = 0,4 kW
//  0 = no throttling

// If not using a power level input, define a fixed throttling level like this:
// #define FIXED_LEVEL               5
// If this is defined, the power level inputs will not be used.

// Power level input:
// Use an 8 position binary coded rotary switch or a 3 pin DIP switch (or similar),
// define the input ports used here (analog ports may be used as well):
#define IN_LEVEL_B1               3
#define IN_LEVEL_B2               4
#define IN_LEVEL_B4               5

// Status outputs (e.g. LEDs):
#define OUT_CHARGING_PIN          8
#define OUT_THROTTLING_PIN        9

// Serial debug output level:
//  1 = show charge start/stop
//  2 = show current level changes
#define TWIZY_DEBUG               0

// Serial interface baud rate:
#define SERIAL_SPEED              115200

// Set your CAN MCP clock frequency here:
#define TWIZY_CAN_MCP_FREQ        MCP_16MHZ

// Set your CAN CS pin number here:
#define TWIZY_CAN_CS_PIN          10

// Set your CAN IRQ pin here (0 = no IRQ):
#define TWIZY_CAN_IRQ_PIN         2

#endif // _TwizyChargeThrottle_config_h

