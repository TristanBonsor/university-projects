/*
 * Vector3d.hpp
 * Basic ADT declaration for a vector in R3.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

/*
 * This class provides basic properties, behaviour, and operations for a vector
 * in R3.
 */
class Vector3d
{
public:

    /*
     * Returns the scalar result of the dot product between two vectors.
     */
    static float dotProduct(const Vector3d& u, const Vector3d& v);

    /*
     * Returns the vector that results from the cross product of vectors u and
     * v; u X v (right handed).
     */
    static Vector3d crossProduct(const Vector3d& u, const Vector3d& v);

    /*
     * Default constructor, <0.0, 0.0, 0.0>.
     */
    Vector3d()
        : m_x(0.0f),
          m_y(0.0f),
          m_z(0.0f)
    {
    }

    /*
     * Explicit constructor, <x, y, z>.
     */
    Vector3d(const float& x, const float& y, const float& z)
        : m_x(x),
          m_y(y),
          m_z(z)
    {
    }

    /*
     * Copy constructor, v = <u1, u2, u3>.
     */
    Vector3d(const Vector3d& u)
        : m_x(u.m_x),
          m_y(u.m_y),
          m_z(u.m_z)
    {
    }

    /*
     * Assignment operator.
     */
    Vector3d& operator=(const Vector3d& rhs);

    /*
     * Equality operator.
     */
    bool operator==(const Vector3d& rhs) const;

    /*
     * Inequality operator.
     */
    bool operator!=(const Vector3d& rhs) const;

    /*
     * Returns the magnitude (norm) of the vector.
     */
    float getMagnitude() const;

    /*
     * Returns the normalized version of the vector.
     */
    Vector3d getNormalized() const;

    /*
     * Normalizes the vector. Previous values are discarded.
     */
    void normalize();

    /*
     * Returns the x component of the vector.
     */
    float getX() const;

    /*
     * Returns the y component of the vector.
     */
    float getY() const;

    /*
     * Returns the z component of the vector.
     */
    float getZ() const;

    /*
     * Sets the x component of the vector.
     */
    void setX(const float& x);

    /*
     * Sets the y component of the vector.
     */
    void setY(const float& y);

    /*
     * Sets the z component of the vector.
     */
    void setZ(const float& z);

private:

    float m_x, m_y, m_z;
};

#endif /* VECTOR3D_HPP_ */
