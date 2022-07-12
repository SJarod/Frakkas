#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <type_traits>

#include "maths.hpp"
#include "resources/resource.hpp"

namespace Resources
{
	class Shader : public Resource
	{
	public:
		/**
		 * @Summary Create a shader from a couple of shader files.
		 * @param i_shaderName : Shader name
		 */
		Shader(const std::string& i_name,
			const std::string &i_shaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});

		/**
		 * @Summary Create a shader from a vertex shader file and a fragment shader file seperately.
		 */
		Shader(const std::string& i_name,
			const std::string& i_vertexShaderFilePath,
			const std::string& i_fragmentShaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});

		bool DependenciesReady() override { return true; }
		bool CPULoad() override { return true; }
		bool GPULoad() override;
		bool CPUUnload() override { return true; }
		bool GPUUnload() override;

		/**
		 * @Summary Use shader program.
		 */
		void Use() const;

        /**
         * @Summary Un use current shader program.
         */
        static void Unuse();

        /**
        * @Summary Pass a bool to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const bool& i_value) const;

		/**
		* @Summary Pass a float to the shader as uniform.
		*/
		void SetUniform(const std::string_view& i_uniformName, const float& i_value) const;

        /**
        * @Summary Pass a Matrix4 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Matrix4& i_value, const bool i_transpose = false) const;

		/**
		* @Summary Pass an array of Matrix4 as an array of float to the shader as uniform.
		*/
		void SetUniform(const std::string_view& i_uniformName, const int i_num, const float* i_array, const bool i_transpose = false) const;

        /**
        * @Summary Pass a Vector3 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Vector3& i_value) const;

        /**
        * @Summary Pass a Vector4 to the shader as uniform.
        */
        void SetUniform(const std::string_view& i_uniformName, const Vector4& i_value) const;

	private:
		const std::string vertexShaderName;
		const std::string fragmentShaderName;

		std::vector<std::string> defines;

		GLuint program = 0;
	};
}