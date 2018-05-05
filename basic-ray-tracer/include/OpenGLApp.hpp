/**
 * \file OpenGLApp.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef OPENGLAPP_HPP_
#define OPENGLAPP_HPP_

/**
 * \class OpenGLApp OpenGLApp.hpp "include/OpenGLApp.hpp"
 * \brief A very simple abstract interface for an OpenGL application.
 *
 * Provides a simple abstract interface for OpenGL applications
 */
class OpenGLApp
{
public:

    virtual ~OpenGLApp()
    {
    }
    ;

    /**
     * Run the application.
     */
    virtual void run() = 0;

protected:

    /**
     * Initialize the application.
     * @param argc Command line argument count.
     * @param argv Command line arguments.
     */
    virtual void init(int argc, char** argv) = 0;
};

#endif /* OPENGLAPP_HPP_ */
