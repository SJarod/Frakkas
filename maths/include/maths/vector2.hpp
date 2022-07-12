#pragma once

struct Vector3;

struct Vector2
{
    union
    {
        struct { float x; float y; };

        float element[2]{};
    };

////////////////////////////// CONSTRUCTORS

    Vector2();
    Vector2(const float& i_x, const float& i_y);
    Vector2(const Vector3& i_v);

////////////////////////////// OPERATORS

    template<typename T>
    Vector2 operator*(const T& i_k) const;
    Vector2 operator*(const Vector2& i_vec) const;

    template<typename T>
    Vector2 operator/(const T& i_k) const;
    Vector2 operator/(const Vector2& i_vec) const;

    template<typename T>
    Vector2 operator+(const T& i_k) const;
    Vector2 operator+(const Vector2& i_vec) const;

    Vector2 operator-(const Vector2& i_vec) const;
    Vector2 operator-(const float& i_k) const;
    Vector2 operator-() const;

    Vector2 operator+=(Vector2& i_vec);

    Vector2 operator-=(Vector2& i_vec);

    bool operator==(const Vector2& i_vec) const;

    template<typename T>
    Vector2 &operator*=(const T& i_k);

////////////////////////////// FUNCTIONS

/**
 * @return a vector with same direction but length = 1
 */
    Vector2 Normalize() const;

/**
 * @return vector's absolute values
 */
    Vector2 Abs() const;

/**
 * @return vector's length/magnitude
 */
    float Length() const;

};

std::ostream& operator<<(std::ostream& o_o, const Vector2& i_vec);

#include "vector2.inl"