# Robottino
## A tracked vehicle based on:
	- Raspberry Pi2
	- MakeBlock Starter Kit (w/o electronic)
	- Pololu Dual MC33926 Motor Driver for Raspberry Pi
	- 3x HCSR04 ultrasound proximity sensors
	- 2x16 char. LCD display based on HD44780 controller 
----
### Pathfinder ###
pathfinder/ is the implementation of an autonomous vehicle using three proximity sensors and a camera for object recognition.

**TODO:**

	- totally rewrite Pathfinder class in pure C++14 (without C)
	- add path finding logic
	- add object recognition and following using OpenCV library
----
### RC Vehicle ###
remote/ and vehicle/ are a sort di RC (Bluetooth) vehicle with remote camera implemented using bash scripting and the gpio commands from the wiringPi library (from http://wiringpi.com/) 

