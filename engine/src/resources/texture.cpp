#include <stb_image.h>

#include "resources/resources_manager.hpp"
#include "debug/log.hpp"

#include "resources/texture.hpp"

bool Resources::Texture::CPULoad()
{
	resourceType = EResourceType::TEXTURE;

	stbi_set_flip_vertically_on_load_thread(flip);
	data = stbi_load(name.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		Log::Info("Successfully loaded texture file : " + name);

		ram = 0;
		vram = 0;

		vram += width * height * channels * sizeof(unsigned char);

		return true;
	}
	else
	{
		if (!ResourcesManager::GetDefaultTexture().Found(name))
    		Log::Warning("Could not load texture file : " + name);

		return false;
	}
}

bool Resources::Texture::GPULoad()
{
	gpu = std::make_unique<GPUTexture>();
	glGenTextures(1, &gpu->data);

	glBindTexture(GL_TEXTURE_2D, gpu->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		GLint format = channels == 1 ? GL_RED : channels == 2 ? GL_RG : channels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return true;
	}
	return false;
}

bool Resources::Texture::CPUUnload()
{
	if (data)
	{
		stbi_image_free(data);
		return true;
	}
	return false;
}

Resources::DefaultTexture::DefaultTexture()
{
	std::vector<std::string> colorNames;
	colorNames.emplace_back("red");
	colorNames.emplace_back("green");
    colorNames.emplace_back("blue");
    colorNames.emplace_back("black");
    colorNames.emplace_back("white");
	colorNames.emplace_back("transparent");

	std::vector<Vector4> colors;
	colors.emplace_back(Vector4{ 1.f, 0.f, 0.f, 1.f });
	colors.emplace_back(Vector4{ 0.f, 1.f, 0.f, 1.f });
	colors.emplace_back(Vector4{ 0.f, 0.f, 1.f, 1.f });
    colors.emplace_back(Vector4{ 0.f, 0.f, 0.f, 1.f });
    colors.emplace_back(Vector4{ 1.f, 1.f, 1.f, 1.f });
    colors.emplace_back(Vector4{ 1.f, 1.f, 1.f, 0.5f });

	{
		GLuint tex;

		glGenTextures(1, &tex);

		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		Vector4 pink = { 1.f, 0.f, 1.f, 1.f };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, pink.element);

		glBindTexture(GL_TEXTURE_2D, 0);

		textures["error"] = tex;
	}

	for (int i = 0; i < colors.size(); ++i)
	{
		GLuint tex;

		glGenTextures(1, &tex);

		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, colors[i].element);

		glBindTexture(GL_TEXTURE_2D, 0);

		textures[colorNames[i]] = tex;
	}
}

GLuint DefaultTexture::ChooseColor(const std::string_view& i_color) const
{
	if (textures.find(i_color.data()) != textures.end())
		return textures.find(i_color.data())->second;
	else
		return textures.find("transparent")->second;
}

bool DefaultTexture::Found(const std::string_view& i_color) const
{
	if (textures.find(i_color.data()) != textures.end())
		return true;
	else
		return false;
}