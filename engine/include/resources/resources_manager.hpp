#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>

#include "debug/log.hpp"

#include "utils/singleton.hpp"
#include "utils/normalize_filepath.hpp"
#include "multithread/threadpool.hpp"

#include "resources/mesh.hpp"
#include "resources/texture.hpp"
#include "resources/skeletal_animation.hpp"

#include "resources/resource.hpp"

using namespace Resources;

namespace Resources
{
	class ResourcesManager : public Singleton<ResourcesManager>
	{
		friend class Singleton<ResourcesManager>;

	public:
		const DefaultTexture defaultTexture;

		~ResourcesManager() = default;

		/**
		* Add a resource to the resources map and return it.
		* Return an already existing resource if any.
		*
		* @param i_name : the resource name or the resource path
		*/
		template<class TResource, typename... TExtraParams> requires std::constructible_from<TResource, const std::string, TExtraParams...>
		static std::shared_ptr<TResource> LoadResource(const std::string& i_name, const TExtraParams&... i_params) noexcept;

		/**
		* Get the default texture.
		*/
		static const DefaultTexture& GetDefaultTexture();

		/**
		 * Get a reference to the resources map without options for changing it.
		 */
		static const std::unordered_map<std::string, std::shared_ptr<Resource>>& ViewAllResources();

		/**
		 * Refresh all resources by reloading them.
		 */
		static void Refresh();

		/**
		 * Destroy a specific resource given its name.
		 */
		static void DestroyThisResource(const std::string& i_name);

		/**
		 * Empty the resources manager.
		 */
		static void DestroyResources();

	private:
		// mutex for the resources map
		std::mutex resourceMX;

		// Array of every loaded resource.
		std::unordered_map<std::string, std::shared_ptr<Resource>> resources;

		ResourcesManager() = default;

		/**
		 * Try to load the given resource, postpone the load if failed.
		 */
		inline void TryLoad(std::shared_ptr<Resource> i_newResource, const std::string& i_name);
	};
}

template<class TResource, typename... TExtraParams> requires std::constructible_from<TResource, const std::string, TExtraParams...>
std::shared_ptr<TResource> ResourcesManager::LoadResource(const std::string& i_name, const TExtraParams&... i_params) noexcept
{
	if (i_name == "")
	{
		Log::Warning("Cannot create unnamed resource");
		return nullptr;
	}

    // Normalize name slash if it is a filepath
    std::string name = i_name;
    Utils::NormalizeFilepath(name);

	std::shared_ptr<TResource> newResource;

	ResourcesManager& rm = Instance();

	{
		std::lock_guard<std::mutex> guard(rm.resourceMX);

		if (rm.resources.find(name.c_str()) != rm.resources.end())
			return std::dynamic_pointer_cast<TResource>(rm.resources[name.c_str()]);

		newResource = std::make_shared<TResource>(name, i_params...);
		rm.resources[name.c_str()] = newResource;
	}

	ThreadPool::AddTask([newResource, name]() {
		ResourcesManager& rm = Instance();
		rm.TryLoad(newResource, name);
		});

	return std::dynamic_pointer_cast<TResource>(newResource);
}

inline void ResourcesManager::TryLoad(std::shared_ptr<Resource> i_newResource, const std::string& i_name)
{
	auto Load = [this, i_newResource, i_name]() {
		if (!i_newResource->CPULoad())
		{
			if (!defaultTexture.Found(i_name))
				Log::Warning(i_name, " is an invalid resource or an incorrectly loaded resource, it will be erased from the resources manager.");
			i_newResource->CPUUnload();
			DestroyThisResource(i_name);
		}
		else
		{
			ThreadPool::AddTask([i_newResource]() {
				i_newResource->GPULoad();
				i_newResource->loaded.test_and_set(std::memory_order_release);
				}, false);
		}
	};

	if (!i_newResource->DependenciesReady())
	{
		ThreadPool::AddTask([this, i_newResource, i_name]() {
			TryLoad(i_newResource, i_name);
			});
	}
	else
		Load();
}