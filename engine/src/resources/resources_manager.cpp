#include <assimp/Importer.hpp>

#include "debug/log.hpp"
#include "maths.hpp"

#include "resources/resources_manager.hpp"

const DefaultTexture& Resources::ResourcesManager::GetDefaultTexture()
{
	return Instance().defaultTexture;
}

const std::unordered_map<std::string, std::shared_ptr<Resource>>& Resources::ResourcesManager::ViewAllResources()
{
	return Instance().resources;
}

void Resources::ResourcesManager::Refresh()
{
	ResourcesManager& rm = Instance();

	for (auto& resource : rm.resources)
	{
		std::shared_ptr<Resource> ptr = resource.second;
		ThreadPool::AddTask([&, ptr]() {
			ptr->CPUUnload();
			ptr->CPULoad();

			{
				ThreadPool::AddTask([ptr]() {
					ptr->GPUUnload();
					ptr->GPULoad();
					}, false);
			}
			});
	}
}

void Resources::ResourcesManager::DestroyThisResource(const std::string& i_name)
{
	ResourcesManager& rm = Instance();
	std::lock_guard<std::mutex> guard(rm.resourceMX);

	std::string name = i_name;
	rm.resources.erase(name);

	if (!rm.defaultTexture.Found(i_name))
		Log::Info(name, " was deleted");
}

void Resources::ResourcesManager::DestroyResources()
{
	ResourcesManager& rm = Instance();

	ThreadPool::FinishTasks();

	std::lock_guard<std::mutex> guard(rm.resourceMX);

	for (auto& resource : rm.resources)
	{
		resource.second->CPUUnload();
		resource.second->GPUUnload();
	}

	rm.resources.clear();
}