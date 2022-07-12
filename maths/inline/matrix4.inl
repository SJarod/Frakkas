#include "maths/vector3.hpp"
#include "maths/vector4.hpp"
#include "maths/utils.hpp"
#include "maths/matrix4.hpp"


////////////////////////////// CONSTRUCTORS

inline Matrix4::Matrix4()
    : element{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }
{}
inline Matrix4::Matrix4(const float& i_f0, const float& i_f1, const float& i_f2, const float& i_f3,
    const float& i_f4, const float& i_f5, const float& i_f6, const float& i_f7,
    const float& i_f8, const float& i_f9, const float& i_f10, const float& i_f11,
    const float& i_f12, const float& i_f13, const float& i_f14, const float& i_f15)
    : element{ i_f0, i_f1, i_f2, i_f3,
        i_f4, i_f5, i_f6, i_f7,
        i_f8, i_f9, i_f10, i_f11,
        i_f12, i_f13, i_f14, i_f15 }
{}

inline Matrix4::Matrix4(const Vector4& i_v0, const Vector4& i_v1, const Vector4& i_v2, const Vector4& i_v3)
    : col{i_v0, i_v1, i_v2, i_v3}
{}

////////////////////////////// OPERATORS

inline Vector3 Matrix4::operator*(const Vector3& vec) const
{
    const float w = element[3] * vec.x + element[7] * vec.y + element[11] * vec.z + element[15];

    return {
            (element[0] * vec.x + element[4] * vec.y + element[8] * vec.z + element[12]) / w,
            (element[1] * vec.x + element[5] * vec.y + element[9] * vec.z + element[13]) / w,
            (element[2] * vec.x + element[6] * vec.y + element[10] * vec.z + element[14]) / w
    };
}

inline Vector4 Matrix4::operator*(const Vector4& vec) const
{
    Vector4 result = { 0.f, 0.f, 0.f, 0.f };

    for (int column = 0; column < 4; column++)
    {
        for (int line = 0; line < 4; line++)
        {
            result.element[column] += *(&this->col[line].x + column) * vec.element[line];
        }
    }

    return result;
}

inline Matrix4 Matrix4::operator*(const Matrix4& mat) const
{
    Matrix4 result = {};

    for(int c=0; c < 4; c++)
    {
        for(int r=0; r < 4; r++)
        {
            for(int k=0;k<4;k++)
            {
                result.col[c].element[r]+= col[c].element[k] * mat.col[k].element[r];
            }
        }
    }

    return result;
}

inline bool Matrix4::operator==(const Matrix4& mat) const
{
    bool result = true;
    for(int i = 0; i < 16 && result; i++)
        result = Maths::Abs(element[i] - mat.element[i]) <= Maths::Constants::weakEpsilon;

    return result;
}


inline std::ostream& operator<<(std::ostream& o, const Matrix4& mat)
{
    return o << mat.element[0] << ", " << mat.element[1] << ", " << mat.element[2] << ", " << mat.element[3] << ",\n"
             << mat.element[4] << ", " << mat.element[5] << ", " << mat.element[6] << ", " << mat.element[7] << ",\n"
             << mat.element[8] << ", " << mat.element[9] << ", " << mat.element[10] << ", " << mat.element[11] << ",\n"
             << mat.element[12] << ", " << mat.element[13] << ", " << mat.element[14] << ", " << mat.element[15] << "\n";
}

////////////////////////////// FUNCTIONS

inline Matrix4 Matrix4::Identity()
{
    return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
    };
}

inline Matrix4 Matrix4::Null()
{
    return {
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f,
            0.f, 0.f, 0.f, 0.f
    };
}

inline Matrix4 Matrix4::Transpose() const
{
    return {
            element[0], element[4], element[8], element[12],
            element[1], element[5], element[9], element[13],
            element[2], element[6], element[10], element[14],
            element[3], element[7], element[11], element[15]
    };
}

inline Matrix4 Matrix4::OrthoNormalize() const
{
    return Matrix4(
        col[0].Normalize(),
        col[1].Normalize(),
        col[2].Normalize(),
        col[3]
    );
}

inline Matrix4 Matrix4::Inverse() const
{
    Matrix4 result;

    // Cache the matrix values (speed optimization)
    float a00 = element[0], a01 = element[1], a02 = element[2], a03 = element[3];
    float a10 = element[4], a11 = element[5], a12 = element[6], a13 = element[7];
    float a20 = element[8], a21 = element[9], a22 = element[10], a23 = element[11];
    float a30 = element[12], a31 = element[13], a32 = element[14], a33 = element[15];

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

    result.element[0] = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
    result.element[1] = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
    result.element[2] = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
    result.element[3] = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
    result.element[4] = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
    result.element[5] = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
    result.element[6] = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
    result.element[7] = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;
    result.element[8] = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
    result.element[9] = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
    result.element[10] = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
    result.element[11] = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
    result.element[12] = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
    result.element[13] = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
    result.element[14] = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
    result.element[15] = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

    return result;
}

inline Matrix4 Matrix4::Frustum(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far)
{
    float rightMinusLeft = i_right - i_left;
    float topMinusBottom = i_top - i_bottom;
    float farMinusNear = i_far - i_near;

    return {
            (2.f * i_near) / rightMinusLeft,      0.f,                                  0.f,                                     0.f,
            0.f,                                  (2.f * i_near) / topMinusBottom,      0.f,                                     0.f,
            (i_right + i_left) / rightMinusLeft,  (i_top + i_bottom) / topMinusBottom,  -(i_far + i_near) / farMinusNear,        -1.f,
            0.f,                                  0.f,                                  -(i_far * i_near * 2.f) / farMinusNear,  0.f
    };
}

inline Matrix4 Matrix4::Orthographic(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far)
{
    float rightMinusLeft = i_right - i_left;
    float topMinusBottom = i_top - i_bottom;
    float farMinusNear = i_far - i_near;

    return
            {
                    2.f / rightMinusLeft,                 0.f,                                   0.f,                               0.f,
                    0.f,                                  2.f / topMinusBottom,                  0.f,                               0.f,
                    0.f,                                  0.f,                                   -2 / farMinusNear,                 0.f,
                    -(i_right + i_left) / rightMinusLeft, -(i_top + i_bottom) / topMinusBottom,  -(i_near + i_far) / farMinusNear,  1.f
            };
}

inline Matrix4 Matrix4::Perspective(float fovY, float aspect, float near, float far, bool ortho)
{
    float top = near * Maths::Tan(fovY * 0.5f);

    float right = top * aspect;
    float bottom = -top, left = -right;

    if (ortho)
        return Orthographic(left, right, bottom, top, near, far);
    else
        return Frustum(left, right, bottom, top, near, far);
}

inline Matrix4 Matrix4::RotateX(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            1.f, 0.f,  0.f, 0.f,
            0.f, cos, -sin, 0.f,
            0.f, sin,  cos, 0.f,
            0.f, 0.f,  0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateY(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            cos,  0.f, sin, 0.f,
            0.f,  1.f, 0.f, 0.f,
            -sin, 0.f, cos, 0.f,
            0.f,  0.f, 0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateZ(const float& i_angleRadians)
{
    float cos = Maths::Cos(i_angleRadians), sin = Maths::Sin(i_angleRadians);

    return {
            cos, -sin, 0.f, 0.f,
            sin, cos,  0.f, 0.f,
            0.f, 0.f,  1.f, 0.f,
            0.f, 0.f,  0.f, 1.f
    };
}

inline Matrix4 Matrix4::RotateXYZ(float i_xRadAngle, float i_yRadAngle, float i_zRadAngle)
{
    return  RotateZ(i_zRadAngle) * RotateY(i_yRadAngle) * RotateX(i_xRadAngle);
}

inline Matrix4 Matrix4::RotateXYZ(const Vector3& i_anglesRadians)
{
    return  RotateXYZ(i_anglesRadians.x,i_anglesRadians.y,i_anglesRadians.z);
}

inline Matrix4 Matrix4::MatrixFromQuat(const Quaternion& i_q)
{
#if 0
    float q0 = i_q.w, q1 = i_q.x, q2 = i_q.y, q3 = i_q.z;
    return Matrix4{
        2.f * (q0 * q0 + q1 * q1) - 1.f, 2.f * (q1 * q2 - q0 * q3), 2.f * (q1 * q3 + q0 * q2), 0.f,
        2.f * (q1 * q2 + q0 * q3), 2.f * (q0 * q0 + q2 * q2) - 1.f, 2.f * (q2 * q3 - q0 * q1), 0.f,
        2.f * (q1 * q3 - q0 * q2), 2.f * (q2 * q3 + q0 * q1), 2.f * (q0 * q0 + q3 * q3) - 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f }.Transpose();
#else
    float dX = 2.f * i_q.x;
    float dY = 2.f * i_q.y;
    float dZ = 2.f * i_q.z;
    float sqX = dX * i_q.x;
    float sqY = dY * i_q.y;
    float sqZ = dZ * i_q.z;

    float qXY = dX * i_q.y;
    float qXZ = dX * i_q.z;
    float qYZ = dY * i_q.z;
    float qXW = dX * i_q.w;
    float qYW = dY * i_q.w;
    float qZW = dZ * i_q.w;

    return {
        1.f - sqY - sqZ,    qXY + qZW,          qXZ - qYW,          0.f,
        qXY - qZW,          1.f - sqX - sqZ,    qYZ + qXW,          0.f,
        qXZ + qYW,          qYZ - qXW,          1.f - sqX - sqY,    0.f,
        0.f,                0.f,                0.f,                1.f
    };
#endif
}

inline Matrix4 Matrix4::Translate(float i_x, float i_y, float i_z)
{
    return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            i_x, i_y, i_z, 1.f
    };
}

inline Matrix4 Matrix4::Translate(const Vector3& i_vec)
{
    return Translate(i_vec.x,i_vec.y,i_vec.z);
}

inline Matrix4 Matrix4::Scale(const float& i_sc)
{
    return {
            i_sc, 0.f,  0.f,  0.f,
            0.f,  i_sc, 0.f,  0.f,
            0.f,  0.f,  i_sc, 0.f,
            0.f,  0.f,  0.f,  1.f
    };
}

inline Matrix4 Matrix4::Scale(const Vector3& i_sc)
{
    return {
            i_sc.x,  0.f,     0.f,     0.f,
            0.f,     i_sc.y,  0.f,     0.f,
            0.f,     0.f,     i_sc.z,  0.f,
            0.f,     0.f,     0.f,     1.f
    };
}

inline Matrix4 Matrix4::LookAt(const Vector3& i_eye, const Vector3& i_center, const Vector3& i_up)
{
    Vector3 Z = (i_center - i_eye).Normalize();
    Vector3 X = Vector3::CrossProduct(Z, i_up).Normalize();
    Vector3 Y = Vector3::CrossProduct(X, Z);

    return Matrix4{
        X.x, Y.x, -Z.x, 0.f,
        X.y, Y.y, -Z.y, 0.f,
        X.z, Y.z, -Z.z, 0.f,
        -Vector3::DotProduct(X, i_eye), -Vector3::DotProduct(Y, i_eye), Vector3::DotProduct(Z, i_eye), 1.f
    };
}

inline Vector3 Matrix4::DecomposeTranslation() const
{
    return col[3];
}

inline Vector3 Matrix4::DecomposeRotation() const
{
    return Quaternion::QuatFromMatrix(*this).Normalize().QuatToEuler();
}

inline Vector3 Matrix4::DecomposeScale() const
{
    return {col[0].Length(), col[1].Length(), col[2].Length()};
}
