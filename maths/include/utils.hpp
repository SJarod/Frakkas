//
// Created by f.marcellot on 22/03/2022.
//

#pragma once

struct Vector3;
struct Vector4;

namespace Maths{

    /**
     *
     * @param i_degreeAngle an angle in degrees
     * @return the angle converted from degrees to radians
     */
    float ToRadians(float i_degAngle);

    /**
     *
     * @param i_radAngle an angle in radians
     * @return the angle converted from radians to degrees
     */
    float ToDegrees(float i_radAngle);

    /**
 * @summary Return minimum between two values, negative numbers accepted
 */
    template<typename T>
    T Min(T i_a, T i_b);

/**
 * @summary Return maximum between two values, negative numbers accepted
 */
    template<typename T>
    T Max(T i_a, T i_b);

/**
 * @summary Return minimum between three values, negative numbers accepted
 */
    template<typename T>
    T Min(T i_a, T i_b, T i_c);

/**
 * @summary Return maximum between three values, negative numbers accepted
 */
    template<typename T>
    T Max(T i_a, T i_b, T i_c);

/**
 * @summary Avoid a variable value to be outside of a range, negative numbers accepted
 */
    template<typename T>
    T Clamp(T i_val, T i_min, T i_max);

/**
 * @summary Return a float's absolute value
 */
    float Abs(float i_f);

/**
 * @summary Return a float's square root
 */
    float Sqrt(float i_f);

/**
 * @summary Return a float's square, negative numbers accepted
 */
    float Square(float i_f);

/**
 * @summary Return a float's cosine, negative numbers accepted
 * @param i_f   float in radian
 */
    float Cos(float i_f);

/**
 * @summary Return a float's arccosine
 * @param i_f   float in radian
 */
    float Acos(float i_f);

/**
 * @summary Return a float's sine
 * @param i_f   float in radian
 */
    float Sin(float i_f);

/**
 * @summary Return a float's arcsine
 * @param i_f   float in radian
 */
    float Asin(float i_f);

/**
 * @summary Return a float's tangent
 * @param i_f   float in radian
 */
    float Tan(float i_f);

/**
 * @summary Return a float's arctangent
 * @param i_f float in radian
 */
    float Atan(float i_f);

/**
 * @summary Linearly interpolates between i_start and i_reach by i_factor
 * @return The interpolated float result between the two float values
 * @param i_factor clamped to the range [0, 1]
 */
    template<typename T>
    T Lerp(T i_start, T i_reach, float i_factor);

/**
 * @summary Define the sign of a number, positive or negative
 * @returns 1 if the number is positive,
 *          -1 if the number is negative
 */
    template <typename T>
    int Sign(T i_val);

}

#include "utils.inl"
