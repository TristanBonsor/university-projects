/*
 * OutputManager.hpp
 * Singleton output management class.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef OUTPUTMANAGER_HPP_
#define OUTPUTMANAGER_HPP_

#include "ControlPointManager.hpp"
#include "Vector3d.hpp"

/*
 * Manages all program output. Encapsulates all drawing methods as well as
 * utility methods for performing rotations in R3.
 */
class OutputManager
{
public:

    /*
     * Defines possible output modes handled by OutputManager.
     */
    enum class OutputMode
    {
        NONE,   /* Display all control points, if they exist. */
        LINEAR, /* Display linear interpolation of control points. */
        BEZIER, /* Display bezier interpolation of control points. */
        SURFACE /* Display surface of revolution based on 1 of the above 3
                 * previous modes. */
    };

    /*
     * Registers display callback function and fixes camera position at position
     * vector <0,0,CAMERA_RADIUS> with perspective defined by window width,
     * window height, and CAMERA_RADIUS. Camera up vector is <0,1,0>. Assumes
     * glutInit() has already been called.
     */
    static void init();

    /*
     * Utility method that toggles the specified output mode based on the
     * current output mode.
     */
    static void toggleOutputMode(const OutputMode& mode);

    /*
     * Rotate the scene based upon arcball position vectors. Assumes vectors
     * lie within the XY plane (z=0). Axis of rotation vector is the normalized
     * result of (vector1 x vector2).
     */
    static void arcballScene(const Vector3d& xy_plane_vector1,
                             const Vector3d& xy_plane_vector2);

    /*
     * Fixes camera position at position vector <0,0,CAMERA_RADIUS> with
     * perspective defined by window width, window height, and CAMERA_RADIUS.
     * Camera up vector is <0,1,0>.
     */
    static void fixCamera();

    /*
     * Clears the currently calculated bezier line (if one exists).
     */
    static void clearBezierLine();

private:

    /*
     * Draws R3 axis where the x-axis is red, y-axis is green, and z-axis is
     * blue.
     */
    static void drawAxis();

    /*
     * Draw a point at the specified position vector in R3.
     */
    static void drawPoint3d(const Vector3d& p);

    /*
     * Draw a linear line with end points indicated by the specified position
     * vectors in R3.
     */
    static void drawLinearLine3d(const Vector3d& p1, const Vector3d& p2);

    /*
     * Draw all currently defined control points.
     */
    static void drawControlPoints();

    /*
     * Draw linear lines with line segments specified by currently defined
     * control points.
     */
    static void drawLinearLines();

    /*
     * Draw a bezier line specified by currently defined control points.
     */
    static void drawBezierLine();

    /*
     * Draw a surface of revolution based on the output of the previous output mode
     * (points, linear line, or bezier line). Axis of revolution defaults to
     * <0,1,0>.
     */
    static void drawSurfaceOfRevolution();

    /*
     * Calculate all pseudo-control points necessary for drawing a bezier line;
     * these pseudo-control points are to be linearly interpolated.
     */
    static void calculateBezierLine();

    /*
     * Transforms a given vector in the XY plane into a position vector that
     * represents the pinch location of the virtual arcball.
     */
    static Vector3d calculateArcballVector(const Vector3d& p);

    /*
     * Rotate a position vector about the specified axis vector by the specified
     * angle (in radians);
     */
    static Vector3d rotatePositionVector(const float& angle,
                                         const Vector3d& axis,
                                         const Vector3d& position);

    /*
     * Main display function registered to glutDisplayFunc().
     */
    static void onMainWindowDisplay();

    /*
     * Maintains current output mode for lifetime of program.
     */
    static OutputMode m_output_mode;

    /*
     * Maintains the last previous output mode for lifetime of program.
     */
    static OutputMode m_previous_output_mode;

    /*
     * Pseudo-control points that are to be linearly interpolated to render
     * a bezier line.
     */
    static ControlPointManager::ControlPoints m_bezier_line;

    /*
     * The current axis of revolution (defaults to <0,1,0>) used in rendering.
     * a surface of revolution.
     */
    static Vector3d m_axis_of_revolution;
};

#endif /* OUTPUTMANAGER_HPP_ */
