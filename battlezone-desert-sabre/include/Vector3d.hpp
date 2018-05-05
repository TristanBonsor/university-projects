/**
 * \file Vector3d.hpp
 * Basic ADT declaration for a vector in R3.
 *
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <cmath>

/**
 * \struct Vector3d Vector3d.hpp "include/Vector3d.hpp"
 * \brief ADT for a vector in R3.
 *
 * This structure provides basic properties, behaviour, and operations for a
 * vector in R3.
 */
struct Vector3d
{
    /**
     * Default constructor.
     * Initializes the vector <0.0, 0.0, 0.0>.
     */
    Vector3d()
        : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    /**
     * Explicit constructor.
     * Initialize the vector <x, y, z>.
     * @param a The value of the x component of the vector.
     * @param b The value of the y component of the vector.
     * @param c The value of the z component of the vector.
     */
    Vector3d(const float& a, const float& b, const float& c)
        : x(a), y(b), z(c)
    {
    }

    /**
     * Equality operator.
     * @param v A vector in R3.
     * @return True if vectors are equal, false otherwise.
     */
    inline bool operator==(const Vector3d& v) const;

    /**
     * Inequality operator.
     * @param v A vector in R3.
     * @return True if vectors are not equal, false otherwise.
     */
    inline bool operator!=(const Vector3d& v) const;

    /**
     * Calculate the length of the vector.
     * @return A scalar representing the length of the vector.
     */
    inline float length() const;

    /**
     * Calculate the squared length of the vector.
     * @return A scalar representing the squared length of the vector.
     */
    inline float lengthSquared() const;

    /**
     * Normalizes the vector. Previous component values are discarded.
     * @return A reference to the vector.
     */
    inline Vector3d& normalize();

    /**
     * Calculate the normalized version of the vector.
     * @return A normalized vector in R3.
     */
    inline Vector3d unit() const;

    /**
     * Calculate the dot product of two vectors in R3.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return The scalar result of the dot product.
     */
    static inline float dot(const Vector3d& u, const Vector3d& v);

    /**
     * Calculate the vector that results from the cross product of two vectors
     * in R3 assuming u X v (right handed).
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return The vector result, in R3, of the cross product.
     */
    static inline Vector3d cross(const Vector3d& u, const Vector3d& v);

    /**
     * Calculate the position vector of a given point on the line defined by two
     * position vectors in R3.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @param t The unit parameter of the line.
     * @return A position vector in R3.
     */
    static inline Vector3d lerp(const Vector3d& u,
                                const Vector3d& v,
                                const float& t);

    /**
     * Vector addition operator.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return A vector in R3.
     */
    friend inline Vector3d operator+(const Vector3d& u, const Vector3d& v);

    /**
     * Vector subtraction operator.
     * @param u A vector in R3.
     * @param v A vector in R3.
     * @return A vector in R3.
     */
    friend inline Vector3d operator-(const Vector3d& u, const Vector3d& v);

    /**
     * Vector scalar multiplication operator.
     * @param c A scalar value.
     * @param v A vector in R3.
     * @return A vector in R3.
     */
    friend inline Vector3d operator*(const float& c, const Vector3d& v);

    /**
     * Vector scalar multiplication operator.
     * @param v A vector in R3.
     * @param c A scalar value.
     * @return A vector in R3.
     */
    friend inline Vector3d operator*(const Vector3d& v, const float& c);

    float x; //!< The x component of the vector.
    float y; //!< The y component of the vector.
    float z; //!< The z component of the vector.
};

inline bool Vector3d::operator==(const Vector3d& v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

inline bool Vector3d::operator!=(const Vector3d& v) const
{
    return !(*this == v);
}

inline float Vector3d::length() const
{
    return sqrt(lengthSquared());
}

inline float Vector3d::lengthSquared() const
{
    return x * x + y * y + z * z;
}

inline Vector3d& Vector3d::normalize()
{
    const float len = length();

    if(len != 0)
    {
        const float scale = 1.0f / len;
        x *= scale;
        y *= scale;
        z *= scale;
    }

    return *this;
}

inline Vector3d Vector3d::unit() const
{
    Vector3d v(*this);
    v.normalize();
    return v;
}

inline float Vector3d::dot(const Vector3d& u, const Vector3d& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector3d Vector3d::cross(const Vector3d& u, const Vector3d& v)
{
    return Vector3d(u.y * v.z - u.z * v.y,
                    u.z * v.x - u.x * v.z,
                    u.x * v.y - u.y * v.x);
}

inline Vector3d Vector3d::lerp(const Vector3d& u,
                               const Vector3d& v,
                               const float& t)
{
    return Vector3d(u.x + t * (v.x - u.x),
                    u.y + t * (v.y - u.y),
                    u.z + t * (v.z - u.z));
}

inline Vector3d operator+(const Vector3d& u, const Vector3d& v)
{
    return Vector3d(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector3d operator-(const Vector3d& u, const Vector3d& v)
{
    return Vector3d(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector3d operator*(const float& c, const Vector3d& v)
{
    return Vector3d(v.x * c, v.y * c, v.z * c);
}

inline Vector3d operator*(const Vector3d& v, const float& c)
{
    return c * v;
}

#endif /* VECTOR3D_HPP_ */
