#pragma once

#include "maths.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Camera)

		float near = 1.f;
		float far = 1000.f;

		//vertical field of view in degrees, you should convert in radians for calcul
		float fovY = 90.f;

		/**
		* @Summary Get camera's view matrix.
		*/
		Matrix4 GetViewMatrix() const;

		/**
		 * @Summary Get camera's projection matrix.
		 * @param i_aspectRatio
		 */
		Matrix4 GetProjectionMatrix(float i_aspectRatio) const;

		/**
		 * @Summary Get camera's projection matrix from a part of the frustum (specified near and far values).
		 * @param i_aspectRatio
		 * @param i_near
		 * @param i_far
		 */
		Matrix4 GetCascadeProjectionMatrix(float i_aspectRatio, float i_near, float i_far) const;

    KK_COMPONENT_END
}