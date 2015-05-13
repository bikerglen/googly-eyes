googly_eyes_arduino_basic.ino
-----------------------------
This Arduino sketch moves the googly eyes through a sequence of positions. It runs completely standalone and does not require a connection to a computer. It is useful for a static wall display of the googly eyes. It is the code used in my blog post at http://bikerglen.com/blog/bringing-the-robotic-googly-eyes-to-life/

googly_eyes_arduino_listen.ino
------------------------------
This Arduino sketch moves the googly eyes to a position received over the Arduino serial port. The serial port runs at 19200 baud. The format of the position command is 

mt <x> <y> <cr|lf>

where x is the position in steps (decimal) to move the left eyeball and y is the position in steps (decimal) to move the right eyeball. Negative numbers are CW from 0 (look left); positive numbers are CCW from 0 (look right).

mt -800 -800 would make both eyes look left.
mt 800 800 would make both eyes look right.
mt 800 -800 goes cross eyed.
