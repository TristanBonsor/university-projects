/*
 * InputManager.hpp
 * Singleton input management class.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

#include "Vector3d.hpp"

/*
 * Maintains program input state.
 */
class InputManager
{
public:

    /*
     * Keyboard key that clears all current control points.
     */
    static const char KEY_CLEAR_CONTROL_POINTS = 'c';

    /*
     * Keyboard key that toggles control point input mode on/off.
     */
    static const char KEY_TOGGLE_CONTROL_POINT_INPUT_MODE = 'p';

    /*
     * Keyboard key that toggles surface orientation mode on/off.
     */
    static const char KEY_TOGGLE_SURFACE_ORIENTATION_INPUT_MODE = 'o';

    /*
     * Keyboard key that toggles linear interpolation output mode on/off.
     */
    static const char KEY_LINEAR_RENDER_MODE = 'l';

    /*
     * Keyboard key that toggles bezier interpolation output mode on/off.
     */
    static const char KEY_BEZIER_RENDER_MODE = 'b';

    /*
     * Keyboard key that toggles surface of revolution output mode on/off.
     */
    static const char KEY_RENDER_SURFACE_OF_REVOLUTION = 's';

    /*
     * Registers necessary callback functions that process user input. This
     * must be called once and only once. Assumes glutInit() has already been
     * called.
     */
    static void init();

private:

    /*
     * Defines possible input modes handled by InputManager.
     */
    enum class InputMode
    {
        NONE, /* Mouse input will not be processed. */
        CONTROL_POINT, /* Mouse clicks place a control point in the XY
                        * plane. */
        SURFACE_ORIENTATION /* Mouse click and drag for arcball viewing. */
    };

    /*
     * Handles mouse clicks based on current input mode.
     */
    static void mouseButtonEventHandler(int button, int state, int x, int y);

    /*
     * Handles arcball viewing if in SURFACE_ORIENTATION mode.
     */
    static void mouseActiveMotionEventHandler(int x, int y);

    /*
     * Handles keyboard press events (toggling different input/output modes.
     */
    static void keyPressEventHandler(unsigned char key, int x, int y);

    /*
     * Handles mouse left button down input based on input mode.
     */
    static void onMouseLeftButtonDown(int x, int y);

    /*
     * Handles mouse left button up input based on input mode.
     */
    static void onMouseLeftButtonUp(int x, int y);

    /*
     * Utility method that toggles the specified input mode based on the
     * current input mode.
     */
    static void toggleInputMode(const InputMode& mode);

    /*
     * Converts the x coordinate of the mouse pointer from window coordinates to
     * XY plane coordinates.
     */
    static float convertMouseX(int x);

    /*
     * Converts the y coordinate of the mouse pointer from window coordinates to
     * XY plane coordinates.
     */
    static float convertMouseY(int y);

    /*
     * Maintains current input mode for lifetime of program.
     */
    static InputMode m_input_mode;

    /*
     * Tracks whether the arcball is currently pinched.
     */
    static bool m_is_arcball_pinched;

    /*
     * Tracks the first arcball position vector for determining angle of
     * rotation and axis of rotation.
     */
    static Vector3d m_arcball_position_vector1;

    /*
     * Tracks the second arcball position vector for determining angle of
     * rotation and axis of rotation.
     */
    static Vector3d m_arcball_position_vector2;
};

#endif /* INPUTMANAGER_HPP_ */
