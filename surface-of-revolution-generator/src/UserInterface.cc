/*!
 * \file UserInterface.cc
 * UserInterface implementation.
 *
 * \copyright Copyright (C) 2014 Tristan W. Bonsor
 */

#include <GL/glut.h>

#include "UserInterface.hpp"
#include "WindowManager.hpp"
#include "OutputManager.hpp"
#include "InputManager.hpp"

void UserInterface::init(int argc, char** argv)
{
    glutInit(&argc, argv);

    WindowManager::init();
    OutputManager::init();
    InputManager::init();
}

void UserInterface::run()
{
    glutMainLoop();
}
