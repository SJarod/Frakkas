//
// Created by m.mehalin on 22/02/2022.
//

#pragma once

struct Vector3;
struct Matrix4;

struct Quaternion {
    union {
        struct { float x; float y; float z; float w; };

        float element[4]{};
    };

////////////////////////////// CONSTRUCTORS

    Quaternion();
    Quaternion(const float& i_x, const float& i_y, const float& i_z, const float& i_w);

////////////////////////////// OPERATORS

    template<typename T>
    Quaternion operator+(const T& i_k) const;
    Quaternion operator+(const Quaternion& i_q) const;

    template<typename T>
    Quaternion operator-(const T& i_k) const;
    Quaternion operator-(const Quaternion& i_q) const;

    template<typename T>
    Quaternion operator*(const T& i_k) const;
    Quaternion operator*(const Quaternion& i_q) const;

    Quaternion operator/(const Quaternion& i_q) const;

    Quaternion operator*=(const Quaternion& i_q);

    bool operator==(const Quaternion& i_q) const;

////////////////////////////// FUNCTIONS

/**
 * @summary identity have a lenght = 1, and does not affect multiplication
 * @return quaternion identity, the '1' factor in quaternion space
 */
    static Quaternion Identity();

/**
 * @return the quaternion with same rotation value, but lenght = 1
 */
    Quaternion Normalize() const;

/**
 * @summary lenght/magnitude of quaternion is equivalent to Vector4::Length()
 * @return the length of the quaternion
 */
    float Length() const;

/**
 * @summary inverse of a quaternion means that
 * if we multiply the input quaternion with its inverse, we got identity (q * q^-1 = Identity())
 * @return quaternion's inverse if exists, else return the input vector
 */
    Quaternion Invert() const;

/**
 * @return a quaternion from rotation matrix
 * @param i_mat should be a rotation matrix, else the result would have no sense
 */
    static Quaternion QuatFromMatrix(const Matrix4& i_mat);

/**
 * @summary Create a quaternion from traditional radian angles
 * @param i_roll Rotation around X axis, in radian
 * @param i_pitch Rotation around Y axis, in radian
 * @param i_yaw Rotation around Z axis, in radian
 */
    static Quaternion QuatFromEuler(const float& i_roll, const float& i_pitch, const float& i_yaw);

/**
 * @summary Create a quaternion given an angle, in radians, and a rotation axis
 * @param i_axis the axis of rotation, will be normalized
 * @param i_radAngle the angle of rotation in radians
 */
    static Quaternion QuatFromAxisAngle(const Vector3& i_axis, float i_radAngle);

/**
 * @summary Get the axis and angle of rotation from a quaternion
 */
    void QuatToAxisAngle(Vector3* o_outAxis, float* o_outAngle);

/**
 * @summary Calculate linear interpolation between two quaternions.
 * --- It is recommanded to normalize quaternion before.
 * @param i_factor will be clamped in the range[0, 1]
 */
    static Quaternion Lerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor);

/**
 * @summary Calculate slerp-optimized interpolation between two quaternions
 * --- It is recommanded to normalize quaternion before
 * @param i_factor will be clamped in the range[0, 1]
 */
    static Quaternion Nlerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor);

/**
 * @summary Calculates spherical linear interpolation between two quaternions
 * --- It is recommanded to normalize quaternion before
 * @param i_factor will be clamped in the range[0, 1]
 */
    static Quaternion Slerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor);
};

std::ostream& operator<<(std::ostream& o_o, const Quaternion& i_q);

#include "quaternion.inl"