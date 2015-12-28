# Blackmoon Servo
Servo motor controller with analog feedback by potentiometer

This is the source-code from Blackmoon controller. It can be seen in: 
http://gotronics.com.br/products/blackmoon/

The board uses a PIC18F1330

## TODO list:

Low-level
- Implement a flag on ADC to sinalizes conversion end. This flag must be set by ADC interrupt.
- Check the bridge code (only can be made with the schematics)
- Check the eeprom code and implement write with integrity check(write using interrupts to make it faster)
- Make a 1ms tick resolution software timer (using timer 0)

High-level
- Implement all the data flow among uart, eeprom and ram. Store when parameter is changed a load it back to ram
- Complete the missing LED modes, BLINK_SLOW and BLINK_FAST(2Hz and 8Hz) using TIMER driver
- Make the eeprom memory map to save all the parameters
- All PID code
- Integrate all drivers to make a closed loop
