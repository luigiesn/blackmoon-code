# Blackmoon Servo
Servo motor controller with analog feedback by potentiometer

This is the source-code from Blackmoon controller. It can be seen in: 
http://gotronics.com.br/products/blackmoon/

The board uses a PIC18F1330

## TODO list:

Low-level
- Check the bridge code (only can be made with the schematics)
- Make a 1ms tick resolution software timer (using timer 0)

High-level
- Implement all the data flow from uart to the ram and vice-versa when needed.
- Complete the missing LED modes, BLINK_SLOW and BLINK_FAST(2Hz and 8Hz) using TIMER driver
- All PID code
- Integrate all drivers to make a closed loop
