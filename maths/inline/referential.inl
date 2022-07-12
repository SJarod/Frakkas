#include "vector3.hpp"
#include "quaternion.hpp"

////////////////////////////// CONSTRUCTORS

inline Referential::Referential()
{
    origin = Vector3(0.f, 0.f, 0.f);

    i = Vector3(1.f, 0.f, 0.f);
    j = Vector3(0.f, 1.f, 0.f);
    k = Vector3(0.f, 0.f, 1.f);

    angle = 0.f;
}

inline Referential::Referential(const Vector3& _origin, const Vector3& _i, const Vector3& _j, const Vector3& _k)
{
    origin = _origin;

    i = _i;
    j = _j;
    k = _k;

    angle = 0.f;
}

inline Referential::Referential(const Vector3& _origin, const Quaternion& quat)
{
    origin = _origin;

    i.RotateByQuat(Vector3(1.f, 0.f, 0.f), quat);
    j.RotateByQuat(Vector3(0.f, 1.f, 0.f), quat);
    k.RotateByQuat(Vector3( 0.f, 0.f, 1.f), quat);

    angle = quat.w;
}

inline Referential::Referential(const Vector3& ptA, const Vector3& ptB)
{
    angle = 0.f;

    Vector3 axe = ptB - ptA;

    origin = ptA + (axe / 2.f);
    j = axe.Normalize();

    Vector3 notJ = { 1.f, 0.f, 0.f };

    if (j == notJ)
        notJ = { 0.f, 1.f, 0.f };

    k = (j ^ notJ).Normalize();
    i = (k ^ j).Normalize();
}

////////////////////////////// OPERATORS

inline std::ostream& operator<<(std::ostream& o, const Referential& ref)
{
    return o << "origin : " << ref.origin << '\n'
             << "i : " << ref.i << '\n'
             << "j : " << ref.j << '\n'
             << "k : " << ref.k << '\n'
             << "angle = " << ref.angle;
}

////////////////////////////// FUNCTIONS

inline Vector3 Referential::PosLocalToGlobal(const Vector3& i_localPos) const
{
    return origin + (i * i_localPos.x) + (j * i_localPos.y) + (k * i_localPos.z);
}

inline Vector3 Referential::PosGlobalToLocal(const Vector3& i_globalPos) const
{
    Vector3 originToPos = i_globalPos - origin;

    return {
            Vector3::DotProduct(originToPos, i),
            Vector3::DotProduct(originToPos, j),
            Vector3::DotProduct(originToPos, k)
    };
}

inline Vector3 Referential::VectLocalToGlobal(const Vector3& i_localVect) const
{
    return (i * i_localVect.x) + (j * i_localVect.y) + (k * i_localVect.z);
}

inline Vector3 Referential::VectGlobalToLocal(const Vector3& i_globalVect) const
{
    return {
            Vector3::DotProduct(i_globalVect, i),
            Vector3::DotProduct(i_globalVect, j),
            Vector3::DotProduct(i_globalVect, k)
    };
}