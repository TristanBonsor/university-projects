/*
 * UserInterface.hpp
 * Singleton user interface class that manages all program input and output.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef USERINTERFACE_HPP_
#define USERINTERFACE_HPP_

/*
 * Encapsulates all necessary input, output, and window management required by
 * the program.
 */
class UserInterface
{
public:

    /*
     * Initialize the user interface and all sub-modules.
     */
    static void init(int argc, char** argv);

    /*
     * Run the program/opengl main loop.
     */
    static void run();
};

#endif /* USERINTERFACE_HPP_ */
