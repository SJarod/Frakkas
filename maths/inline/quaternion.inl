#include "maths/vector3.hpp"
#include "maths/matrix4.hpp"
#include "maths/utils.hpp"

////////////////////////////// CONSTRUCTORS

inline Quaternion::Quaternion()
        : x(0.f), y(0.f), z(0.f), w(1.f)
{}
inline Quaternion::Quaternion(const float& _x, const float& _y, const float& _z, const float& _w)
        : x(_x), y(_y), z(_z), w(_w)
{}

////////////////////////////// OPERATORS

template<typename T>
inline Quaternion Quaternion::operator+(const T& k) const
{
    return {
            x + k,
            y + k,
            z + k,
            w + k
    };
}
inline Quaternion Quaternion::operator+(const Quaternion& q) const
{
    return {
            x + q.x,
            y + q.y,
            z + q.z,
            w + q.w
    };
}

template<typename T>
inline Quaternion Quaternion::operator-(const T& k) const
{
    return {
            x - k,
            y - k,
            z - k,
            w - k
    };
}
inline Quaternion Quaternion::operator-(const Quaternion& q) const
{
    return {
            x - q.x,
            y - q.y,
            z - q.z,
            w - q.w
    };
}

template<typename T>
inline Quaternion Quaternion::operator*(const T& k) const
{
    float qax = x, qay = y, qaz = z, qaw = w;

    return {
            qax * k + qaw * k + qay * k - qaz * k,
            qay * k + qaw * k + qaz * k - qax * k,
            qaz * k + qaw * k + qax * k - qay * k,
            qaw * k - qax * k - qay * k - qaz * k
    };
}
inline Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return {
            x * q.w + y * q.z - z * q.y + w * q.x,
            -x * q.z + y * q.w + z * q.x + w * q.y,
            x * q.y - y * q.x + z * q.w + w * q.z,
            -x * q.x - y * q.y - z * q.z + w * q.w
    };
}

inline Quaternion Quaternion::operator/(const Quaternion& q) const
{
    return {
            x / q.x,
            y / q.y,
            z / q.z,
            w / q.w
    };
}

inline Quaternion Quaternion::operator*=(const Quaternion& q)
{
    return *this = *this * q;
}

inline bool Quaternion::operator==(const Quaternion& q) const
{
    return (
            Maths::Abs(x - q.x) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(y - q.y) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(z - q.z) <= Maths::Constants::weakEpsilon &&
            Maths::Abs(w - q.w) <= Maths::Constants::weakEpsilon
    );
}

inline std::ostream& operator<<(std::ostream& o, const Quaternion& q)
{
    return o << "x = " << q.x << ", y = " << q.y << ", z = " << q.z << ", w = " << q.w;
}

////////////////////////////// FUNCTIONS

inline Quaternion Quaternion::Identity()
{
    return { 0.f, 0.f, 0.f, 1.f };
}

inline Quaternion Quaternion::Normalize() const
{
    float length = this->Length();

    if (length < Maths::Constants::weakEpsilon)
        length = 1.0f;

    float ilength = 1.0f / length;

    return {
            x * ilength,
            y * ilength,
            z * ilength,
            w * ilength
    };
}

inline float Quaternion::Length() const
{
    return Maths::Sqrt(x * x + y * y + z * z + w * w);
}

inline Quaternion Quaternion::Invert() const
{
    Quaternion result = *this;
    float length = this->Length();
    float lengthSq = length * length;

    if (lengthSq != 0.0)
    {
        float i = 1.0f / lengthSq;

        result.x *= -i;
        result.y *= -i;
        result.z *= -i;
        result.w *= i;
    }

    return result;
}

inline Quaternion Quaternion::QuatFromMatrix(const Matrix4& i_mat)
{
#if 0 // GLM
    Matrix4 m = i_mat;
    float m00 = m.line[0].element[0];
    float m11 = m.line[1].element[1];
    float m22 = m.line[2].element[2];
    float m21 = m.line[2].element[1];
    float m12 = m.line[1].element[2];
    float m02 = m.line[0].element[2];
    float m20 = m.line[2].element[0];
    float m10 = m.line[1].element[0];
    float m01 = m.line[0].element[1];

    Quaternion q = {0.f, 0.f, 0.f, 0.f};
    q.w = Maths::Sqrt(1.f + m00 + m11 + m22) * 0.5f;
    if (q.w == 0.f)
        return {0.f, 0.f, 0.f, 1.f};

    q.x = (m21 - m12) / (4.f * q.w);
    q.y = (m20 - m02) / (4.f * q.w);
    q.z = (m10 - m01) / (4.f * q.w);

    return q;
#elif 0 // RAYMATH
    Quaternion result;

    if ((i_mat.element[0] > i_mat.element[5]) && (i_mat.element[0] > i_mat.element[10]))
    {
        float s = sqrtf(1.0f + i_mat.element[0] - i_mat.element[5] - i_mat.element[10])*2.f;

        if (s == 0.f)
            return {0.f, 0.f, 0.f, 1.f};

        result.x = 0.25f*s;
        result.y = (i_mat.element[4] + i_mat.element[1]) / s;
        result.z = (i_mat.element[2] + i_mat.element[8]) / s;
        result.w = (i_mat.element[9] - i_mat.element[6]) / s;
    }
    else if (i_mat.element[5] > i_mat.element[10])
    {
        float s = sqrtf(1.0f + i_mat.element[5] - i_mat.element[0] - i_mat.element[10])*2;

        if (s == 0.f)
            return {0.f, 0.f, 0.f, 1.f};

        result.x = (i_mat.element[4] + i_mat.element[1]) / s;
        result.y = 0.25f*s;
        result.z = (i_mat.element[9] + i_mat.element[6]) / s;
        result.w = (i_mat.element[2] - i_mat.element[8]) / s;
    }
    else
    {
        float s = sqrtf(1.0f + i_mat.element[10] - i_mat.element[0] - i_mat.element[5])*2;

        if (s == 0.f)
            return {0.f, 0.f, 0.f, 1.f};

        result.x = (i_mat.element[2] + i_mat.element[8]) / s;
        result.y = (i_mat.element[9] + i_mat.element[6]) / s;
        result.z = 0.25f*s;
        result.w = (i_mat.element[4] - i_mat.element[1]) / s;
    }

    return result;
#elif 1 // JOLT

    float diagonal = i_mat.element[0] + i_mat.element[5] + i_mat.element[10];

    if (diagonal >= 0.0f)
    {
        float s = Maths::Sqrt(diagonal + 1.0f);
        float sInverse = 0.5f / s;
        return {
                (i_mat.col[2].element[1] - i_mat.col[1].element[2]) * sInverse,
                (i_mat.col[0].element[2] - i_mat.col[2].element[0]) * sInverse,
                (i_mat.col[1].element[0] - i_mat.col[0].element[1]) * sInverse,
                0.5f * s
        };
    }
    else
    {
        int situationIndex = 0;

        if (i_mat.col[1].element[1] > i_mat.col[0].element[0])
            situationIndex = 1;
        if (i_mat.col[2].element[2] > i_mat.col[situationIndex].element[situationIndex])
            situationIndex = 2;

        if (situationIndex == 0)
        {
            float s = Maths::Sqrt(i_mat.col[0].element[0] - (i_mat.col[1].element[1] + i_mat.col[2].element[2]) + 1);
            float sInverse = 0.5f / s;
            return {
                    0.5f * s,
                    (i_mat.col[0].element[1] + i_mat.col[1].element[0]) * sInverse,
                    (i_mat.col[2].element[0] + i_mat.col[0].element[2]) * sInverse,
                    (i_mat.col[2].element[1] - i_mat.col[1].element[2]) * sInverse
            };
        }
        else if (situationIndex == 1)
        {
            float s = Maths::Sqrt(i_mat.col[1].element[1] - (i_mat.col[2].element[2] + i_mat.col[0].element[0]) + 1);
            float sInverse = 0.5f / s;
            return {
                    (i_mat.col[0].element[1] + i_mat.col[1].element[0]) * sInverse,
                    0.5f * s,
                    (i_mat.col[1].element[2] + i_mat.col[2].element[1]) * sInverse,
                    (i_mat.col[0].element[2] - i_mat.col[2].element[0]) * sInverse
            };
        }
        else
        {
            float s = Maths::Sqrt(i_mat.col[2].element[2] - (i_mat.col[0].element[0] + i_mat.col[1].element[1]) + 1);
            float sInverse = 0.5f / s;
            return {
                    (i_mat.col[0].element[2] + i_mat.col[2].element[0]) * sInverse,
                    (i_mat.col[2].element[1] + i_mat.col[1].element[2]) * sInverse,
                    0.5f * s,
                    (i_mat.col[0].element[1] - i_mat.col[1].element[0]) * sInverse
            };
        }
    }
#endif
}

inline Quaternion Quaternion::QuatFromEuler(const float& i_yaw, const float& i_pitch, const float& i_roll)
{
    float cosYaw = Maths::Cos(i_yaw * 0.5f);
    float sinYaw = Maths::Sin(i_yaw * 0.5f);
    float cosPitch = Maths::Cos(i_pitch * 0.5f);
    float sinPitch = Maths::Sin(i_pitch * 0.5f);
    float cosRoll = Maths::Cos(i_roll * 0.5f);
    float sinRoll = Maths::Sin(i_roll * 0.5f);

    return {
            sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll,
            sinYaw * cosPitch * sinRoll + cosYaw * sinPitch * cosRoll,
            cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll,
            cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll
    };
}

inline Quaternion Quaternion::QuatFromEuler(const Vector3& i_eulerRadAngles)
{
    return QuatFromEuler(i_eulerRadAngles.x, i_eulerRadAngles.y, i_eulerRadAngles.z);
}

inline Vector3 Quaternion::QuatToEuler() const
{
    Vector3 result = Vector3::zero;

    // Yaw (x-axis rotation)
    float x0 = 2.0f * (w * x + y * z);
    float x1 = 1.0f - 2.0f * (x * x + y * y);
    result.x = Maths::Atan2(x0, x1);

    // Pitch (y-axis rotation)
    float y0 = 2.0f * (w * y - z * x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = Maths::Asin(y0);

    // Roll (z-axis rotation)
    float z0 = 2.0f * (w * z + x * y);
    float z1 = 1.0f - 2.0f * (y * y + z * z);
    result.z = Maths::Atan2(z0, z1);

    return result;
}

inline Quaternion Quaternion::QuatFromAxisAngle(const Vector3& i_axis, float i_radAngle)
{
    Quaternion result = Identity();

    if (i_axis.Length() != 0.0f)
        i_radAngle *= 0.5f;

    Vector3 newAxis = i_axis.Normalize();

    float cosres = Maths::Cos(i_radAngle);
    float sinres = Maths::Sin(i_radAngle);

    result.x = newAxis.x * sinres;
    result.y = newAxis.y * sinres;
    result.z = newAxis.z * sinres;
    result.w = cosres;

    return result.Normalize();
}

inline void Quaternion::QuatToAxisAngle(Vector3& o_outAxis, float& o_outAngle)
{
    if (Maths::Abs(w) > 1.0f)
        *this = this->Normalize();

    Vector3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f * Maths::Acos(w);
    float den = Maths::Sqrt(1.0f - w * w);

    if (den > 0.0001f)
    {
        resAxis.x = x / den;
        resAxis.y = y / den;
        resAxis.z = z / den;
    }
    else
    {
        resAxis.x = x;
        resAxis.y = y;
        resAxis.z = z;
    }

    o_outAxis = resAxis;
    o_outAngle = resAngle;
}

inline Quaternion Quaternion::VectorToVector(const Vector3& i_from, const Vector3& i_to)
{
    Quaternion result = { 0.f, 0.f, 0.f, 0.f};

    float dot = Vector3::DotProduct(i_from, i_to);
    Vector3 cross = Vector3::CrossProduct(i_from, i_to);

    result.x = cross.x;
    result.y = cross.y;
    result.z = cross.z;
    result.w = 1.0f + dot;

    return result.Normalize();
}

inline Quaternion Quaternion::Lerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);
    return {
            i_q1.x + i_factor * (i_q2.x - i_q1.x),
            i_q1.y + i_factor * (i_q2.y - i_q1.y),
            i_q1.z + i_factor * (i_q2.z - i_q1.z),
            i_q1.w + i_factor * (i_q2.w - i_q1.w)
    };
}

inline Quaternion Quaternion::Nlerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);

    Quaternion result = Lerp(i_q1, i_q2, i_factor);

    return result.Normalize();
}

inline Quaternion Quaternion::Slerp(const Quaternion& i_q1, const Quaternion& i_q2, float i_factor)
{
#if false
    i_factor = Maths::Clamp(i_factor, 0.f, 1.f);

    Quaternion result = Identity();

    float cosHalfTheta = i_q1.x * i_q2.x + i_q1.y * i_q2.y + i_q1.z * i_q2.z + i_q1.w * i_q2.w;

    if (Maths::Abs(cosHalfTheta) >= 1.0f)
        result = i_q1;
    else if (cosHalfTheta > 0.95f)
        result = Nlerp(i_q1, i_q2, i_factor);
    else
    {
        float halfTheta = Maths::Acos(cosHalfTheta);
        float sinHalfTheta = Maths::Sqrt(1.0f - cosHalfTheta * cosHalfTheta);

        if (Maths::Abs(sinHalfTheta) < 0.001f)
        {
            result.x = (i_q1.x * 0.5f + i_q2.x * 0.5f);
            result.y = (i_q1.y * 0.5f + i_q2.y * 0.5f);
            result.z = (i_q1.z * 0.5f + i_q2.z * 0.5f);
            result.w = (i_q1.w * 0.5f + i_q2.w * 0.5f);
        }
        else
        {
            float ratioA = Maths::Sin((1 - i_factor) * halfTheta) / sinHalfTheta;
            float ratioB = Maths::Sin(i_factor * halfTheta) / sinHalfTheta;

            result.x = (i_q1.x * ratioA + i_q2.x * ratioB);
            result.y = (i_q1.y * ratioA + i_q2.y * ratioB);
            result.z = (i_q1.z * ratioA + i_q2.z * ratioB);
            result.w = (i_q1.w * ratioA + i_q2.w * ratioB);
        }
    }

    return result;
#else
    float cosHalfOmega = i_q1.x * i_q2.x + i_q1.y * i_q2.y + i_q1.z * i_q2.z + i_q1.w * i_q2.w;

    Quaternion tto = i_q2;
    if (cosHalfOmega < 0.f)
    {
        tto = { -tto.x, -tto.y, -tto.z, -tto.w };
        cosHalfOmega = -cosHalfOmega;
    }

    if (Maths::Abs(cosHalfOmega) >= 1.0f)
    {
        return i_q1;
    }
    else
    {
        float halfOmega = Maths::Acos(cosHalfOmega);
        float sinHalfOmega = Maths::Sqrt(1.f - cosHalfOmega * cosHalfOmega);

        float ratioA = (Maths::Sin((1.f - i_factor) * halfOmega) / sinHalfOmega);
        float ratioB = (Maths::Sin(i_factor * halfOmega) / sinHalfOmega);
        return { (i_q1.x * ratioA + tto.x * ratioB),
            (i_q1.y * ratioA + tto.y * ratioB),
            (i_q1.z * ratioA + tto.z * ratioB),
            (i_q1.w * ratioA + tto.w * ratioB) };
    }
#endif
}
