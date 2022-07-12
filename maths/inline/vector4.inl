#include "maths/vector3.hpp"
#include "maths/utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Vector4::Vector4()
        : x(0.f), y(0.f), z(0.f), w(0.f)
{}
inline Vector4::Vector4(const float& _x, const float& _y, const float& _z, const float& _w)
        : x(_x), y(_y), z(_z), w(_w)
{}
inline Vector4::Vector4(const Vector3& v3, const float& _w)
        : x(v3.x), y(v3.y), z(v3.z), w(_w)
{}

////////////////////////////// OPERATORS

template<typename T>
inline Vector4 Vector4::operator*(const T& k) const
{
    return {
            x * k,
            y * k,
            z * k,
            w * k
    };
}
inline Vector4 Vector4::operator*(const Vector4& vec) const
{
    return {
            x * vec.x,
            y * vec.y,
            z * vec.z,
            w * vec.w
    };
}

template<typename T>
inline Vector4 Vector4::operator/(const T& k) const
{
    if (k == 0.f)
        return { 0.f, 0.f, 0.f, 0.f };

    T quotient = 1.f / k;

    return {
            x * quotient,
            y * quotient,
            z * quotient,
            w * quotient
    };
}
inline Vector4 Vector4::operator/(const Vector4& vec) const
{
    return {
            x / vec.x,
            y / vec.y,
            z / vec.z,
            w / vec.w
    };
}

template<typename T>
inline Vector4 Vector4::operator+(const T& k) const
{
    return {
            k + x,
            k + y,
            k + z,
            k + w
    };
}
inline Vector4 Vector4::operator+(const Vector4& vec) const
{
    return {
            x + vec.x,
            y + vec.y,
            z + vec.z,
            w + vec.w
    };
}

inline Vector4 Vector4::operator+(const Vector3& i_vec) const
{
    return Vector4(x + i_vec.x, y + i_vec.y, z + i_vec.z, w);
}

inline Vector4 Vector4::operator-(const Vector4& vec) const
{
    return {
            x - vec.x,
            y - vec.y,
            z - vec.z,
            w - vec.w
    };
}
inline Vector4 Vector4::operator-(const float& k) const
{
    return {
            k - x,
            k - y,
            k - z,
            k - w
    };
}
inline Vector4 Vector4::operator-() const
{
    return {
            -x,
            -y,
            -z,
            -w
    };
}

inline Vector4 Vector4::operator+=(Vector4& vec)
{
    return {
            x += vec.x,
            y += vec.y,
            z += vec.z,
            w += vec.w
    };
}

inline Vector4 Vector4::operator-=(Vector4& vec)
{
    return {
            x -= vec.x,
            y -= vec.y,
            z -= vec.z,
            w -= vec.w
    };
}

inline bool Vector4::operator==(Vector4& vec) const
{
    return (
            x == vec.x &&
            y == vec.y &&
            z == vec.z &&
            w == vec.w
    );
}

template<typename T>
inline Vector4& Vector4::operator*=(const T& k)
{
    return this *= k;
}

inline std::ostream& operator<<(std::ostream& o, const Vector4& vec)
{
    return o << "x = " << vec.x << ", y = " << vec.y << ", z = " << vec.z << ", w = " << vec.w;
}

////////////////////////////// FUNCTIONS

inline Vector4 Vector4::Normalize() const
{
    float mag = this->Length();

    if (mag == 0.f)
        return *this;

    return *this / mag;
}

inline Vector4 Vector4::Abs() const
{
    return { Maths::Abs(x), Maths::Abs(y), Maths::Abs(z), Maths::Abs(w) };
}

inline float Vector4::Length() const
{
    return Maths::Sqrt(x * x + y * y + z * z + w * w);
}

inline Vector4 Vector4::Clamp(const Vector4& i_v, float i_min, float i_max)
{
    return Vector4(Maths::Clamp(i_v.x, i_min, i_max),
                   Maths::Clamp(i_v.y, i_min, i_max),
                   Maths::Clamp(i_v.z, i_min, i_max),
                   Maths::Clamp(i_v.w, i_min, i_max));
}
