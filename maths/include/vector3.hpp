//
// Created by manon on 21/02/2022.
//

#pragma once

#include "constants.hpp"

struct Vector4;
struct Quaternion;

struct Vector3 {
    union {
        struct { float x; float y; float z; };
        struct { float r; float g; float b; };

        float element[3]{};
    };

////////////////////////////// CONSTRUCTORS

    Vector3();
    Vector3(const float& i_x, const float& i_y, const float& i_z);
    explicit Vector3(const Vector4& i_v4);

////////////////////////////// CONSTANTS

    static const Vector3 zero;
    static const Vector3 right;
    static const Vector3 left;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 forward;
    static const Vector3 backward;

////////////////////////////// OPERATORS

    template<typename T>
    Vector3 operator*(const T& i_k) const;
    Vector3 operator*(const Vector3& i_vec) const;

    template<typename T>
    Vector3 operator/(const T& i_k) const;
    Vector3 operator/(const Vector3& i_vec) const;

    template<typename T>
    Vector3 operator+(const T& i_k) const;
    Vector3 operator+(const Vector3& i_vec) const;

    Vector3 operator-(const Vector3& i_vec) const;
    Vector3 operator-(const float& i_k) const;
    Vector3 operator-() const;

    Vector3 operator^(const Vector3& i_vec) const;

    Vector3 operator+=(Vector3& i_vec);

    Vector3 operator-=(Vector3& i_vec);

    bool operator==(const Vector3& i_vec) const;

    template<typename T>
    Vector3 &operator*=(const T& i_k);

////////////////////////////// FUNCTIONS

/**
 * @return a vector with same direction but length = 1
 */
    Vector3 Normalize() const;

/**
 * @return vector's absolute values
 */
    Vector3 Abs() const;

/**
 * @return vector's length/magnitude
 */
    float Length() const;

/**
 * @returns dot product of two vectors.
 * If the vectors have the same direction the dot product return 1
 * If the vectors are orthogonal the dot product return 0
 * If the vectors have the opposite direction dot product return -1
 */
    static float DotProduct(const Vector3 &i_vecA, const Vector3 &i_vecB);

/**
 * @returns
 * cross product of two vectors, equal to a perpendicular vector to the two input vectors
 */
    static Vector3 CrossProduct(const Vector3 &i_vecA, const Vector3 &i_vecB);

/**
 * @return
 * the vector which starts from point A and end at point B,
 * equivalent to AB vector
 */
    static Vector3 VecFromPt(const Vector3 &i_ptA, const Vector3 &i_ptB);

/**
 * @return the vector rotated by a quaternion
 * @param i_q should be normalized,
 * you can use Quaternion::QuatFromAngleAxis() to rotate vector around axis
 */
    static Vector3 RotateByQuat(const Vector3& i_v, const Quaternion& i_q);

    /**
     * @summary Clamps each coordinates of the input vector between min and max values
     * @param i_v the input vector
     * @param i_min minimum value to compare against
     * @param i_max maximum value to compare against
     */
    static Vector3 Clamp(const Vector3& i_v, float i_min, float i_max);
};

std::ostream& operator<<(std::ostream& o_o, const Vector3& i_vec);

#include "vector3.inl"