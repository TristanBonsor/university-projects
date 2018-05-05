/*
 * \file Main.cc
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#include "Controller.hpp"

int main(int argc, char** argv)
{
    Controller::init(argc, argv);
    Controller::run();
    return 0;
}
