#include "maths/vector3.hpp"
#include "maths/utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Vector2::Vector2()
        : x(0.f), y(0.f)
{}
inline Vector2::Vector2(const float& _x, const float& _y)
        : x(_x), y(_y)
{}

inline Vector2::Vector2(const Vector3& i_v)
    :x(i_v.x), y(i_v.y)
{}


////////////////////////////// OPERATORS

template<typename T>
inline Vector2 Vector2::operator*(const T& k) const
{
    return {
            x * k,
            y * k
    };
}
inline Vector2 Vector2::operator*(const Vector2& vec) const
{
    return {
            x * vec.x,
            y * vec.y
    };
}

template<typename T>
inline Vector2 Vector2::operator/(const T& k) const
{
    if (k == 0.f)
        return { 0.f, 0.f };

    T quotient = 1.f / k;

    return {
            x * quotient,
            y * quotient
    };
}
inline Vector2 Vector2::operator/(const Vector2& vec) const
{
    return {
            x / vec.x,
            y / vec.y
    };
}

template<typename T>
inline Vector2 Vector2::operator+(const T& k) const
{
    return {
            k + x,
            k + y
    };
}
inline Vector2 Vector2::operator+(const Vector2& vec) const
{
    return {
            x + vec.x,
            y + vec.y
    };
}

inline Vector2 Vector2::operator-(const Vector2& vec) const
{
    return {
            x - vec.x,
            y - vec.y
    };
}
inline Vector2 Vector2::operator-(const float& k) const
{
    return {
            k - x,
            k - y
    };
}
inline Vector2 Vector2::operator-() const
{
    return {
            -x,
            -y
    };
}

inline Vector2 Vector2::operator+=(Vector2& vec)
{
    return {
            x += vec.x,
            y += vec.y
    };
}

inline Vector2 Vector2::operator-=(Vector2& vec)
{
    return {
            x -= vec.x,
            y -= vec.y
    };
}

inline bool Vector2::operator==(const Vector2& vec) const
{
    return (
            x == vec.x &&
            y == vec.y
    );
}

template<typename T>
inline Vector2& Vector2::operator*=(const T& k)
{
    return this *= k;
}

inline std::ostream& operator<<(std::ostream& o, const Vector2& vec)
{
    return o << "x = " << vec.x << ", y = " << vec.y;
}

////////////////////////////// FUNCTIONS

inline Vector2 Vector2::Normalize() const
{
    float mag = this->Length();

    if (mag == 0.f)
        return *this;

    return *this / mag;
}

inline Vector2 Vector2::Abs() const
{
    return { Maths::Abs(x), Maths::Abs(y) };
}

inline float Vector2::Length() const
{
    return Maths::Sqrt(x * x + y * y);
}