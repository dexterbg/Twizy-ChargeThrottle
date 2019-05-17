# TwizyChargeThrottle

This is a very simple tool to limit the Twizy charge current if you don't have an OVMS.

It can limit the charge current to a fixed level (defined in the config file) or use a
coded rotary switch or simple DIP switch for dynamic level adjustment. It can signal
the charging state and active throttling by LEDs.

See [wiring scheme](extras/Wiring.pdf) for an example using a coded rotary switch.

The Arduino needs to be connected to the Twizy CAN bus during the charge process to be able
to actively overwrite the charge current control frames.

Note: using an interrupt capable CAN shield is highly recommended. No attempt at
reducing the Arduino current consumption is made, include a power switch or fork and
extend the code for sleep mode (and send me a pull request).


## Info

Author: Michael Balzer <dexter@dexters-web.de>

Libraries used:
  - MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib

This is free software under GNU Lesser General Public License (LGPL)
https://www.gnu.org/licenses/lgpl.html
