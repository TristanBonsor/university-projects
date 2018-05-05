/**
 * \file Quaternion.hpp
 * \copyright (C) Tristan W. Bonsor
 */

#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_

#include "Vector3d.hpp"

/**
 * \struct Quaternion Quaternion.hpp "include/Quaternion.hpp"
 * \brief ADT for a quaternion used in R3.
 *
 * This structure provides basic properties, behaviour, and operations for a
 * quaternion in R3.
 */
struct Quaternion
{
    /**
     * Default constructor.
     * Initializes an identity quaternion.
     */
    Quaternion()
        : w(1.0f), x(0.0f), y(0.0f), z(0.0f)
    {
    }

    /**
     * Explicit constructor.
     * Initializes an explicitly defined quaternion.
     * @param a
     * @param b
     * @param c
     * @param d
     */
    Quaternion(const float& a, const float& b, const float& c, const float& d)
        : w(a), x(b), y(c), z(d)
    {
    }

    /**
     * Explicit constructor.
     * Initializes an explicitly defined quaternion.
     * @param angle
     * @param axis
     */
    Quaternion(const float& angle, const Vector3d& axis)
    {
        const float a = angle * 0.5f;
        const float s = (float)sin(a);
        const float c = (float)cos(a);
        w = c;
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
    }

    /**
     * Equality operator.
     * @param u A quaternion in R3.
     * @return True if quaternions are equal, false otherwise.
     */
    inline bool operator==(const Quaternion& u) const;

    /**
     * Inequality operator.
     * @param u A quaternion in R3.
     * @return True if quaternions are not equal, false otherwise.
     */
    inline bool operator!=(const Quaternion& u) const;

    /**
     * Determine the Euler angle and rotation axis specified by the quaternion.
     * @param angle
     * @param axis
     */
    inline void angleAxis(float& angle, Vector3d& axis) const;

    /**
     * Determine the quaternion's conjugate.
     * @return The conjugate of the quaternion.
     */
    inline Quaternion conjugate() const;

    /**
     * Determine the quaternion's length.
     * @return A scalar value representing the quaternion's length.
     */
    inline float length() const;

    /**
     * Determine the quaternion's norm.
     * @return A scalar value representing the quaternion's norm (length
     * squared).
     */
    inline float norm() const;

    /**
     * Normalize the quaternion.
     * @return A reference to the quaternion.
     */
    inline Quaternion& normalize();

    /**
     * Determine the quaternion's inverse.
     * @return The inverse of the quaternion.
     */
    inline Quaternion inverse() const;

    /**
     * Determine the dot product of two quaternions.
     * @param u A quaternion in R3.
     * @param v A quaternion in R3.
     * @return The quaternion resulting from the dot product operation.
     */
    static inline Quaternion dot(const Quaternion& u, const Quaternion& v);

    /**
     *
     */
    static inline Quaternion slerp(const Quaternion& u,
                                   const Quaternion& v,
                                   const float t);

    /**
     *
     */
    static inline Quaternion rotationBetweenVectors(const Vector3d& u,
                                                    const Vector3d& v);

    /**
     * Quaternion addition operator.
     * @param u A quaternion in R3.
     * @param v A quaternion in R3.
     * @return The quaternion resulting from the addition operation.
     */
    friend inline Quaternion operator+(const Quaternion& u,
                                       const Quaternion& v);

    /**
     * Quaternion subtraction operator.
     * @param u A quaternion in R3.
     * @param v A quaternion in R3.
     * @return The quaternion resulting from the subtraction operation.
     */
    friend inline Quaternion operator-(const Quaternion& u,
                                       const Quaternion& v);

    /**
     * Quaternion multiplication operator.
     * @param u A quaternion in R3.
     * @param v A quaternion in R3.
     * @return The quaternion resulting from the multiplication operation.
     */
    friend inline Quaternion operator*(const Quaternion& u,
                                       const Quaternion& v);

    /**
     * Quaternion scalar multiplication operator.
     * @param c A scalar value.
     * @param u A quaternion in R3.
     * @return The quaternion resulting from the scalar multiplication
     * operation.
     */
    friend inline Quaternion operator*(const float& c, const Quaternion& u);

    /**
     * Quaternion scalar multiplication operator.
     * @param u A quaternion in R3.
     * @param c A scalar value.
     * @return The quaternion resulting from the scalar multiplication
     * operation.
     */
    friend inline Quaternion operator*(const Quaternion& u, const float& c);

    /**
     * Quaternion vector multiplication operator.
     * @param v A vector in R3.
     * @param u A quaternion in R3.
     * @return The vector, in R3, resulting from the quaternion vector
     * multiplication operation.
     */
    friend inline Vector3d operator*(const Vector3d& v, const Quaternion& u);

    /**
     * Quaternion vector multiplication operator.
     * @param u A quaternion in R3.
     * @param v A vector in R3.
     * @return The vector, in R3, resulting from the quaternion vector
     * multiplication operation.
     */
    friend inline Vector3d operator*(const Quaternion& u, const Vector3d& v);

    float w; //!< The w component of the quaternion.
    float x; //!< The x component of the quaternion.
    float y; //!< The y component of the quaternion.
    float z; //!< The z component of the quaternion.
};

inline bool Quaternion::operator==(const Quaternion& u) const
{
    return (w == u.w && x == u.x && y == u.y && z == u.z);
}

inline bool Quaternion::operator!=(const Quaternion& u) const
{
    return !(*this == u);
}

inline void Quaternion::angleAxis(float& angle, Vector3d& axis) const
{
    const float square_len = norm();

    if(square_len > 0.0000000001f)
    {
        angle = 2.0f * (float)acos(w);
        const float inverse_len = 1.0f / (float)pow(square_len, 0.5f);
        axis.x = x * inverse_len;
        axis.y = y * inverse_len;
        axis.z = z * inverse_len;
    }
    else
    {
        angle = 0.0f;
        axis.x = 0.0f;
        axis.y = 1.0f;
        axis.z = 0.0f;
    }
}

inline Quaternion Quaternion::conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

inline float Quaternion::length() const
{
    return sqrt(norm());
}

inline float Quaternion::norm() const
{
    return w * w + x * x + y * y + z * z;
}

inline Quaternion& Quaternion::normalize()
{
    const float len = length();

    if(len != 0)
    {
        float inv = 1.0f / len;
        w *= inv;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    else
    {
        w = 1.0f;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    return *this;
}

inline Quaternion Quaternion::inverse() const
{
    const float n = norm();

    if(n != 0)
    {
        return Quaternion(w / n, -x / n, -y / n, -z / n);
    }

    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

inline Quaternion Quaternion::dot(const Quaternion& u, const Quaternion& v)
{
    return Quaternion(u.w * v.w + u.x * v.x + u.y * v.y + u.z * v.z, 0, 0, 0);
}

inline Quaternion Quaternion::slerp(const Quaternion& u,
                                    const Quaternion& v,
                                    const float t)
{
    float flip = 1;

    float cosine = u.w * v.w + u.x * v.x + u.y * v.y + u.z * v.z;

    if(cosine < 0)
    {
        cosine = -cosine;
        flip = -1;
    }

    if((1 - cosine) < 0.00001f)
    {
        return u * (1 - t) + v * (t * flip);
    }

    float theta = (float)acos(cosine);
    float sine = (float)sin(theta);
    float beta = (float)sin((1 - t) * theta) / sine;
    float alpha = (float)sin(t * theta) / sine * flip;

    return u * beta + v * alpha;
}

inline Quaternion Quaternion::rotationBetweenVectors(const Vector3d& u,
                                                     const Vector3d& v)
{
    Vector3d start(u);
    Vector3d destination(v);

    start.normalize();
    destination.normalize();

    float cosTheta = Vector3d::dot(start, destination);

    Vector3d rotationAxis;

    if(cosTheta < -1 + 0.001f)
    {
        // Special case when vectors in opposite directions so guess one.
        rotationAxis = Vector3d::cross(Vector3d(0.0f, 0.0f, 1.0f), start);

        if(rotationAxis.lengthSquared() < 0.01)
        {
            // Parallel, try again!
            rotationAxis = Vector3d::cross(Vector3d(1.0f, 0.0f, 0.0f), start);
        }

        rotationAxis.normalize();

        return Quaternion(180.0f, rotationAxis);
    }

    rotationAxis = Vector3d::cross(start, destination);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return Quaternion(s * 0.5f,
                      rotationAxis.x * invs,
                      rotationAxis.y * invs,
                      rotationAxis.z * invs);
}

inline Quaternion operator+(const Quaternion& u, const Quaternion& v)
{
    return Quaternion(u.w + v.w, u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Quaternion operator-(const Quaternion& u, const Quaternion& v)
{
    return Quaternion(u.w - v.w, u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Quaternion operator*(const Quaternion& u, const Quaternion& v)
{
    return Quaternion(u.w * v.w - u.x * v.x - u.y * v.y - u.z * v.z,
                      u.w * v.x + u.x * v.w + u.y * v.z - u.z * v.y,
                      u.w * v.y - u.x * v.z + u.y * v.w + u.z * v.x,
                      u.w * v.z + u.x * v.y - u.y * v.x + u.z * v.w);
}

inline Quaternion operator*(const float& c, const Quaternion& u)
{
    return Quaternion(u.w * c, u.x * c, u.y * c, u.z * c);
}

inline Quaternion operator*(const Quaternion& u, const float& c)
{
    return c * u;
}

inline Vector3d operator*(const Vector3d& v, const Quaternion& u)
{
    float n1 = u.x * 2.0f;
    float n2 = u.y * 2.0f;
    float n3 = u.z * 2.0f;
    float n4 = u.x * n1;
    float n5 = u.y * n2;
    float n6 = u.z * n3;
    float n7 = u.x * n2;
    float n8 = u.x * n3;
    float n9 = u.y * n3;
    float n10 = u.w * n1;
    float n11 = u.w * n2;
    float n12 = u.w * n3;

    float x = (1.0f - (n5 + n6)) * v.x + (n7 - n12) * v.y + (n8 + n11) * v.z;
    float y = (n7 + n12) * v.x + (1.0f - (n4 + n6)) * v.y + (n9 - n10) * v.z;
    float z = (n8 - n11) * v.x + (n9 + n10) * v.y + (1.0f - (n4 + n5)) * v.z;

    return Vector3d(x, y, z);
}

inline Vector3d operator*(const Quaternion& u, const Vector3d& v)
{
    return v * u;
}

#endif /* QUATERNION_HPP_ */
