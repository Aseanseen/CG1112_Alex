CG1112 Alex 2-02-2

===================================================================================================
Overview:
===================================================================================================
Control of Alex is done mainly through the Pi, which we will
control remotely via vnc server or SSH

Mapping is done via the Lidar using SLAM (Simultaneous
Localization And Mapping). The map is shown to us, and we
should be executing commands to the pi, which sends commands
to the Arduino, to control the motors.

Lidar <---- PI -----> Arduino -----> Motors

---------------------------------------------------------------------------------------------------
===================================================================================================
Status:
===================================================================================================
Build 29.3.19:
	PWM control completed, may require some calibration once all parts are fully assembled and
	full weight of bot can be tested.
	Serial comms between Pi and Arduino is done.
	Basic communication between both platforms are tested, additional non-essential parts of the 
	comms will be tested later on.

---------------------------------------------------------------------------------------------------
===================================================================================================
Contents:
===================================================================================================
Alex_Serial_Comms: test files for serial comms
Alex_test: test files for Alex
Builds: Previous stable builds
Dependencies: Relevant libs to be used
Mierda Dos: Latest stable build to use
Test Build: test build to use

---------------------------------------------------------------------------------------------------
===================================================================================================
Components:
===================================================================================================
Arduino:
	constants.h - constants header files
	motorA.h - analogue motor control (not used)
	motor.h - hall effect sensor control
	packet.h - packet handling control
	serial.h - serial comms control
	

Pi:
	Alex-pi - executable to interface with Arduino
	

---------------------------------------------------------------------------------------------------
===================================================================================================
Motors:
===================================================================================================
Motors controlled via the DRV-8833 chip

Pin connections:
VIN: Arduino 5v
GND: Arduino GND

AIN1: PIN6 //reverse direction
AIN2: PIN5 //forward direction
BIN1: PIN9 //reverse direction
BIN2: PIN10 //forward direction

BOUT1: Motor 2 +ve / Red wire from RIGHT motor.
BOUT2: Motor 2 –ve / Black wire from RIGHT motor.
AOUT1: Motor 1 +ve / Red wire from LEFT motor.
AOUT2: Motor 1 –ve / 	Black wire from LEFT MOTOR

Hall Effect Sensors:
White - PIN2/3 for left / right motor
Red - Arduino 5v pin
Black - Arduino GND pin

===================================================================================================
Accelerometer:
===================================================================================================
Status: Probably not getting used for the time being
Lib: https://github.com/tockn/MPU6050_tockn
	- reset x/y/z dist travelled before moveoff
	- calc dist using .getAcc(), and add to a static global 
	variable
	- if y accel goes too far, call appropriate turn 
	commands to veer (will need to calibrate later on)

===================================================================================================
Hall Effect Sensors / Encoders:
===================================================================================================
Current Status: Working

Issues:
Might need to calibrate properly
Note: motor power is always affected by battery power
 
===================================================================================================
To Do:
===================================================================================================

===================================================================================================
Project Specifications
===================================================================================================
Minimally, Alex must be able to carry out the following commands:
a. Go straight (you can define how far / how long, speed control etc). - done
b. Turn left / right (you can define the turning angle or the compass direction). - done
c. Identify object** (see more in “additional functionality” section below).
You can implement additional commands as you see fit.

During the evaluation, you have to manually take note of the environment mapped out by Alex. This 
"map" will be submitted at the end of your evaluation. (physical map) Evaluation stops as soon as 
Alex explored and mapped the entire arena OR the time limit is up. Exact time limit will be 
announced nearer to the final evaluation.

Additional task:
There are 2‐3 regular shaped (e.g. cube, cylinder), either red or green, objects scattered 
throughout the rooms. These objects are at least astall as the outer wall (i.e. ~18cm). Alex is 
supposed to figure out the colour of these objects during the main evaluation, i.e. the operator 
should send an "identify object" command when such objects are detected during navigation.

Power consumption - 5% (low priority)

-----------------------------------------------------------
Stuff to refer to:
-----------------------------------------------------------

Accelerometers and interrupts
https://www.youtube.com/watch?v=EE-tB8nR9EQ
MPU-6050 Libs
https://github.com/jarzebski/Arduino-MPU6050
https://github.com/tockn/MPU6050_tockn/blob/master/examples/GetAllData/GetAllData.ino //use this

Handshake Code:
http://robot-resource.blogspot.com/2018/01/handshake-code-processing-and-arduino.html

GNUPLOT:
http://www.gnuplot.info/

State of the Art thing:
http://www.jst.go.jp/EN/research/index.html