/*
 * InputManager.cc
 * InputManager implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <iostream>

#include <GL/gl.h>
#include <GL/glut.h>

#include "InputManager.hpp"
#include "WindowManager.hpp"
#include "OutputManager.hpp"
#include "ControlPointManager.hpp"

/*
 * Static member initialization.
 */

InputManager::InputMode InputManager::m_input_mode =
    InputManager::InputMode::NONE;

bool InputManager::m_is_arcball_pinched = false;

Vector3d InputManager::m_arcball_position_vector1 = Vector3d();

Vector3d InputManager::m_arcball_position_vector2 = Vector3d();

/*
 * Static method definition.
 */

void InputManager::init()
{
    // Register input related event handlers.
    glutSetWindow(WindowManager::getWindowId(WindowManager::Window::MAIN));
    glutKeyboardFunc(keyPressEventHandler);
    glutMouseFunc(mouseButtonEventHandler);
    glutMotionFunc(mouseActiveMotionEventHandler);
}

void InputManager::mouseButtonEventHandler(int button, int state, int x, int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            switch(state)
            {
                case GLUT_DOWN:

                    onMouseLeftButtonDown(x, y);

                    break;

                case GLUT_UP:

                    onMouseLeftButtonUp(x, y);

                    break;

                default:

                    break;
            }

            break;

        default:

            break;
    }
}

void InputManager::mouseActiveMotionEventHandler(int x, int y)
{
    if(m_is_arcball_pinched)
    {
        // New arcball vector. Convert mouse coordinates from window space to XY
        // plane.
        m_arcball_position_vector2 = Vector3d(convertMouseX(x),
                                              convertMouseY(y),
                                              0.0f);

        if(m_arcball_position_vector1 != m_arcball_position_vector2)
        {
            // Rotate scene as defined by the angle between arcball vectors.
            OutputManager::arcballScene(m_arcball_position_vector1,
                                        m_arcball_position_vector2);

            // New arcball vector becomes old arcball vector - prepared for a
            // new arcball vector.
            m_arcball_position_vector1 = m_arcball_position_vector2;
        }
    }
}

void InputManager::keyPressEventHandler(unsigned char key, int x, int y)
{
    switch(key)
    {
        case KEY_CLEAR_CONTROL_POINTS:

            ControlPointManager::clearControlPoints();
            OutputManager::clearBezierLine();
            OutputManager::fixCamera(); // Snap view upon XY plane.

            break;

        case KEY_TOGGLE_CONTROL_POINT_INPUT_MODE:

            toggleInputMode(InputMode::CONTROL_POINT);
            OutputManager::fixCamera(); // Snap view upon XY plane.

            break;

        case KEY_TOGGLE_SURFACE_ORIENTATION_INPUT_MODE:

            toggleInputMode(InputMode::SURFACE_ORIENTATION);
            OutputManager::fixCamera(); // Snap view upon XY plane.

            break;

        case KEY_LINEAR_RENDER_MODE:

            OutputManager::toggleOutputMode(OutputManager::OutputMode::LINEAR);

            break;

        case KEY_BEZIER_RENDER_MODE:

            OutputManager::toggleOutputMode(OutputManager::OutputMode::BEZIER);

            break;

        case KEY_RENDER_SURFACE_OF_REVOLUTION:

            OutputManager::toggleOutputMode(OutputManager::OutputMode::SURFACE);

            break;

        default:

            return;

            break;
    }

    // Update display with glutDisplayFunc() callback.
    glutPostRedisplay();
}

void InputManager::onMouseLeftButtonDown(int x, int y)
{
    switch(m_input_mode)
    {
        // Begin arcball rotation of scene.
        case InputMode::SURFACE_ORIENTATION:

            m_is_arcball_pinched = true;

            // Initial arcball vector. Convert mouse coordinates from window
            // space to XY plane.
            m_arcball_position_vector1 = Vector3d(convertMouseX(x),
                                                  convertMouseY(y),
                                                  0.0f);

            break;

        default:

            break;
    }
}

void InputManager::onMouseLeftButtonUp(int x, int y)
{
    switch(m_input_mode)
    {
        case InputMode::CONTROL_POINT:

            ControlPointManager::addControlPoint(Vector3d(convertMouseX(x),
                                                          convertMouseY(y),
                                                          0.0f));

            // Update display with glutDisplayFunc() callback.
            glutPostRedisplay();

            break;

        case InputMode::SURFACE_ORIENTATION:

            m_is_arcball_pinched = false;

            break;

        default:

            break;
    }
}

void InputManager::toggleInputMode(const InputManager::InputMode& mode)
{
    if(mode == m_input_mode)
    {
        m_input_mode = InputMode::NONE;
    }
    else
    {
        m_input_mode = mode;
    }
}

float InputManager::convertMouseX(int x)
{
    glutSetWindow(WindowManager::getWindowId(WindowManager::Window::MAIN));

    return glutGet(GLUT_WINDOW_WIDTH) / -2.0f + x;
}

float InputManager::convertMouseY(int y)
{
    glutSetWindow(WindowManager::getWindowId(WindowManager::Window::MAIN));

    return glutGet(GLUT_WINDOW_HEIGHT) / 2.0f - y;
}
