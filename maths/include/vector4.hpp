//
// Created by m.mehalin on 22/02/2022.
//

#pragma once

struct Vector3;

struct Vector4 {
    union {
        struct { float x; float y; float z; float w; };
        struct { float r; float g; float b; float a; };

        float element[4]{};
    };

////////////////////////////// CONSTRUCTORS

    Vector4();
    Vector4(const float& i_x, const float& i_y, const float& i_z, const float& i_w);
    Vector4(const Vector3& i_v3, const float& i_w);

////////////////////////////// OPERATORS

    template<typename T>
    Vector4 operator*(const T& i_k) const;
    Vector4 operator*(const Vector4& i_vec) const;

    template<typename T>
    Vector4 operator/(const T& i_k) const;
    Vector4 operator/(const Vector4& i_vec) const;

    template<typename T>
    Vector4 operator+(const T& i_k) const;
    Vector4 operator+(const Vector4& i_vec) const;

    Vector4 operator-(const Vector4& i_vec) const;
    Vector4 operator-(const float& i_k) const;
    Vector4 operator-() const;

    Vector4 operator+=(Vector4& i_vec);

    Vector4 operator-=(Vector4& i_vec);

    bool operator==(Vector4& i_vec) const;

    template<typename T>
    Vector4 &operator*=(const T& i_k);

////////////////////////////// FUNCTIONS

/**
 * @return a vector with same direction but length = 1
 */
    Vector4 Normalize() const;

/**
 * @return vector's absolute values
 */
    Vector4 Abs() const;

/**
 * @return vector's length
 */
    float Length() const;

    /**
     * @summary Clamps each coordinates of the input vector between min and max values
     * @param i_v the input vector
     * @param i_min minimum value to compare against
     * @param i_max maximum value to compare against
     */
    static Vector4 Clamp(const Vector4& i_v, float i_min, float i_max);

};

std::ostream& operator<<(std::ostream& o_o, const Vector4& i_vec);

#include "vector4.inl"