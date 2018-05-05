/*
 * Main.cc
 * Main program execution entry point.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "UserInterface.hpp"

int main(int argc, char** argv)
{
    UserInterface::init(argc, argv);
    UserInterface::run();

    return 0;
}
