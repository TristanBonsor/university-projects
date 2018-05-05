BasicRayTracer
==============

Compilation:

When in root project directory type 'make'.

Controls:

wasd - move the camera in the forward/left/backward/right directions
       (respectively) for the given keyboard input.
       
ijkl - rotate the camera: pitch-down/yaw-left/pitch-up/yaw-right
       directions.
       
Note:

Class documentation can be found in doc/html/index.html. The core ray
tracer code may be found in OutputManager.cc.

This program does all rendering via ray tracing. No doubt the fly camera
may run sluggishly on the lab computers, so compilation optimization is
set to level 2.

I have made use of the color.h file from lab 5. No attempt has been made
to refactor the CRGB class to conform to coding standards. Otherwise,
all other files should be legit.
