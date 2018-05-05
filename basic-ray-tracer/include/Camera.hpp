/**
 * \file Camera.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "Vector3d.hpp"

/**
 * \class Camera Camera.hpp "include/Camera.hpp"
 * \brief A singleton camera class.
 *
 * Provides an interface for moving and orienting the camera that is viewing
 * the scene.
 */
class Camera
{
public:

    /**
     * Translates the camera position.
     * @param v Direction vector in R3 by which the camera will be translated.
     */
    static void translate(const Vector3d& v);

    /**
     * Pitches the camera up or down.
     * @param angle An angle in radians. Positive to pitch down, negative to
     * pitch up.
     */
    static void pitch(const float& angle);

    /**
     * Yaws the camera left or right.
     * @param angle An angle in radians. Negative to yaw left, positive to yaw
     * right.
     */
    static void yaw(const float& angle);

    /**
     * Get the camera's position vector.
     * @return A vector in R3.
     */
    static Vector3d getPosition();

    /**
     * Get the camera's up vector.
     * @return A vector in R3.
     */
    static Vector3d getUp();

    /**
     * Get the camera's at vector.
     * @return A vector in R3.
     */
    static Vector3d getAt();

    /**
     * Get the camera's right vector.
     * @return A vector in R3.
     */
    static Vector3d getRight();

    /**
     * Get the distance from the camera to the image plane.
     * @return A scalar distance.
     */
    static float getImageDistance();

    /**
     * Set the camera's position vector.
     * @param p A vector in R3.
     */
    static void setPosition(const Vector3d& p);

    /**
     * Set the camera's up vector.
     * @param v A vector in R3.
     */
    static void setUp(const Vector3d& v);

    /**
     * Set the camera's at vector.
     * @param v A vector in R3.
     */
    static void setAt(const Vector3d& v);

    /**
     * Set the camera's right vector.
     * @param v A vector in R3.
     */
    static void setRight(const Vector3d& v);

    /**
     * Set the distance from the camera to the image plane.
     * @param d A scalar distance.
     */
    static void setImageDistance(const float& d);

protected:

    /**
     * Rotate a position vector about an axis by a given angle.
     * @param angle An angle in radians.
     * @param axis A vector in R3.
     * @param position A position vector in R3.
     * @return A position vector in R3.
     */
    static Vector3d rotate(const float& angle,
                           const Vector3d& axis,
                           const Vector3d& position);

    /**
     * The camera position.
     */
    static Vector3d m_position;

    /**
     * The camera up vector.
     */
    static Vector3d m_up;

    /**
     * The camera at vector.
     */
    static Vector3d m_at;

    /**
     * The camera right vector.
     */
    static Vector3d m_right;

    /**
     * The image distance from the camera.
     */
    static float m_image_distance;
};

#endif /* CAMERA_HPP_ */
