/**
 * \file InputManager.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

/**
 * \class InputManager "InputManager.hpp" "include/InputManager.hpp"
 * \brief Input management.
 *
 * Provides all logic and interfaces for managing input.
 */
class InputManager
{
public:

    /**
     * Initializes the input manager. Assumes glutInit and OutputManager::init
     * have already been called.
     */
    static void init();

protected:

    /**
     * Provides all key press event handling and delegation.
     * @param key The ascii code of the key pressed.
     * @param x The x coordinate of the mouse when key was pressed.
     * @param y The y coordinate of the mouse when key was pressed.
     */
    static void keyPressEventHandler(unsigned char key, int x, int y);
};

#endif /* INPUTMANAGER_HPP_ */
