#pragma once

#include <vector>
#include <list>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "resources/resource.hpp"
#include "renderer/lowlevel/vertex.hpp"
#include "resources/texture.hpp"

using namespace Renderer::LowLevel;

namespace Resources
{
	struct GPUMesh
	{
		GLuint VBO = 0;
		GLuint VAO = 0;

		void Unload()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
		}
	};

	class Submesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Matrix4 localTransform = Matrix4::Identity();

		GPUMesh gpu;

		/**
		 * Get the size of this submesh.
		 */
		size_t GetMemorySize() const;
	};

	class Mesh : public Resource
	{
	public:
		// Those string are code name to load in-engine shape when call LoadFromInfo()
        static constexpr char quadMesh[] = "ProceduralQuad";
		static constexpr char cubeMesh[] = "ProceduralCube";
		static constexpr char sphereMesh[] = "ProceduralSphere";
		static constexpr char icoSphereMesh[] = "ProceduralIcoSphere";
		static constexpr char cubeColliderMesh[] = "ProceduralCubeLines";
		static constexpr char sphereColliderMesh[] = "ProceduralSphereLines";

	protected:

		////////////////////////////////////////////// ASSIMP

		/**
		 * Extrat Assimp's mesh data (vertices and textures) and load them into the given mesh.
		 * Try to load mesh's embeddeed textures if no specific texture file was specified.
		 *
		 * @param o_submesh
		 * @param i_aiMesh (from Assimp)
		 * @param i_aiScene
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		virtual void ProcessAiMesh(std::shared_ptr<Submesh>& o_submesh,
			const aiMesh& i_aim,
			const aiScene& i_scene);

		/**
		 * Go through every Assimp node to extract data (meshes) and add the loaded mesh into the given array of meshes.
		 * Tell ProcessAiMesh() to load embedded textures or not.
		 *
		 * @param o_submeshes
		 * @param i_importer
		 * @param i_node
		 * @param i_textureFilename
		 * @param i_flipTexture
		 */
		void ProcessAiNode(std::list<std::shared_ptr<Submesh>>& o_submeshes,
			const Assimp::Importer& i_importer,
			const aiNode* i_node);

		/**
		 * Reorder mesh's vertices with its stored indices.
		 *
		 * @param io_submesh
		 */
		void ParseSubmesh(Submesh& io_submesh);

		////////////////////////////////////////////// PROCEDURAL LOADING

        /**
         * Procedurally load a quad.
         */
        void LoadQuad();

		/**
		 * Procedurally load a cube.
		 */
		void LoadCube();

        /**
         * Load procedurally a cube for line rendering
         */
        void LoadLineCube();

		/**
		 * Procedurally load a sphere.
		 *
		 * @param i_radius
		 * @param i_longitude
		 * @param i_latitude
		 */
		void LoadSphere(const float i_radius = 1.f,
			const int i_longitude = 50,
			const int i_latitude = 25);

		/**
		 * Procedurally load an icosphere.
		 *
		 * @param i_res the sphere resolution
		 */
		void LoadIcoSphere(const int i_res = 4);

		/**
		 * Load procedurally a sphere for line rendering
		 */
        void LoadLineSphere(float i_radius = 1.f);

	public:
		// Every submeshes that this mesh possesses.
		std::list<std::shared_ptr<Submesh>> submeshes;

		Mesh(const std::string& i_name);

		bool DependenciesReady() override { return true; }
		bool CPULoad() override;
		bool GPULoad() override;
		bool CPUUnload() override { return true; }
		bool GPUUnload() override;

		void ComputeMemorySize();
	};
}