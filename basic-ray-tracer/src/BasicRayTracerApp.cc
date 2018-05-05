/*
 * BasicRayTracerApp.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <GL/glut.h>

#include "BasicRayTracerApp.hpp"

#include "InputManager.hpp"
#include "OutputManager.hpp"
#include "Scene.hpp"

void BasicRayTracerApp::run()
{
    glutMainLoop();
}

void BasicRayTracerApp::init(int argc, char** argv)
{
    glutInit(&argc, argv);

    OutputManager::init();
    InputManager::init();
    Scene::init();
}
