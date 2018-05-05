/*
 * OutputManager.cc
 * OutputManager implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Defines.hpp"
#include "OutputManager.hpp"
#include "WindowManager.hpp"

/*
 * Static member initialization.
 */

OutputManager::OutputMode OutputManager::m_output_mode =
    OutputManager::OutputMode::NONE;

OutputManager::OutputMode OutputManager::m_previous_output_mode =
    OutputManager::OutputMode::NONE;

ControlPointManager::ControlPoints OutputManager::m_bezier_line = {};

Vector3d OutputManager::m_axis_of_revolution =
    Vector3d(DEFAULT_AXIS_OF_REVOLUTION);

/*
 * Static method definition.
 */

void OutputManager::init()
{
    // Register display function and fix camera position and perspective to
    // default values.
    glutSetWindow(WindowManager::getWindowId(WindowManager::Window::MAIN));
    glutDisplayFunc(onMainWindowDisplay);

    fixCamera();

}

void OutputManager::toggleOutputMode(const OutputManager::OutputMode& mode)
{
    // Store old output mode in case we are transitioning to surface of
    // revolution mode.
    m_previous_output_mode = m_output_mode;

    if(mode == m_output_mode)
    {
        m_output_mode = OutputMode::NONE;
    }
    else
    {
        m_output_mode = mode;
    }
}

void OutputManager::arcballScene(const Vector3d& xy_plane_vector1,
                                 const Vector3d& xy_plane_vector2)
{

    // Transform and rotate arcball position vectors from XY plane to
    // appropriate virtual arcball position vectors.

    Vector3d arcball_vector1 = calculateArcballVector(xy_plane_vector1);
    Vector3d arcball_vector2 = calculateArcballVector(xy_plane_vector2);

    arcball_vector1.normalize();
    arcball_vector2.normalize();

    // Calculate angle between arcball position vectors.

    float cos_value = Vector3d::dotProduct(arcball_vector1, arcball_vector2);

    if(cos_value > MAX_COS_ARGUMENT)
    {
        cos_value = MAX_COS_ARGUMENT;
    }
    else if(cos_value < MIN_COS_ARGUMENT)
    {
        cos_value = MIN_COS_ARGUMENT;
    }

    float rotation_angle_radians = acos(cos_value);

    float rotation_angle_degrees = RADIANS_TO_DEGREES * rotation_angle_radians;

    // Calculate rotation axis vector specified by arcball position vectors.

    Vector3d rotation_axis_vector = Vector3d::crossProduct(arcball_vector1,
                                                           arcball_vector2);

    rotation_axis_vector.normalize();

    // Accumulate all rotations in the same model view matrix.

    glMatrixMode(GL_MODELVIEW);

    glRotatef(rotation_angle_degrees,
              rotation_axis_vector.getX(),
              rotation_axis_vector.getY(),
              rotation_axis_vector.getZ());

    // Update display with glutDisplayFunc() callback.
    glutPostRedisplay();
}

void OutputManager::fixCamera()
{
    glutSetWindow(WindowManager::getWindowId(WindowManager::Window::MAIN));

    // Calculate camera perspective based on window dimensions and default
    // camera radius.

    float width = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);

    float fov = CAMERA_FIELD_OF_VIEW_CONVERSION
                * atan2(height / DIVIDE2, CAMERA_RADIUS);

    float aspect = width / height;

    // Set calculated camera perspective and set camera position/orientation to
    // default.

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, NEAR_Z, FAR_Z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(DEFAULT_CAMERA_LOOK_AT);
}

void OutputManager::clearBezierLine()
{
    m_bezier_line.clear();
}

void OutputManager::drawAxis()
{
    glColor3f(COLOR_RED);

    glBegin(GL_LINES);
    glVertex3f(POSITIVE_X_AXIS);
    glVertex3f(NEGATIVE_X_AXIS);
    glEnd();

    glColor3f(COLOR_GREEN);

    glBegin(GL_LINES);
    glVertex3f(POSITIVE_Y_AXIS);
    glVertex3f(NEGATIVE_Y_AXIS);
    glEnd();

    glColor3f(COLOR_BLUE);

    glBegin(GL_LINES);
    glVertex3f(POSITIVE_Z_AXIS);
    glVertex3f(NEGATIVE_Z_AXIS);
    glEnd();

    glFinish();
    glutSwapBuffers();
}

void OutputManager::drawPoint3d(const Vector3d& p)
{
    glPointSize(CONTROL_POINT_SIZE);

    glBegin(GL_POINTS);
    glVertex3f(p.getX(), p.getY(), p.getZ());
    glEnd();
    glFinish();
    glutSwapBuffers();
}

void OutputManager::drawLinearLine3d(const Vector3d& p1, const Vector3d& p2)
{
    glBegin(GL_LINES);
    glVertex3f(p1.getX(), p1.getY(), p1.getZ());
    glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    glEnd();
    glFinish();
    glutSwapBuffers();
}

void OutputManager::drawControlPoints()
{
    ControlPointManager::ControlPoints cps =
        ControlPointManager::getControlPoints();

    for(auto& p : cps)
    {
        drawPoint3d(p);
    }
}

void OutputManager::drawLinearLines()
{
    ControlPointManager::ControlPoints cps =
        ControlPointManager::getControlPoints();

    for(int i = 0; i < (int)cps.size() - 1; ++i)
    {
        drawLinearLine3d(cps[i], cps[i + 1]);
    }
}

void OutputManager::drawBezierLine()
{
    calculateBezierLine();

    for(int i = 0; i < (int)m_bezier_line.size() - 1; ++i)
    {
        drawLinearLine3d(m_bezier_line[i], m_bezier_line[i + 1]);
    }
}

void OutputManager::drawSurfaceOfRevolution()
{
    ControlPointManager::ControlPoints cps;

    // The surface to be drawn will depend on the output of the previous output
    // mode.
    switch(m_previous_output_mode)
    {
        case OutputMode::NONE:
        case OutputMode::LINEAR:

            cps = ControlPointManager::getControlPoints();

            break;

        case OutputMode::BEZIER:

            cps = m_bezier_line;

            break;

        default:

            break;
    }

    if(cps.empty())
    {
        return;
    }

    // Revolve and draw currently displayed points.
    if(m_previous_output_mode == OutputMode::NONE)
    {
        for(int i = 0; i <= MAX_REVOLUTION_SLICES; ++i)
        {
            glBegin(GL_POINTS);
            for(auto& p : cps)
            {
                Vector3d rp =
                    rotatePositionVector((i / REVOLUTION_SLICE_DENOMINATOR) * PI,
                                         m_axis_of_revolution,
                                         p);

                glVertex3f(rp.getX(), rp.getY(), rp.getZ());
            }
            glEnd();
        }
    }
    // Revolve and draw currently displayed line segments.
    else if(m_previous_output_mode == OutputMode::LINEAR
            || m_previous_output_mode == OutputMode::BEZIER)
    {
        for(int i = 0; i <= MAX_REVOLUTION_SLICES; ++i)
        {
            glBegin(GL_LINE_STRIP);
            for(auto& p : cps)
            {
                Vector3d rp =
                    rotatePositionVector((i / REVOLUTION_SLICE_DENOMINATOR) * PI,
                                         m_axis_of_revolution,
                                         p);

                glVertex3f(rp.getX(), rp.getY(), rp.getZ());
            }
            glEnd();
        }
    }

    glutSwapBuffers();
}

void OutputManager::calculateBezierLine()
{
    ControlPointManager::ControlPoints cps =
        ControlPointManager::getControlPoints();

    ControlPointManager::ControlPoints bezier_control_points;

    // Calculate points along each line segment as specified by current control
    // points. These calculated points are used for further bezier line
    // calculations.
    for(int i = 0; i < (int)cps.size() - 1; ++i)
    {
        // Find equation of the line.

        float x1 = cps[i].getX();
        float y1 = cps[i].getY();

        float x2 = cps[i + 1].getX();
        float y2 = cps[i + 1].getY();

        float diff_x = x2 - x1;
        float diff_y = y2 - y1;

        float m = diff_y / diff_x;

        float b = y2 - m * x2;

        // Calculate distance between each x input used to calculate a point on
        // this line.
        float dx = abs(diff_x) / (float)BEZIER_POINTS_PER_SEGMENT;

        float x = x1;
        float y = 0.0f;

        // Check if we must iterate in a decreasing fashion along x-axis.
        if(diff_x < 0.0f)
        {
            dx = -dx;
        }

        // Line is not vertical.
        if(dx != 0.0f)
        {
            for(int j = 0; j <= BEZIER_POINTS_PER_SEGMENT; ++j)
            {
                y = m * x + b;
                bezier_control_points.push_back(Vector3d(x, y, 0.0f));
                x += dx;
            }
        }
        // Line is vertical.
        else
        {
            float dy = abs(diff_y) / (float)BEZIER_POINTS_PER_SEGMENT;

            if(diff_y < 0.0f)
            {
                dy = -dy;
            }

            y = y1;

            for(int j = 0; j <= BEZIER_POINTS_PER_SEGMENT; ++j)
            {
                bezier_control_points.push_back(Vector3d(x, y, 0.0f));
                y += dy;
            }
        }
    }

    // Use the above calculated points to generate a bezier line.

    // Initialize t parameter and delta t.
    float t = 0.0f;
    float dt = 1.0f / (float)(bezier_control_points.size() - 1);

    m_bezier_line.clear();

    // Iterate through all calculated points. For each point, calculate a point
    // in a bezier line based on all other points.
    for(int i = 0; i < (int)bezier_control_points.size(); ++i)
    {
        ControlPointManager::ControlPoints bezier_points(bezier_control_points);

        for(int j = 1; j < (int)bezier_points.size(); ++j)
        {
            for(int k = 0; k < (int)bezier_points.size() - j; ++k)
            {
                bezier_points[k].setX((1.0f - t) * bezier_points[k].getX()
                                      + t * bezier_points[k + 1].getX());

                bezier_points[k].setY((1.0f - t) * bezier_points[k].getY()
                                      + t * bezier_points[k + 1].getY());
            }
        }

        // Append the final, refined point to bezier line list.
        m_bezier_line.push_back(bezier_points[0]);

        t += dt;
    }
}

Vector3d OutputManager::calculateArcballVector(const Vector3d& p)
{
    // Calculate positive z-component that emanates from XY plane (Pythagoras).
    // Assumes arcball position vector lies on the XY plane.

    Vector3d initial_arcball_vector = p;

    float c_square = pow(CAMERA_RADIUS, POWER2);

    float a_square = pow(p.getX(), POWER2) + pow(p.getY(), POWER2);

    if(a_square <= c_square)
    {
        initial_arcball_vector.setZ(sqrt(c_square - a_square));
    }
    else
    {
        initial_arcball_vector.normalize();
    }

    // Arcball position vector must now be rotated to the appropriate
    // orientation as specified by the camera position in world space.

    // Get accumulated rotation matrix and find its inverse. Since matrix
    // solely accumulates rotations, we may transpose it to accomplish this.
    GLfloat matrix[NUM_OPENGL_MATRIX_ENTRIES];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    Vector3d column_vector_x = Vector3d(matrix[OPENGL_MATRIX_ENTRY_11],
                                        matrix[OPENGL_MATRIX_ENTRY_12],
                                        matrix[OPENGL_MATRIX_ENTRY_13]);

    Vector3d column_vector_y = Vector3d(matrix[OPENGL_MATRIX_ENTRY_21],
                                        matrix[OPENGL_MATRIX_ENTRY_22],
                                        matrix[OPENGL_MATRIX_ENTRY_23]);

    Vector3d column_vector_z = Vector3d(matrix[OPENGL_MATRIX_ENTRY_31],
                                        matrix[OPENGL_MATRIX_ENTRY_32],
                                        matrix[OPENGL_MATRIX_ENTRY_33]);

    // Multiply arcball position vector with inverse rotation matrix to yield
    // the valid virtual arcball position vector.

    float x = Vector3d::dotProduct(initial_arcball_vector, column_vector_x);
    float y = Vector3d::dotProduct(initial_arcball_vector, column_vector_y);
    float z = Vector3d::dotProduct(initial_arcball_vector, column_vector_z);

    return Vector3d(x, y, z);
}

Vector3d OutputManager::rotatePositionVector(const float& angle,
                                             const Vector3d& axis,
                                             const Vector3d& position)
{
    float ax = axis.getX();
    float ay = axis.getY();
    float az = axis.getZ();

    // Calculate the rotation matrix.

    float x1 = (1.0f - cos(angle)) * pow(ax, POWER2) + cos(angle);
    float x2 = (1.0f - cos(angle)) * ax * ay - az * sin(angle);
    float x3 = (1.0f - cos(angle)) * ax * az - ay * sin(angle);

    Vector3d column_vector_x(x1, x2, x3);

    float y1 = (1.0f - cos(angle)) * ax * ay + az * sin(angle);
    float y2 = (1.0f - cos(angle)) * pow(ay, POWER2) + cos(angle);
    float y3 = (1.0f - cos(angle)) * ay * az - ax * sin(angle);

    Vector3d column_vector_y(y1, y2, y3);

    float z1 = (1.0f - cos(angle)) * ax * az - ay * sin(angle);
    float z2 = (1.0f - cos(angle)) * ay * az + ax * sin(angle);
    float z3 = (1.0f - cos(angle)) * pow(az, POWER2) + cos(angle);

    Vector3d column_vector_z(z1, z2, z3);

    // Multiply the position vector by the rotation matrix.

    float x = Vector3d::dotProduct(position, column_vector_x);
    float y = Vector3d::dotProduct(position, column_vector_y);
    float z = Vector3d::dotProduct(position, column_vector_z);

    return Vector3d(x, y, z);
}

void OutputManager::onMainWindowDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    drawAxis();

    glColor3f(COLOR_GREY);

    switch(m_output_mode)
    {
        case OutputMode::NONE:

            drawControlPoints();

            break;

        case OutputMode::LINEAR:

            drawLinearLines();

            break;

        case OutputMode::BEZIER:

            drawBezierLine();

            break;

        case OutputMode::SURFACE:

            drawSurfaceOfRevolution();

            break;

        default:

            break;
    }
}
