#include <filesystem>

#include "resources/resources_manager.hpp"
#include "resources/skeletal_mesh.hpp"

#include "animation/animation_graph.hpp"

void Animation::AnimationGraph::SetOwningSkeletalMesh(const std::shared_ptr<Resources::SkeletalMesh> i_owningSkmesh)
{
	owningSkmesh = i_owningSkmesh;
	player.SetSkeletalMesh(i_owningSkmesh);
}

void Animation::AnimationGraph::AddAnimationPack(const std::string& i_packName)
{
	if (owningSkmesh.lock())
	{
		std::filesystem::path skpath = owningSkmesh.lock()->name;
		std::string name = skpath.filename().string() + "_" + i_packName;

		skfiles.emplace_back(i_packName);
		skpacks.emplace_back(ResourcesManager::LoadResource<SkeletalAnimationPack>(name, i_packName, owningSkmesh.lock()));
	}
	else
		Log::Info("No skeletal mesh to attach animation to in the graph yet");
}

void Animation::AnimationGraph::FindPackAndRemove(const std::string& i_animationName)
{
	for (auto pack = skpacks.begin(); pack != skpacks.end(); ++pack)
	{
		if (pack->lock())
		{
			for (const auto& anim : pack->lock()->GetAnimationsMap())
			{
				if (anim.first == i_animationName)
				{
					for (auto file = skfiles.begin(); file != skfiles.end(); ++file)
					{
						if (*file == pack->lock()->animationFilename)
						{
							skfiles.erase(file);
							break;
						}
					}
					skpacks.erase(pack);
					return;
				}
			}
		}
	}
}

void Animation::AnimationGraph::GetAnimationsName(std::vector<std::string_view>& o_names) const
{
	for (const auto& pack : skpacks)
	{
		if (pack.lock())
		{
			for (const auto& anim : pack.lock()->GetAnimationsMap())
				o_names.emplace_back(anim.first);
		}
	}
}

void Animation::AnimationGraph::GetFilesName(std::vector<std::string_view>& o_names) const
{
	for (const auto& file : skfiles)
		o_names.emplace_back(file);
}

bool Animation::AnimationGraph::PlayAnimation(const std::string& i_animationName, const bool i_loop)
{
	const SkeletalAnimation* anim = nullptr;
	for (const auto& pack : skpacks)
	{
		if (pack.expired())
			continue;

		anim = pack.lock()->GetAnimation(i_animationName);
		if (anim)
		{
			player.UploadAnimation(anim, i_loop);
			return true;
		}
	}

	Log::Info("No animation named " + std::string(i_animationName));
	return false;
}