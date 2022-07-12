#include <cassert>
#include <filesystem>

#include "debug/log.hpp"
#include "resources/resources_manager.hpp"

#include "resources/skeletal_animation.hpp"

using namespace Resources;

float KeyFrameBone::GetLerpFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime) const
{
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	float t = midWayLength / framesDiff;
	return t;
}

Matrix4 KeyFrameBone::InterpolatePosition(const float animationTime) const
{
	if (1 == positions.size())
		return Matrix4::Translate(positions[0].pos);

	int p0 = GetPositionKeyFrame(animationTime);
	int p1 = p0 + 1;
	float scaleFactor = GetLerpFactor(positions[p0].timeStamp, positions[p1].timeStamp, animationTime);

	Vector3 finalPosition = Maths::Lerp(positions[p0].pos, positions[p1].pos, scaleFactor);
	return Matrix4::Translate(finalPosition);
}

Matrix4 KeyFrameBone::InterpolateRotation(const float animationTime) const
{
	if (1 == rotations.size())
	{
		Quaternion rotation = rotations[0].rot.Normalize();
		return Matrix4::MatrixFromQuat(rotation);
	}

	int r0 = GetRotationKeyFrame(animationTime);
	int r1 = r0 + 1;
	float scaleFactor = GetLerpFactor(rotations[r0].timeStamp, rotations[r1].timeStamp, animationTime);
	Quaternion finalRotation = Quaternion::Slerp(rotations[r0].rot, rotations[r1].rot, scaleFactor);

	finalRotation = finalRotation.Normalize();
	return Matrix4::MatrixFromQuat(finalRotation);
}

Matrix4 KeyFrameBone::InterpolateScale(const float animationTime) const
{
	if (1 == scales.size())
		return Matrix4::Scale(scales[0].scl);

	int s0 = GetScaleKeyFrame(animationTime);
	int s1 = s0 + 1;
	float scaleFactor = GetLerpFactor(scales[s0].timeStamp, scales[s1].timeStamp, animationTime);

	Vector3 finalScale = Maths::Lerp(scales[s0].scl, scales[s1].scl, scaleFactor);
	return Matrix4::Scale(finalScale);
}

KeyFrameBone::KeyFrameBone(const std::string_view& i_name, const int i_id, const aiNodeAnim* i_assimpChannel)
	: name(i_name), id(i_id)
{
	for (int positionIndex = 0; positionIndex < i_assimpChannel->mNumPositionKeys; ++positionIndex)
	{
		aiVector3D aiPosition = i_assimpChannel->mPositionKeys[positionIndex].mValue;
		float timeStamp = i_assimpChannel->mPositionKeys[positionIndex].mTime;

		KeyFramePos data;
		data.pos = { aiPosition.x, aiPosition.y, aiPosition.z };
		data.timeStamp = timeStamp;
		positions.emplace_back(data);
	}

	for (int rotationIndex = 0; rotationIndex < i_assimpChannel->mNumRotationKeys; ++rotationIndex)
	{
		aiQuaternion aiOrientation = i_assimpChannel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = i_assimpChannel->mRotationKeys[rotationIndex].mTime;

		KeyFrameRot data;
		data.rot = { aiOrientation.x, aiOrientation.y, aiOrientation.z, aiOrientation.w };
		data.timeStamp = timeStamp;
		rotations.emplace_back(data);
	}

	for (int keyIndex = 0; keyIndex < i_assimpChannel->mNumScalingKeys; ++keyIndex)
	{
		aiVector3D scale = i_assimpChannel->mScalingKeys[keyIndex].mValue;
		float timeStamp = i_assimpChannel->mScalingKeys[keyIndex].mTime;

		KeyFrameScl data;
		data.scl = { scale.x, scale.y, scale.z };
		data.timeStamp = timeStamp;
		scales.emplace_back(data);
	}
}

void KeyFrameBone::InterpolateTransform(const float animationTime, Matrix4& t, Matrix4& r, Matrix4& s) const
{
	t = InterpolatePosition(animationTime);
	r = InterpolateRotation(animationTime);
	s = InterpolateScale(animationTime);
}

int KeyFrameBone::GetPositionKeyFrame(const float animationTime) const
{
	int size = positions.size();
	for (int index = 0; index < size - 1; ++index)
	{
		if (animationTime < positions[index + 1].timeStamp)
			return index;
	}
	return size - 1;
}

int KeyFrameBone::GetRotationKeyFrame(const float animationTime) const
{
	int size = rotations.size();
	for (int index = 0; index < size - 1; ++index)
	{
		if (animationTime < rotations[index + 1].timeStamp)
			return index;
	}
	return size - 1;
}

int KeyFrameBone::GetScaleKeyFrame(const float animationTime) const
{
	int size = scales.size();
	for (int index = 0; index < size - 1; ++index)
	{
		if (animationTime < scales[index + 1].timeStamp)
			return index;
	}
	return size - 1;
}

const std::string& KeyFrameBone::GetName() const
{
	return name;
}

size_t SkeletonNodeData::GetMemorySize() const
{
	int size = 0;
	size += sizeof(SkeletonNodeData);

	int numChildren = children.size();
	for (int i = 0; i < numChildren; ++i)
		size += children[i].GetMemorySize();

	return size;
}

void SkeletalAnimation::LoadKeyFrameBones(const aiAnimation* animation, SkeletalMesh& skmesh)
{
	int size = animation->mNumChannels;

	std::unordered_map<std::string, Resources::Bone>& bim = skmesh.boneInfoMap;
	int& boneCount = skmesh.boneCount;

	for (int i = 0; i < size; ++i)
	{
		aiNodeAnim* channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (bim.find(boneName) == bim.end())
		{
			bim[boneName].id = boneCount;
			++boneCount;
		}
		kfBones.emplace_back(KeyFrameBone(boneName, bim[boneName].id, channel));
	}
}

SkeletalAnimation::SkeletalAnimation(const std::string& i_name, const aiScene* scene, SkeletalMesh& skmesh, const int animationIndex)
	: animationIndex(animationIndex), animationName(i_name)
{
	aiAnimation* animation = scene->mAnimations[animationIndex];

	duration = animation->mDuration;
	tick = animation->mTicksPerSecond;

	LoadKeyFrameBones(animation, skmesh);

	Log::Info("Successfully loaded animation in file : " + i_name);
}

const KeyFrameBone* SkeletalAnimation::FindBone(const std::string_view& name) const
{
	const std::vector<KeyFrameBone>::const_iterator& iter = std::find_if(kfBones.begin(), kfBones.end(),
		[&](const KeyFrameBone& kfb)
		{
			return kfb.GetName() == name;
		}
	);

	if (iter == kfBones.end())
		return nullptr;
	else
		return &(*iter);
}

size_t SkeletalAnimation::GetMemorySize() const
{
	int size = 0;

	size += kfBones.size() * sizeof(KeyFrameBone);

	return size;
}

SkeletalAnimationPack::SkeletalAnimationPack(const std::string& i_name,
	const std::string& i_animationFilename,
	std::shared_ptr<SkeletalMesh> i_skmesh)
	: animationFilename(i_animationFilename), owningSkmesh(i_skmesh)
{
	name = i_name;
}

bool SkeletalAnimationPack::DependenciesReady()
{
	if (owningSkmesh.expired())
		return true;
	return owningSkmesh.lock()->submeshes.size() != 0;
}

bool SkeletalAnimationPack::CPULoad()
{
	resourceType = EResourceType::ANIMPACK;

	if (owningSkmesh.expired())
	{
		Log::Info(animationFilename, "'s owning skeletal mesh was not correctly loaded");
		return false;
	}

	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* scene = importer.ReadFile(animationFilename, aiProcess_Triangulate);
	if (!scene || !scene->mRootNode)
	{
		Log::Info(animationFilename, " is not an animation file");
		return false;
	}

	if (!scene->HasAnimations())
	{
		Log::Info(animationFilename + " has no animations");
		return false;
	}

	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		std::filesystem::path path = animationFilename;
		const std::string animationName = path.filename().string() + "_" + std::string(scene->mAnimations[i]->mName.data);
		SkeletalAnimation skanim = SkeletalAnimation(animationName, scene, *owningSkmesh.lock(), i);

#ifdef ANIMATION_MAP
		animations.insert({ animationName, skanim });
#else
		animations.emplace_back(skanim);
#endif
	}

	Log::Info("Successfully loaded animation pack in file : " + animationFilename);

	ram = 0;
	vram = 0;

	for (const auto& anim : animations)
	{
#ifdef ANIMATION_MAP
		ram += anim.second.GetMemorySize();
#else
		ram += anim.GetMemorySize();
#endif
	}

	return true;
}

#ifdef ANIMATION_MAP
const SkeletalAnimation* SkeletalAnimationPack::GetAnimation(const std::string_view& i_animationName) const
{
	if (animations.find(std::string(i_animationName)) == animations.end())
		return nullptr;

	return &animations.find(std::string(i_animationName))->second;
}

const std::unordered_map<std::string, SkeletalAnimation>& SkeletalAnimationPack::GetAnimationsMap() const
{
	return animations;
}
#else
const SkeletalAnimation* SkeletalAnimationPack::GetAnimation(const unsigned int i) const
{
	if (i >= animations.size())
	{
		Log::Info("No animation at index " + std::to_string(i));
		return nullptr;
	}

	return &animations[i];
}
#endif