//
// Created by m.mehalin on 23/02/2022.
//

#pragma once

struct Vector3;
struct Quaternion;

struct Referential {
    Vector3 origin;
    Vector3 i, j, k;
    float angle;

////////////////////////////// CONSTRUCTORS

    Referential();
    Referential(const Vector3& i_origin, const Quaternion& i_quat);
    Referential(const Vector3& i_ptA, const Vector3& i_ptB);
    Referential(const Vector3& i_origin, const Vector3& i_i, const Vector3& i_j, const Vector3& i_k);

////////////////////////////// FUNCTIONS

/**
 * @summary Convert position from local to global referential
 * @return global position of the input position
 * @warning this method consider a position, not a translation vector
 */
    Vector3 PosLocalToGlobal(const Vector3& i_localPos) const;

/**
 * @summary Convert position from global to local referential
 * @return local position of the input position
 * @warning this method consider a position, not a translation vector
 */
    Vector3 PosGlobalToLocal(const Vector3& i_globalPos) const;

/**
 * @summary Convert vector from local to global referential
 * @return global vector of the input vector
 * @warning this method consider a translation vector, not a position
 */
    Vector3 VectLocalToGlobal(const Vector3& i_localVector) const;

/**
 * @summary Convert vector from global to local referential
 * @return local vector of the input vector
 * @warning this method consider a translation vector, not a position
 */
    Vector3 VectGlobalToLocal(const Vector3& i_globalVector) const;

};

std::ostream& operator<<(std::ostream& o_o, const Referential& i_ref);

#include "referential.inl"