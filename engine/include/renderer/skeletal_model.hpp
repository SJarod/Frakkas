#pragma once

#include <memory>

#include "renderer/render_object.hpp"
#include "renderer/model.hpp"
#include "game/transform.hpp"

namespace Resources
{
	class Texture;

	struct Bone;
	class SkeletalMesh;

	class Serializer;
}

namespace Renderer
{
	class Socket
	{
	public:
        bool render = false;
		Game::Transform transform;
		Renderer::Model model;
		int boneID;
	};

	class SkeletalModel : public Renderer::RenderObject, public Renderer::SceneObject
	{
	public:
		bool showSkeleton = false;

		std::weak_ptr<Resources::SkeletalMesh> skmesh;
		std::list<Socket> sockets;

		std::string textureName = "";
		bool		flipTexture = false;

		std::weak_ptr<Resources::Texture> diffuseTex;

		SkeletalModel();

		/**
		* @Summary Set a mesh coming from a 3D model file after the creation of this model object.
		* The specified mesh may contain embedded texture.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		*/
		void SetSkeletalMeshFromFile(const std::string& i_meshFilename);

		/**
		* @Summary Set a mesh coming from a 3D model file, manually specifying its texture, after the creation of this model object.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		* If user uses procedural loading, the specified texture will not be precessed.
		*/
		void SetSkeletalMeshFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * @brief Set one texture to every submesh of the model's mesh.
		 * @param i_textureFilename The path from Engine root to the texture
		 * @param i_flipTexture Is the texture flip horizontally or not:
		 */
		void SetTexture(const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * @brief Add a socket to the socket list.
		 */
		void AddSocket(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture, int i_boneID, const Game::Transform& i_transform);

		/**
		 * @brief Remove a socket from the socket list.
		 */
		void RemoveSocket(int i_boneID);

		/**
		 * @Summary Setup entity components from input file.
		 * @param i_file the opened input file.
		 */
		void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

		/**
		 * @Summary Write the entity components in scene text format.
		 * @param o_file the opened output file.
		 */
		void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};
	};
}