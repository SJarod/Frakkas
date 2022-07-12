#pragma once

#include <glad/glad.h>
#include <memory>
#include <unordered_map>

#include "maths.hpp"
#include "resources/resource.hpp"

namespace Resources
{
	enum class ETextureType
	{
		TEXTURE_DIFFUSE = 0
	};

	struct GPUTexture
	{
	public:
		GLuint data;
	};

	class Texture : public Resource
	{
	public:
		ETextureType	type = ETextureType::TEXTURE_DIFFUSE;
		bool			flip = false;

		int				width = 0;
		int				height = 0;
		int				channels = 0;

		void*			data = nullptr;

		std::unique_ptr<GPUTexture> gpu;

		Texture(const std::string& i_name, const bool i_flip)
		{
			name = i_name;
			flip = i_flip;
		}

		bool DependenciesReady() override { return true; }
		bool CPULoad() override;
		bool GPULoad() override;
		bool CPUUnload() override;
		bool GPUUnload() override { return true; }
	};

	class DefaultTexture
	{
	private:
		std::unordered_map<std::string, GLuint> textures;

	public:
		DefaultTexture();

		GLuint ChooseColor(const std::string_view& i_color) const;

		bool Found(const std::string_view& i_color) const;
	};
}