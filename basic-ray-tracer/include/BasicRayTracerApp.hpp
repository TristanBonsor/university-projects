/*
 * \file BasicRayTracerApp.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef BASICRAYTRACERAPP_HPP_
#define BASICRAYTRACERAPP_HPP_

#include "OpenGLApp.hpp"

/**
 * \class BasicRayTracerApp BasicRayTracerApp.hpp "include/BasicRayTracerApp.hpp"
 * \brief Encapsulates a basic ray tracing application.
 *
 * This class provides a simple interface for running a basic ray tracing
 * application.
 */
class BasicRayTracerApp: public OpenGLApp
{
public:

    /**
     * Explicit constructor
     * @param argc Command line argument count.
     * @param argv Command line arguments.
     */
    BasicRayTracerApp(int argc, char** argv)
    {
        init(argc, argv);
    }

    virtual ~BasicRayTracerApp()
    {
    }

    /**
     * Run the application.
     */
    void run();

protected:

    /**
     * Initialize the application.
     * @param argc Command line argument count.
     * @param argv Command line arguments.
     */
    void init(int argc, char** argv);
};

#endif /* BASICRAYTRACERAPP_HPP_ */
