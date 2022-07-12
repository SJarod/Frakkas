#pragma once

#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "resources/mesh.hpp"
#include "maths.hpp"

#define MAX_BONES 100

namespace Resources
{
	struct Bone
	{
		int id;
		Matrix4 offset;
	};

	/**
	 * Structure used to store the skeleton's hierarchy.
	 */
	struct SkeletonNodeData
	{
		Matrix4 transform;
		std::string name;

		int childrenCount;
		std::vector<SkeletonNodeData> children;

		/**
		 * Get the size of this submesh.
		 */
		size_t GetMemorySize() const;
	};

	class SkeletalMesh : public Resources::Mesh
	{
	private:
		/**
		 * Extract bone info while loading the submesh.
		 */
		void ProcessAiMesh(std::shared_ptr<Submesh>& o_submesh,
			const aiMesh& i_aim,
			const aiScene& i_scene) override;

		/**
		 * Set a specific vertex's bone indices and weights.
		 */
		void SetVertexBoneData(Vertex& vertex, const int boneID, const float weight);

		/**
		 * Load bone informations (bone indice and bone weight) for each vertex of the mesh.
		 */
		void ExtractBoneWeightForVertices(std::shared_ptr<Submesh>& o_submesh,
			const aiMesh& i_aim,
			const aiScene& i_scene);

		/**
		 * Load the skeleton node data with Assimp's data.
		 */
		void LoadSkeleton(SkeletonNodeData& dest, const aiNode* src);

	public:
		std::unordered_map<std::string, Bone> boneInfoMap;
		SkeletonNodeData rootNode;
		int boneCount = 0;

		SkeletalMesh(const std::string& i_name);

		bool DependenciesReady() override { return true; }
		bool CPULoad() override;
		bool GPULoad() override { return Mesh::GPULoad(); }
		bool CPUUnload() override { return true; }
		bool GPUUnload() override { return Mesh::GPUUnload(); }

		/**
		 * Get the root node of the skeleton's data.
		 */
		const SkeletonNodeData* GetRootNode() const;
	};
}