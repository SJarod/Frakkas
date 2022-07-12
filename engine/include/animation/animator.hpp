#pragma once

namespace Resources
{
	struct Bone;
	class SkeletalMesh;

	struct SkeletonNodeData;
	class SkeletalAnimation;
	class SkeletalAnimationPack;
}

using namespace Resources;

namespace Animation
{
	/**
	 * Play a skeletal animation.
	 */
	class Animator
	{
	private:

		std::weak_ptr<Resources::SkeletalMesh> owningSkmesh;

		bool loop = false;
		const SkeletalAnimation* anim = nullptr;
		const std::unordered_map<std::string, Resources::Bone>* animBoneInfo = nullptr;

		// Vector of every bones, stored as global transform matrix
		std::vector<Matrix4> globalBoneMatrices;
		// Vector of every bones, stored as raw transform matrix
		std::vector<Matrix4> rawBoneMatrices;

		/**
		 * Process every bone's matrix to make them global.
		 * Update the values in the boneMatrices vector.
		 * This function is recursive.
		 * 
		 * @param SkeletonNodeData*
		 * @param i_parentTransform
		 */
		void UpdateBoneMatrices(const SkeletonNodeData* i_node, const Matrix4& i_parentTransform = Matrix4::Identity());

		/**
		 * Cancel bone extra translation and scale.
		 * Specify the axis to cancel.
		 * 
		 * @param i_x
		 * @param i_y
		 * @param i_z
		 * @param i_scale
		 */
		void CancelExtraTS(bool i_x = true, bool i_y = true, bool i_z = true, bool i_scale = true);

	public:
        float playSpeed = 1.f;
        float playTime = 0.f;
        
		/**
		 * Create the animator, allocating memory for MAX_BONES matrices.
		 */
		Animator();

		/**
		 * Pass a SkeletalAnimation to this animator and reset the play time.
		 * 
		 * @param SkeletalAnimation* i_anim
		 * @param bool i_loop
		 */
		void UploadAnimation(const SkeletalAnimation* i_anim, bool i_loop = false);

		/**
		 * Set the skeletal mesh target of this animator.
		 * 
		 * @param i_owningSkmesh
		 */
		void SetSkeletalMesh(const std::shared_ptr<Resources::SkeletalMesh> i_owningSkmesh);

		/**
		 * Unload previous animation info.
		 */
		void UnloadAnimation();

		/**
		 * Animator's update, called in the Animated component.
		 * 
		 * @param i_deltaTime
		 */
		void UpdatePlayer(const float& i_deltaTime);

		/**
		 * Is the animator waiting for an animation?
		 * 
		 * @return true if the player is not playing any animation.
		 */
		bool IsWaiting() const;

        /**
         * Is the animator looping an animation ?
         *
         * @return true if the player is looping any animation.
         * @return
         */
        bool IsLooping() const;

		/**
		 * Get the vector of global bone transform matrices.
		 */
		const std::vector<Matrix4>& GetGlobalBoneMatrices() const;

		/**
		 * Get the vector of raw bone transform matrices.
		 */
		const std::vector<Matrix4>& GetRawBoneMatrices() const;
	};
}
