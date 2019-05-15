/**
 * ==========================================================================
 * Twizy charge current throttle
 * ==========================================================================
 */
#ifndef _TwizyChargeThrottle_config_h
#define _TwizyChargeThrottle_config_h

// Set the charge current throttle level here:
//  6 = 30 A = 2,1 kW
//  5 = 25 A = 1,7 kW
//  4 = 20 A = 1,4 kW
//  3 = 15 A = 1,0 kW
//  2 = 10 A = 0,7 kW
//  1 =  5 A = 0,4 kW
#define TWIZY_CHARGE_THROTTLE     5

// Serial debug output level:
//  1 = show charge start/stop
//  2 = show current level changes
#define TWIZY_DEBUG               2

// Serial interface baud rate:
#define SERIAL_SPEED              115200

// Set your CAN MCP clock frequency here:
#define TWIZY_CAN_MCP_FREQ        MCP_16MHZ

// Set your CAN CS pin number here:
#define TWIZY_CAN_CS_PIN          SS

// Set your CAN IRQ pin here (0 = no IRQ):
#define TWIZY_CAN_IRQ_PIN         2

#endif // _TwizyChargeThrottle_config_h

