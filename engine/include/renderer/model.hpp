#pragma once

#include <memory>

#include "renderer/render_object.hpp"
#include "game/transform.hpp"

namespace Resources
{
    class Mesh;
	class Texture;
    class Serializer;
}

namespace Renderer
{
	class Model : public Renderer::RenderObject, public Renderer::SceneObject
	{
	public:
		std::weak_ptr<Resources::Mesh> mesh;

		std::string textureName = "";
		bool		flipTexture = false;

		std::weak_ptr<Resources::Texture> diffuseTex;

		Model();

		/**
		* @Summary Set a mesh coming from a 3D model file after the creation of this model object.
		* The specified mesh may contain embedded texture.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		*/
		void SetMeshFromFile(const std::string& i_meshFilename);

		/**
		* @Summary Set a mesh coming from a 3D model file, manually specifying its texture, after the creation of this model object.
		* User can set "ProceduralCube" or "ProceduralSphere" as filename to procedurally load a cube or a sphere.
		* If user uses procedural loading, the specified texture will not be precessed.
		*/
		void SetMeshFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture);

		/**
		 * @brief Set one texture to every submesh of the model's mesh.
		 * @param i_textureFilename The path from Engine root to the texture
		 * @param i_flipTexture Is the texture flip horizontally or not:
		 */
		void SetTexture(const std::string& i_textureFilename, const bool i_flipTexture);

        /**
         * @Summary Setup entity components from input file.
         * @param i_file the opened input file.
         */
        void Read(std::ifstream& i_file, const Resources::Serializer& i_serializer) {};

        /**
         * @Summary SaveScene the entity components in scene text format.
         * @param o_file the opened output file.
         */
        void Write(std::ofstream& o_file, const Resources::Serializer& i_serializer) const {};
	};
}