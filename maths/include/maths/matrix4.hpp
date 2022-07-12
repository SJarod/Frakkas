#pragma once

struct Vector3;
struct Vector4;

struct Matrix4
{
    union
    {
        float element[16] = {0.f};

        Vector4 col[4];
    };

////////////////////////////// CONSTRUCTORS

    Matrix4();
    Matrix4(const float& i_f0, const float& i_f1, const float& i_f2, const float& i_f3,
        const float& i_f4, const float& i_f5, const float& i_f6, const float& i_f7,
        const float& i_f8, const float& i_f9, const float& i_f10, const float& i_f11,
        const float& i_f12, const float& i_f13, const float& i_f14, const float& i_f15);
    Matrix4(const Vector4& i_v0,const Vector4& i_v1,const Vector4& i_v2,const Vector4& i_v3);

////////////////////////////// OPERATORS

    Vector3 operator*(const Vector3& i_vec) const;
    Vector4 operator*(const Vector4& i_vec) const;
    Matrix4 operator*(const Matrix4& i_mat) const;

    bool operator==(const Matrix4& i_mat) const;

////////////////////////////// FUNCTIONS

/**
 * @summary identity is the '1' element of matrix space.
 * It does not affect multiplication
 * @return matrix identity
 */
    static Matrix4 Identity();

 /**
  * @return null matrix, with sixteen 0
  */
    static Matrix4 Null();

/**
 * @summary The transposed matrix is the one that has the Matrix4x4's columns exchanged with its rows.
 * @return the transpose of this matrix
 */
    Matrix4 Transpose() const;

    /**
     * @summary Normalize the 3 first column of the Matrix
     * @return The normalized matrix.
     */
    Matrix4 OrthoNormalize() const;

/**
 * @summary Compute the inverse matrix.
 * Inverted matrix is such that if multiplied by the original would result in identity matrix
 * @return the inverse of this matrix
 */
    Matrix4 Inverse() const;

    /**
     * @summary Extract a position vector from the matrix.
     * The matrix should be a World Matrix.
     */
    Vector3 DecomposeTranslation() const;

    /**
     * @brief Extract a radians-euler angles vector from the matrix. Matrix should be ortho-normalized.
     * Use QuatfromMatrix instead to have Quaternion rotation.
     */
    Vector3 DecomposeRotation() const;

    /**
     * @breif Extract a scale vector from the matrix.
     * The matrix should be a World matrix.
     */
    Vector3 DecomposeScale() const;

/**
 * @summary Calculate 3D world space frustum matrix
 * @param i_left Frustum's left view plane
 * @param i_right Frustum's right view plane
 * @param i_bottom Frustum's bottom view plane
 * @param i_top Frustum's top view plane
 * @param i_near Nearest value to draw objects
 * @param i_far Further value to draw objects
 */
    static Matrix4 Frustum(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far);

/**
 * @summary Calculate 3D world space orthographic matrix,
 * @param i_left Orthographic's left view plane
 * @param i_right Orthographic's right view plane
 * @param i_bottom Orthographic's bottom view plane
 * @param i_top Orthographic's top view plane
 * @param i_near Nearest value to draw objects
 * @param i_far Further value to draw objects
 * */
    static Matrix4 Orthographic(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far);

/**
 * @summary Calculate 3D world space perspective matrix
 * @param i_fovY Field of view IN RADIANS
 * @param i_aspect Ratio width/height of the screen
 * @param i_near Nearest value to draw objects
 * @param i_far Further value to draw objects
 * @param i_ortho If true, return Orthographic matrix, else return Frustrum
 */
    static Matrix4 Perspective(float i_fovY, float i_aspect, float i_near, float i_far, bool i_ortho);

/**
 * @summary Create a rotation matrix in X-axis
 */
    static Matrix4 RotateX(const float& i_angleRadians);

/**
 * @summary Create a rotation matrix in Y-axis
 */
    static Matrix4 RotateY(const float& i_angleRadians);

/**
 * @summary Create a rotation matrix in Z-axis
 */
    static Matrix4 RotateZ(const float& i_angleRadians);

    /**
 * @summary Create a rotation matrix, calls RotateY * RotateX * RotateZ
 */
    static Matrix4 RotateXYZ(float i_xRadAngle, float i_yRadAngle, float i_zRadAngle);
/**
 * @summary Create a rotation matrix, calls RotateY * RotateX * RotateZ
 */
    static Matrix4 RotateXYZ(const Vector3& i_anglesRadians);

/**
 * @return a rotation matrix from a quaternion
 * @param i_q must be a normalized quaternion
 */
    static Matrix4 MatrixFromQuat(const Quaternion& i_q);

/**
 * @summary Create a translation matrix
 */
    static Matrix4 Translate(float i_x, float i_y, float i_z);

/**
 * @summary Create a translation matrix
 */
    static Matrix4 Translate(const Vector3& i_vec);

/**
 * @summary Create a scale matrix
 * @param i_sc apply same scale for each coordinates X, Y and Z
 */
    static Matrix4 Scale(const float& i_sc);

/**
 * @summary Create a scale matrix
 */
    static Matrix4 Scale(const Vector3& i_sc);

/**
 * @summary Create a look at matrix.
 */
    static Matrix4 LookAt(const Vector3& i_eye, const Vector3& i_center, const Vector3& i_up);
};

std::ostream& operator<<(std::ostream& o_o, const Matrix4& i_mat);

#include "matrix4.inl"
