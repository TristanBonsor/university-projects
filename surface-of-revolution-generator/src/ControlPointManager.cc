/*
 * ControlPointManager.cc
 * ControlPointManager implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "ControlPointManager.hpp"

/*
 * Static member initialization.
 */

ControlPointManager::ControlPoints ControlPointManager::m_control_points =
    ControlPointManager::ControlPoints();

/*
 * Static method definition.
 */

void ControlPointManager::addControlPoint(const Vector3d& control_point)
{
    m_control_points.push_back(control_point);
}

ControlPointManager::ControlPoints ControlPointManager::getControlPoints()
{
    return m_control_points;
}

void ControlPointManager::clearControlPoints()
{
    m_control_points.clear();
}
