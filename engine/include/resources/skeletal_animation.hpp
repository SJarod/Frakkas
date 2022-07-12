#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "resources/resource.hpp"
#include "resources/skeletal_mesh.hpp"
#include "maths.hpp"

#define ANIMATION_MAP

namespace Resources
{
    struct KeyFramePos
    {
        Vector3 pos;
        float timeStamp;
    };

    struct KeyFrameRot
    {
        Quaternion rot;
        float timeStamp;
    };

    struct KeyFrameScl
    {
        Vector3 scl;
        float timeStamp;
    };

    class KeyFrameBone
    {
    private:
        std::vector<KeyFramePos> positions;
        std::vector<KeyFrameRot> rotations;
        std::vector<KeyFrameScl> scales;

        std::string name;
        int id;

        /**
         * Get a scale factor depending on two time stamps and a general time.
         */
        float GetLerpFactor(const float i_lastTimeStamp, const float i_nextTimeStamp, const float i_animationTime) const;

        /**
         * Get the translation matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolatePosition(const float i_animationTime) const;

        /**
         * Get the rotation matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolateRotation(const float i_animationTime) const;

        /**
         * Get the scale matrix of a bone with its value interpolated between two time stamps, depending on the animation time.
         */
        Matrix4 InterpolateScale(const float i_animationTime) const;

    public:
        /**
         * Create a bone containing every key frame values with an Assimp channel.
         * Assimp channels are bones.
         */
        KeyFrameBone(const std::string_view& i_name, const int i_id, const aiNodeAnim* i_assimpChannel);

        /**
         * Store every interpolated transforms in output matrices.
         */
        void InterpolateTransform(const float i_animationTime, Matrix4& o_t, Matrix4& o_r, Matrix4& o_s) const;

        /**
         * Get the key position's index depending on the animation time.
         */
        int GetPositionKeyFrame(const float i_animationTime) const;

        /**
         * Get the key rotation's index depending on the animation time.
         */
        int GetRotationKeyFrame(const float i_animationTime) const;

        /**
         * Get the key scale's index ddepending on the animation time.
         */
        int GetScaleKeyFrame(const float i_animationTime) const;

        /**
         * Get this bone name.
         */
        const std::string& GetName() const;
    };

    /**
     * One animation found in the animation file.
     */
    class SkeletalAnimation
    {
    private:
        std::vector<KeyFrameBone> kfBones;

        const int animationIndex = 0;

        /**
         * Load the keyframe bones.
         * Repair the skeletal mesh bone info if bones are missing.
         */
        void LoadKeyFrameBones(const aiAnimation* i_animation, SkeletalMesh& io_skmesh);

    public:
        const std::string animationName;

        float duration = 0.f;
        //framerate
        float tick = 0.f;

        SkeletalAnimation(const std::string& i_name,
            const aiScene* i_scene,
            SkeletalMesh& i_skmesh,
            const int i_animationIndex);

        /**
         * Look for a bone in the bone map with its name.
         */
        const KeyFrameBone* FindBone(const std::string_view& i_name) const;

        /**
         * Get the size of this submesh.
         */
        size_t GetMemorySize() const;
    };

    /**
     * A skeletal animation pack is a set of animations, representing one animation file.
     * There can be multiple animations in one file.
     * It is made from and for a specific skeletal mesh, it is stored as a resource.
     */
    class SkeletalAnimationPack : public Resources::Resource
    {
    private:
#ifdef ANIMATION_MAP
        std::unordered_map<std::string, SkeletalAnimation> animations;
#else
        std::vector<SkeletalAnimation> animations;
#endif

        std::weak_ptr<SkeletalMesh> owningSkmesh;

    public:
        const std::string animationFilename;

        SkeletalAnimationPack(const std::string& i_name,
            const std::string& i_animationFilename,
            std::shared_ptr<SkeletalMesh> i_skmesh);

        bool DependenciesReady() override;
        bool CPULoad() override;
        bool GPULoad() override { return true; }
        bool CPUUnload() override { return true; }
        bool GPUUnload() override { return true; }

#ifdef ANIMATION_MAP
        /**
         * Get an animation specifying its name.
         */
        const SkeletalAnimation* GetAnimation(const std::string_view& i_animationName) const;

        /**
         * Get the animations map.
         */
        const std::unordered_map<std::string, SkeletalAnimation>& GetAnimationsMap() const;
#else
        /**
         * Get an animation specifying its index.
         */
        const SkeletalAnimation* GetAnimation(const unsigned int i_index) const;
#endif
    };
}