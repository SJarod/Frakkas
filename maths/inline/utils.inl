#include "maths/vector3.hpp"
#include "maths/vector4.hpp"

#include "maths/constants.hpp"

namespace Maths {

    inline float ToRadians(float i_degAngle)
    {
        return i_degAngle * (Constants::pi / 180.f);
    }

    inline Vector3 ToRadians(const Vector3& i_degAngles)
    {
        return i_degAngles * (Constants::pi / 180.f);
    }

    inline float ToDegrees(float i_radAngle)
    {
        return i_radAngle * (180.f / Constants::pi);
    }

    inline Vector3 ToDegrees(const Vector3& i_radAngles)
    {
        return i_radAngles * (180.f / Constants::pi);
    }

    template<typename T>
    inline T Min(T i_a, T i_b)
    {
        return i_a < i_b ? i_a : i_b;
    }

    template<typename T>
    inline T Max(T i_a, T i_b)
    {
        return i_a > i_b ? i_a : i_b;
    }

    template<typename T>
    inline T Min(T i_a, T i_b, T i_c)
    {
        return Min(Min(i_a, i_b), i_c);
    }

    template<typename T>
    inline T Max(T i_a, T i_b, T i_c)
    {
        return Max(Max(i_a, i_b), i_c);
    }

    template<typename T>
    inline T Clamp(T i_val, T i_min, T i_max)
    {
        return Max(Min(i_val, i_max), i_min);
    }

    inline Vector3 Clamp(Vector3 i_val, float i_min, float i_max)
    {
        return Vector3(Clamp(i_val.x, i_min, i_max),
                       Clamp(i_val.y, i_min, i_max),
                       Clamp(i_val.z, i_min, i_max));
    }

    inline Vector4 Clamp(Vector4 i_val, float i_min, float i_max)
    {
        return Vector4(Clamp(i_val.x, i_min, i_max),
                       Clamp(i_val.y, i_min, i_max),
                       Clamp(i_val.z, i_min, i_max),
                       Clamp(i_val.w, i_min, i_max));
    }

    inline int Modulo(int i_val, int i_base)
    {
        return i_val % i_base;
    }

    inline float Modulo(float i_val, float i_base)
    {
        return fmod(i_val, i_base);
    }

    inline Vector3 Modulo(const Vector3& i_val, float i_base)
    {
        return {
                Modulo(i_val.x, i_base),
                Modulo(i_val.y, i_base),
                Modulo(i_val.z, i_base)
        };
    }

    inline float Abs(float i_f)
    {
        return fabs(i_f);
    }

    inline float Sqrt(float i_f)
    {
        return sqrtf(i_f);
    }

    inline float Square(float i_f)
    {
        return i_f * i_f;
    }

    inline float Cos(float i_f)
    {
        return cosf(i_f);
    }

    inline float Acos(float i_f)
    {
        return acosf(i_f);
    }

    inline float Sin(float i_f)
    {
        return sinf(i_f);
    }

    inline float Asin(float i_f)
    {
        return asinf(i_f);
    }

    inline float Tan(float i_f)
    {
        return tanf(i_f);
    }

    inline float Atan(float i_f)
    {
        return atanf(i_f);
    }

    inline float Atan2(float i_f1, float i_f2)
    {
        return atan2(i_f1, i_f2);
    }

    template<typename T>
    inline T Lerp(T i_start, T i_reach, float i_factor)
    {
        i_factor = Clamp(i_factor, -1.f, 1.f);
        return i_start + i_factor * (i_reach - i_start);
    }

    template<typename T>
    inline int Sign(T i_val)
    {
        return (T(0) < i_val) - (i_val < T(0));
    }
}
