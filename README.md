# robottino
## A tracked vehicle based on:
	- Raspberry pi2
	- MakeBlock starter kit (w/o electronic)
	- Pololu Dual MC33926 Motor Driver for Raspberry Pi
	- three HCSR04 ultrasound proximity sensors
	- 2x16 lines LCD display based on HD44780 controller 
----
### Pathfinder ###
pathfinder/ is the implementation of an autonomous vehicle using three proximity sensors and a camera for object recognition.

**TODO:**

	- totally revrite Pathfinder class in pure C++14 (without C)
	- add path finding logic
	- add object recognition and following using OpenCV library
----
### RC Vehicle ###
remote/ and Vehicle/ are a sort di RC (Bluetooth) vehicle with remote camera implemented using bash scripting and the gpio commands from the wiringPi library (from http://wiringpi.com/) 

