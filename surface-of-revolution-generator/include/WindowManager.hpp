/*
 * WindowManager.hpp
 * Singleton window management class that manages GLUT windows.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

/*
 * Window management class for tracking GLUT windows used by this program.
 */
class WindowManager
{
public:
    /*
     * Specific windows that are managed.
     */
    enum class Window
    {
        MAIN
    };

    /*
     * Initialize the main window and the command window. Assumes glutInit() has
     * been called. This should be called once only, before all other
     * WindowManager calls are to be made.
     */
    static void init();

    /*
     * Return a window's corresponding GLUT window id. Returns 0 if the window
     * does not exist.
     */
    static int getWindowId(const Window& window);

private:

    /*
     * Window reshape callback. Keeps window dimensions at initial values.
     */
    static void onReshape(int width, int height);

    /*
     * Called by WindowManager::init(). Creates a specific window (main or
     * command) with specified dimensions.
     */
    static void createWindow(const Window& window,
                             const float& x,
                             const float& y,
                             const float& width,
                             const float& height);

    /*
     * GLUT window id for the main window.
     */
    static int m_main_window_id;
};

#endif /* WINDOWMANAGER_HPP_ */
