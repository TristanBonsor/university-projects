/*
 * ControlPointManager.hpp
 * Singleton control point management class.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef CONTROLPOINTMANAGER_HPP_
#define CONTROLPOINTMANAGER_HPP_

#include <vector>

#include "Vector3d.hpp"

class ControlPointManager
{
public:

    /*
     * Control point container type - a control point is a position vector in
     * R3.
     */
    typedef std::vector<Vector3d> ControlPoints;

    /*
     * Appends a control point to the control point list.
     */
    static void addControlPoint(const Vector3d& control_point);

    /*
     * Returns a container containing copies of all current control points.
     */
    static ControlPoints getControlPoints();

    /*
     * Clears all control points from the control point list.
     */
    static void clearControlPoints();

private:

    /*
     * Contains controls points in the order that they are added, until cleared.
     */
    static ControlPoints m_control_points;
};

#endif /* CONTROLPOINTMANAGER_HPP_ */
