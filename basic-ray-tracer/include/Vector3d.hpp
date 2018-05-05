/**
 * \file Vector3d.hpp
 * Basic ADT declaration for a vector in R3.
 *
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

/**
 * \class Vector3d Vector3d.hpp "include/Vector3d.hpp"
 * \brief ADT for a vector in R3.
 *
 * This class provides basic properties, behaviour, and operations for a vector
 * in R3.
 */
class Vector3d
{
public:

    /**
     * Calculate the dot product of two vectors in R3.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return The scalar result of the dot product.
     */
    static float dotProduct(const Vector3d& u, const Vector3d& v);

    /**
     * Calculate the vector that results from the cross product of two vectors
     * in R3 assuming u X v (right handed).
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return The vector result, in R3, of the cross product.
     */
    static Vector3d crossProduct(const Vector3d& u, const Vector3d& v);

    /**
     * Calculate the position vector of a given point on the line defined by two
     * position vectors in R3.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @param t The unit parameter of the line.
     * @return A position vector in R3.
     */
    static Vector3d interpolate(const Vector3d& u,
                                const Vector3d& v,
                                const float& t);

    /**
     * Default constructor.
     * Initializes the vector <0.0, 0.0, 0.0>.
     */
    Vector3d()
        : m_x(0.0f), m_y(0.0f), m_z(0.0f)
    {
    }

    /**
     * Explicit constructor.
     * Initialize the vector <x, y, z>.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     * @param z The z component of the vector.
     */
    Vector3d(const float& x, const float& y, const float& z)
        : m_x(x), m_y(y), m_z(z)
    {
    }

    /**
     * Copy constructor.
     * Initializes a vector v with the same components as vector u.
     * @param u A vector in R3.
     */
    Vector3d(const Vector3d& u)
        : m_x(u.m_x), m_y(u.m_y), m_z(u.m_z)
    {
    }

    /**
     * Assignment operator.
     * @param rhs A vector in R3.
     * @return A reference to the assignee vector.
     */
    Vector3d& operator=(const Vector3d& rhs);

    /**
     * Addition operator.
     * @param rhs A vector in R3.
     * @return A vector in R3.
     */
    Vector3d operator+(const Vector3d& rhs) const;

    /**
     * Subtraction operator.
     * @param rhs A vector in R3.
     * @return A vector in R3.
     */
    Vector3d operator-(const Vector3d& rhs) const;

    /**
     * Equality operator.
     * @param rhs A vector in R3.
     * @return True if vectors are equal, false otherwise.
     */
    bool operator==(const Vector3d& rhs) const;

    /**
     * Inequality operator.
     * @param rhs A vector in R3.
     * @return True if vectors are not equal, false otherwise.
     */
    bool operator!=(const Vector3d& rhs) const;

    /**
     * Calculate the magnitude (norm) of the vector.
     * @return A scalar representing the magnitude of the vector.
     */
    float getMagnitude() const;

    /**
     * Calculate the normalized version of the vector.
     * @return A normalized vector in R3.
     */
    Vector3d getNormalized() const;

    /**
     * Normalizes the vector. Previous component values are discarded.
     */
    void normalize();

    /**
     * Calculate the scaled version of the vector.
     * @param c A scalar value.
     * @return A scaled vector in R3.
     */
    Vector3d getScaled(const float& c) const;

    /**
     * Scales the vector. Previous component values are discarded.
     * @param c A scalar value.
     */
    void scale(const float& c);

    /**
     * Get the x component of the vector.
     * @return The value of the x component.
     */
    float getX() const;

    /**
     * Get the y component of the vector.
     * @return The value of the y component.
     */
    float getY() const;

    /**
     * Get the z component of the vector.
     * @return The value of the z component.
     */
    float getZ() const;

    /**
     * Sets the x component of the vector.
     * @param x The value to set the x component to.
     */
    void setX(const float& x);

    /**
     * Sets the y component of the vector.
     * @param y The value to set the y component to.
     */
    void setY(const float& y);

    /**
     * Sets the z component of the vector.
     * @param z The value to set the z component to.
     */
    void setZ(const float& z);

protected:

    /**
     * The x component.
     */
    float m_x;

    /**
     * The y component.
     */
    float m_y;

    /**
     * The z component.
     */
    float m_z;
};

#endif /* VECTOR3D_HPP_ */
