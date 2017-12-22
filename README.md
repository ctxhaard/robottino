# Robottino
## A tracked vehicle based on:
	- Raspberry Pi2
	- MakeBlock Starter Kit (w/o electronic)
	- Pololu Dual MC33926 Motor Driver for Raspberry Pi
	- 3x HCSR04 ultrasound proximity sensors
	- 2x16 char. LCD display based on HD44780 controller
	
The proximity sensors are managed by means of the **hcsr04 Linux driver** I developed myself (see my project "proximity").
The LCD display is managed by means of the **hd44780 Linux driver** I developed too (see my project "HD44780").
These Linux drivers are the most important part of these "study" projects I devloped in my free time.

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

