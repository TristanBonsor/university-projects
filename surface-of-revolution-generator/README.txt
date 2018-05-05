README.txt
CSCI 405 Assignment 1

Tristan W. Bonsor

Table Of Contents
=================

1. Compilation & Execution
2. Keyboard Inputs
3. Input Mode Behaviour
4. Output Mode Behaviour
5. Camera Behaviour
6. Additional Notes

1. Compilation & Execution
==========================

Enter the main project directory (which contains Makefile) and type 'make'.

To run the program (after compilation) execute the command './ass1'.

2. Keyboard Inputs
==================

c - Clear any existing control points/surfaces previously created.

p - Toggle the mouse input mode into or out of control point editing mode.

o - Toggle the mouse input mode into or out of surface orientation mode.

l - Toggle control point rendering mode into linear interpretation mode.

b - Toggle control point rendering mode into bezier interpretation mode.

s - Toggle surface of revolution rendering mode.

3. Input Mode Behaviour
=======================

If in control point editing mode, left-mouse clicks will append additional
control points to the existing control point list.

If in in surface orientation mode, left-mouse clicks followed by mouse movement
will cause the object to rotate as defined by the arcball method of control.

4. Output Mode Behaviour
========================

When no ouput mode is set, the current control points will be displayed.

If linear interpretation output mode is on, linear lines will be displayed (as
defined by the current control points). Turning this mode off will result in
just the control points being displayed.

If bezier interpretation output mode is on, a bezier line will be displayed (as
defined by the current control points). Turning this mode off will result in
just the control points being displayed.

Note that only one of linear or bezier interpretation mode can be on; the user
may switch between modes.

If surface of revolution rendering mode is on, then the output of the previous
output mode (either control points, linear lines, or bezier line) will be
revolved around the default axis of revolution <0,1,0>, generating a surface of
revolution. Turning this mode off will result in just the control points being
displayed.

5. Camera Behaviour
===================

Whenever the input modes are toggled, the camera will fix itself at
<0,0,CAMERA_RADIUS> with up vector <0,1,0> and appropriate perspective for
viewing and inputing control points/lines on the XY plane.

6. Additional Notes
===================

This assignment has been implemented with C++ and some effort has been made in
modularizing different program components.

Although there is no input mode for changing the axis of revolution, the program
does contain the functionality for rotating an arbitrary point about and
arbitrary axis (change the value specified by DEFAULT_AXIS_OF_REVOLUTION in
Defines.hpp and recompile to see this in action).

Previous code branches included an additional window that displayed the
current program input and output modes. Unfortunately font and text support in
OpenGL seems to differ from system to system; due to these complications and
time constraints, the command window has been scrapped.

Certain efficiency improvements could be made, chiefly in reducing the number of
calulations performed in revolving a given output point, displaying control
points as they are added, and generating a bezier line, but at this project
scale it was judged to be unneccessary (but must be considered if project scope
were to widen).
