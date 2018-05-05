/*
 * InputManager.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */
#include <iostream>
#include <GL/glut.h>

#include "Camera.hpp"
#include "Defines.hpp"
#include "InputManager.hpp"

void InputManager::init()
{
    glutKeyboardFunc(keyPressEventHandler);
}

void InputManager::keyPressEventHandler(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
            Camera::translate(Camera::getAt());
            break;
        case 'a':
            Camera::translate(Camera::getRight().getScaled(-1));
            break;
        case 's':
            Camera::translate(Camera::getAt().getScaled(-1));
            break;
        case 'd':
            Camera::translate(Camera::getRight());
            break;
        case 'i':
            Camera::pitch(CAMERA_PITCH_DOWN_ANGLE);
            break;
        case 'j':
            Camera::yaw(CAMERA_YAW_LEFT_ANGLE);
            break;
        case 'k':
            Camera::pitch(CAMERA_PITCH_UP_ANGLE);
            break;
        case 'l':
            Camera::yaw(CAMERA_YAW_RIGHT_ANGLE);
            break;
        default:
            return;
            break;
    }

    // Update display with glutDisplayFunc() callback.
    glutPostRedisplay();
}
