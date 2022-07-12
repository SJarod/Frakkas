#pragma once

#include <string>
#include <atomic>

namespace Resources
{
	enum class EResourceType
	{
		UNDEFINED,
		SHADER,
		MESH,
		SKELETALMESH,
		TEXTURE,
		ANIMPACK,
        SOUND
	};

	class Resource
	{
	public:
		virtual ~Resource() = default;

		// Resource name or path
		std::string name;
		EResourceType resourceType = EResourceType::UNDEFINED;

		// The resource's RAM size
		size_t ram = 0;

		// The resource's VRAM size
		size_t vram = 0;

		std::atomic_flag loaded = ATOMIC_FLAG_INIT;

		/**
		 * Some resource needs to be loaded depending on some parameters.
		 * 
		 * @return true if this resource's dependencies are available.
		 */
		virtual bool DependenciesReady() = 0;

		/**
		 * Load the resource from each resource specific info (name, ...).
		 * CPU side load.
		 * 
		 * @return success
		 */
		virtual bool CPULoad() = 0;

		/**
		 * Load the resource for the graphics API.
		 * GPU side load.
		 * 
		 * @return success
		 */
		virtual bool GPULoad() = 0;

		/**
		 * Unload the resource, free memory allocations.
		 * CPU side unload.
		 * 
		 * @return success
		 */
		virtual bool CPUUnload() = 0;

		/**
		 * Unload the resource, free memory allocations.
		 * GPU side unload.
		 *
		 * @return success
		 */
		virtual bool GPUUnload() = 0;
	};
}