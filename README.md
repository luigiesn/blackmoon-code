# Blackmoon Servo
Servo motor controller with analog feedback by potentiometer

This is the source-code from Blackmoon controller. It can be seen in: 
http://gotronics.com.br/products/blackmoon/

The board uses a PIC18F1330

## TODO list:

Low-level
- Finish the eeprom manager

High-level
- Implement all the data flow from uart to the ram and vice-versa when needed.
- All PID code
- Integrate all drivers to make a closed loop (app.c)