#include "debug/log.hpp"
#include "resources/skeletal_mesh.hpp"
#include "resources/skeletal_animation.hpp"

#include "animation/animator.hpp"

Animation::Animator::Animator()
{
	for (unsigned int i = 0; i < MAX_BONES; ++i)
	{
		globalBoneMatrices.emplace_back(Matrix4::Identity());
		rawBoneMatrices.emplace_back(Matrix4::Identity());
	}
}

void Animation::Animator::UploadAnimation(const SkeletalAnimation* anim, const bool i_loop)
{
	playTime = 0.f;
	loop = i_loop;
	this->anim = anim;
	animBoneInfo = &owningSkmesh.lock()->boneInfoMap;
}

void Animation::Animator::SetSkeletalMesh(const std::shared_ptr<Resources::SkeletalMesh> i_owningSkmesh)
{
	owningSkmesh = i_owningSkmesh;
}

void Animation::Animator::UnloadAnimation()
{
	playTime = 0.f;
	loop = false;
	this->anim = nullptr;
}

void Animation::Animator::UpdatePlayer(const float& deltaTime)
{
	if (!anim)
		return;

	float rewind = playSpeed >= 0.f ? 0.f : 1.f;
	float rewindOffset = rewind * anim->duration;

	playTime += deltaTime * anim->tick * playSpeed + rewindOffset;

	if (loop)
		playTime = fmodf(playTime, anim->duration);

	else if (playTime >= anim->duration)
	{
		UnloadAnimation();
		return;
	}

	UpdateBoneMatrices(owningSkmesh.lock()->GetRootNode());
	CancelExtraTS(true, false, true, true);
}

bool Animation::Animator::IsWaiting() const
{
	return !anim;
}

bool Animation::Animator::IsLooping() const
{
    return loop;
}

const std::vector<Matrix4>& Animation::Animator::GetGlobalBoneMatrices() const
{
	return globalBoneMatrices;
}

const std::vector<Matrix4>& Animation::Animator::GetRawBoneMatrices() const
{
	return rawBoneMatrices;
}

void Animation::Animator::UpdateBoneMatrices(const SkeletonNodeData* node, const Matrix4& parentTransform)
{
	std::string nodeName(node->name);
	Matrix4 nodeTransform = node->transform.Transpose();

	const KeyFrameBone* kfBone = anim->FindBone(nodeName);

	if (kfBone)
	{
		Matrix4 t, r, s;
		kfBone->InterpolateTransform(playTime, t, r, s);
		nodeTransform = s * r * t;
	}

	Matrix4 globalTransformation = nodeTransform * parentTransform;

	if (animBoneInfo->find(nodeName) != animBoneInfo->end())
	{
		int index = animBoneInfo->find(nodeName)->second.id;
		Matrix4 offset = animBoneInfo->find(nodeName)->second.offset.Transpose();

		globalBoneMatrices[index] = offset * globalTransformation;
		rawBoneMatrices[index] = globalTransformation;
	}

	for (unsigned int i = 0; i < node->childrenCount; ++i)
		UpdateBoneMatrices(&node->children[i], globalTransformation);
}

void Animation::Animator::CancelExtraTS(const bool i_x, const bool i_y, const bool i_z, const bool i_scale)
{
	float x = static_cast<float>(i_x);
	float y = static_cast<float>(i_y);
	float z = static_cast<float>(i_z);
	Vector3 extra = { x, y, z };

	Matrix4 invGlobalExtra;
	{
		Vector3 t = globalBoneMatrices[0].DecomposeTranslation() * extra;

		if (i_scale)
		{
			Vector3 s = globalBoneMatrices[0].DecomposeScale();
			invGlobalExtra = (Matrix4::Scale(s) * Matrix4::Translate(t)).Inverse();
		}
		else
			invGlobalExtra = Matrix4::Translate(t).Inverse();
	}

	Matrix4 invRawExtra;
	{
		Vector3 t = rawBoneMatrices[0].DecomposeTranslation() * extra;
		invRawExtra = Matrix4::Translate(t).Inverse();
	}

	for (int i = 0; i < MAX_BONES; ++i)
	{
		globalBoneMatrices[i] = globalBoneMatrices[i] * invGlobalExtra;
		rawBoneMatrices[i] = rawBoneMatrices[i] * invRawExtra;
	}
}