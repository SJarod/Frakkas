#include <iostream>

#include "debug/log.hpp"
#include "resources/resources_manager.hpp"

#include "resources/program_shader.hpp"

Resources::Shader::Shader(const std::string& i_name,
	const std::string& i_shaderFilePath,
	const std::initializer_list<const std::string>& i_defines)
	: vertexShaderName(i_shaderFilePath), fragmentShaderName(i_shaderFilePath)
{
	name = i_name;

	int size = i_defines.size();
	for (int i = 0; i < size; ++i)
		defines.emplace_back(*(i_defines.begin() + i));
}

Resources::Shader::Shader(const std::string& i_name,
	const std::string& i_vertexShaderFilePath,
	const std::string& i_fragmentShaderFilePath,
	const std::initializer_list<const std::string>& i_defines)
	: vertexShaderName(i_vertexShaderFilePath), fragmentShaderName(i_fragmentShaderFilePath)
{
	name = i_name;

	int size = i_defines.size();
	for (int i = 0; i < size; ++i)
		defines.emplace_back(*(i_defines.begin() + i));
}

bool Resources::Shader::GPULoad()
{
	resourceType = EResourceType::SHADER;

	//vertex shader
	std::ifstream vsStream(vertexShaderName + ".vs");
	std::ifstream fsStream(fragmentShaderName + ".fs");

	if (!vsStream.is_open() || !fsStream.is_open())
	{
		Log::Error("Could not open shader files : " + name);
		return false;
	}

	std::string vs((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());
	std::string fs((std::istreambuf_iterator<char>(fsStream)), std::istreambuf_iterator<char>());

	std::string version = "#version 450 core\n";

	std::vector<const char*> uberVertexShaderSources;
	uberVertexShaderSources.emplace_back(version.c_str());
	for (const std::string& def : defines)
		uberVertexShaderSources.emplace_back(def.c_str());

	uberVertexShaderSources.emplace_back(vs.c_str());

	std::vector<const char*> uberFragmentShaderSources;
	uberFragmentShaderSources.emplace_back(version.c_str());
	for (const std::string& def : defines)
		uberFragmentShaderSources.emplace_back(def.c_str());

	uberFragmentShaderSources.emplace_back(fs.c_str());

	//shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, uberVertexShaderSources.size(), &uberVertexShaderSources[0], nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, uberFragmentShaderSources.size(), &uberFragmentShaderSources[0], nullptr);
	glCompileShader(fragmentShader);

	int  success[2];
	char infoLog[2][512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success[0]);

	if (!success[0])
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog[0]);
		Log::Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED : " + vertexShaderName + ".vs\n" + infoLog[0]);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success[1]);

	if (!success[1])
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog[1]);
		Log::Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " + fragmentShaderName + ".fs\n" + infoLog[1]);
	}

	if (success[0] && success[1])
	{
		Log::Info("Successfully loaded shader files : " + name);

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	vsStream.close();
	fsStream.close();

	ram = 0;
	vram = 0;

	int size;
	glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &size);
	vram = size;

	return true;
}

bool Resources::Shader::GPUUnload()
{
	glDeleteShader(program);
	return true;
}

void Resources::Shader::Use() const
{
	glUseProgram(program);
}

void Resources::Shader::Unuse()
{
    glUseProgram(0);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const bool& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniform1i(location, i_value);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const float& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniform1f(location, i_value);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Matrix4& i_value, const bool i_transpose) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniformMatrix4fv(location, 1, i_transpose, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const int i_num, const float* i_array, const bool i_transpose) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniformMatrix4fv(location, i_num, i_transpose, i_array);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector3& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniform3fv(location, 1, i_value.element);
}

void Resources::Shader::SetUniform(const std::string_view& i_uniformName, const Vector4& i_value) const
{
	int location = glGetUniformLocation(program, i_uniformName.data());
	glUniform4fv(location, 1, i_value.element);
}