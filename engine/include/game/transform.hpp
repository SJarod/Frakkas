#pragma once

#include <list>

#include "utils/properties.hpp"
#include "maths.hpp"

namespace Resources
{
    class Serializer;
}

namespace Game
{
    /**
     * @brief ScaleLockParams is a struct containing parameters about transform scale when scale is locked.
     * In this case, the scale vector must keep the same direction.
     * It is used only for editing, so this structure is a bit odd but necessary.
     * @param isLock indicate if scale is locked
     * @param ratio the ratio between the current coordinate value and the origin coordinate value
     * @param origScale the origin scale coordinates values. Useful for ratio computing
     */
    struct ScaleLockParams
    {
        bool isLock = true;
        float ratio = 1.f;
        Vector3 origScale = {1.f, 1.f, 1.f };
    };

	struct Transform
	{
	public:
        Transform();
        Transform(const Transform& i_copy);

        Property<Vector3> position;
        // Rotation angles in radians. x : yaw, y : pitch, z : roll
        Property<Vector3> rotation;
        Property<Vector3> scale;

        Property<Transform*> parent;

        // Store the number of collider attached to this transform, so that rotation can't be updated due to Physics issues.
        int colliderComponentCount = 0;
        
        /**
         * @return A rotation quaternion from transform's vector3 rotation.
         */
        Quaternion GetQuaternionRotation() const;

		/**
		 * @brief Get a model matrix from a transform. If model matrix not update, compute the matrix.
		 * @return Matrix4 model
		 */
		Matrix4 GetWorldMatrix() const;

        void RemoveChild(Transform* childToRemove);

        /**
         * Remove all child. Put their parent to nullptr.
         */
        void ClearChilds();

        /**
         * @return the ScaleLockParameters struct of this transform
         */
        ScaleLockParams& GetScaleLockParameters() { return scaleLockParams; }

    private:
        mutable bool needUpdate = true;
        mutable bool needUniformUpdate = true;

        ScaleLockParams scaleLockParams;

        mutable Matrix4 worldMatrix = Matrix4::Identity();

        std::list<Transform*> childs;

        /**
         * Compute the model matrix using position, rotation and scale
         */
        void UpdateWorldMatrix() const;
    };
}
