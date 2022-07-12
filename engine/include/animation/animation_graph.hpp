#pragma once

#include <memory>
#include <list>
#include <unordered_map>

#include "resources/skeletal_animation.hpp"
#include "animation/animator.hpp"

namespace Resources
{
	class SkeletalMesh;
}

namespace Animation
{
	class AnimationGraph
	{
	private:
		std::list<std::weak_ptr<Resources::SkeletalAnimationPack>> skpacks;

		// list of every loaded animation files' name
		std::list<std::string> skfiles;

		std::weak_ptr<Resources::SkeletalMesh> owningSkmesh;

	public:
		Animation::Animator player;

		/**
		 * Set the skeletal mesh target of the animator.
		 *
		 * @param i_owningSkmesh
		 */
		void SetOwningSkeletalMesh(const std::shared_ptr<Resources::SkeletalMesh> i_owningSkmesh);

		/**
		 * Add an animation pack in the list of packs.
		 */
		void AddAnimationPack(const std::string& i_packName);

		/**
		 * Remove an animation pack given one of its animation name.
		 */
		void FindPackAndRemove(const std::string& i_animationName);

		/**
		 * Get the name of every contained animations (within the packs).
		 */
		void GetAnimationsName(std::vector<std::string_view>& o_names) const;

		/**
		 * Get the file name of every contained animation packs.
		 */
		void GetFilesName(std::vector<std::string_view>& o_names) const;

		/**
		 * Tell the animator to play an animation given its name.
		 * 
		 * @param i_animationName
		 * @param i_loop
		 */
		bool PlayAnimation(const std::string& i_animationName, const bool i_loop = false);
	};
}