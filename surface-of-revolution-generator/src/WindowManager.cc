/*!
 * WindowManager.cc
 * WindowManager implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <GL/glut.h>

#include "Defines.hpp"
#include "WindowManager.hpp"

int WindowManager::m_main_window_id = 0;

void WindowManager::init()
{

    createWindow(Window::MAIN,
                 MAIN_WINDOW_X,
                 MAIN_WINDOW_Y,
                 MAIN_WINDOW_WIDTH,
                 MAIN_WINDOW_HEIGHT);
}

int WindowManager::getWindowId(const Window& window)
{
    int window_id = 0;

    switch(window)
    {
        case Window::MAIN:
            window_id = m_main_window_id;
            break;
        default:
            break;
    }

    return window_id;
}

void WindowManager::onReshape(int width, int height)
{

    // Keep window at fixed dimensions.

    int window_id = glutGetWindow();

    if(window_id == m_main_window_id)
    {
        if(width != MAIN_WINDOW_WIDTH || height != MAIN_WINDOW_HEIGHT)
        {
            glutReshapeWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
        }
    }
}

void WindowManager::createWindow(const Window& window,
                                 const float& x,
                                 const float& y,
                                 const float& width,
                                 const float& height)
{
    switch(window)
    {
        case Window::MAIN:

            if(!m_main_window_id)
            {
                glutInitDisplayMode(GLUT_DEPTH
                                    | GLUT_DOUBLE
                                    | GLUT_RGBA
                                    | GLUT_MULTISAMPLE);

                glutInitWindowPosition(x, y);
                glutInitWindowSize(width, height);

                m_main_window_id = glutCreateWindow(MAIN_WINDOW_TITLE);
            }

            break;

        default:

            return;

            break;
    }

    glutReshapeFunc(0);
    glutReshapeFunc(onReshape);
}
