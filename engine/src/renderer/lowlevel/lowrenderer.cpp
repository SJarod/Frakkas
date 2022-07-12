#include "resources/texture.hpp"
#include "renderer/point.hpp"
#include "renderer/screen.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"

using namespace Renderer::LowLevel;

Framebuffer::Framebuffer(float i_width, float i_height, ERenderMode i_renderMode)
{
    renderMode = i_renderMode;

    size.setter = [&](const Vector2& i_value) { SetSize(i_value); };

	glCreateFramebuffers(1, &FBO);

    glCreateTextures(GL_TEXTURE_2D, 1, &color0);
    glTextureParameteri(color0, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(color0, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(color0, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(color0, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glCreateTextures(GL_TEXTURE_2D, 1, &depthStencil);
    glTextureParameteri(depthStencil, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthStencil, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(depthStencil, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthStencil, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Set size property to generate framebuffer texture storage
    size = {i_width, i_height};

    // attaching textures to the framebuffer's corresponding department
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, color0, 0);
    glNamedFramebufferTexture(FBO, GL_DEPTH_STENCIL_ATTACHMENT, depthStencil, 0);
}

void Framebuffer::SetSize(const Vector2& i_value)
{
    if (i_value.x > 0.f && i_value.y > 0.f && i_value != size)
    {
        Vector2 oldSize = i_value, newSize = i_value;
        float newAspect = i_value.x / i_value.y;
        switch(renderMode)
        {

            case ERenderMode::LOCK_ASPECT:              // CHANGE ONLY SIZE
                if (newAspect > AspectRatio())
                    newSize.x = AspectRatio() * oldSize.y;
                else if (newAspect < AspectRatio())
                    newSize.y = oldSize.x / AspectRatio();

                offset.x = (oldSize.x - newSize.x) / 2.f;
                offset.y = (oldSize.y - newSize.y) / 2.f;

                size.set(newSize);

                glBindTexture(GL_TEXTURE_2D, color0);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                glBindTexture(GL_TEXTURE_2D, depthStencil);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, GetWidth(), GetHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

                break;

            case ERenderMode::FREE_ASPECT:              // CHANGE BOTH
            default:
                if (size == Vector2(0.f, 0.f))
                {
                    size.set(i_value);

                    glBindTexture(GL_TEXTURE_2D, color0);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                    glBindTexture(GL_TEXTURE_2D, depthStencil);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, GetWidth(), GetHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
                }

                AspectRatio() = i_value.x / i_value.y;
                break;
        }
    }
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::GetColor0() const
{
	return color0;
}

GLuint Framebuffer::GetDepthStencilMap() const
{
	return depthStencil;
}

int Framebuffer::GetWidth() const
{
	return size.get().x;
}

int Framebuffer::GetHeight() const
{
	return size.get().y;
}

float& Framebuffer::AspectRatio()
{
    return renderMode == ERenderMode::LOCK_ASPECT ? lockAspectRatio : aspectRatio;
}

DepthFramebuffer::DepthFramebuffer(const int i_width, const int i_height)
	: Framebuffer(i_width, i_height, ERenderMode::FREE_ASPECT)
{
	glDeleteFramebuffers(1, &FBO);
	glCreateFramebuffers(1, &FBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, i_width, i_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attaching texture to the framebuffer's depth buffer department
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint DepthFramebuffer::GetDepthMap() const
{
	return depthMap;
}

UniformBuffer::UniformBuffer(const int i_binding, const int i_size)
	: blockBinding(i_binding), blockSize(i_size)
{
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding, UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const bool i_value) const
{
	int b = static_cast<int>(i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(int), &b);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const int i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(int), &i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const float i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(float), &i_value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Vector3& i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Vector3), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Vector4& i_value) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Vector4), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetUniformToBlock(const int i_offset, const Matrix4& i_value) const
{
    Framebuffer::Unbind();
    glViewport(0, 0, 1920, 1080);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, i_offset, sizeof(Matrix4), i_value.element);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

LowRenderer::LowRenderer()
{
	shaderUBOs.insert({ "uRenderMatrices", UniformBuffer(shaderUBOs.size(), 2 * sizeof(Matrix4)) });
	shaderUBOs.insert({ "uRendering", UniformBuffer(shaderUBOs.size(), 128) });
	shaderUBOs.insert({ "uLightMatrices", UniformBuffer(shaderUBOs.size(), 2 * sizeof(Matrix4)) });
	depthMapFBO = std::make_unique<Renderer::LowLevel::DepthFramebuffer>(shadowMapResolution, shadowMapResolution);
	firstPassFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Framebuffer::ERenderMode::FREE_ASPECT);
	secondPassFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Framebuffer::ERenderMode::FREE_ASPECT);
}

void LowRenderer::BeginFrame(const Framebuffer& i_fbo) const
{
	i_fbo.Bind();
	glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());

	glClearColor(0.4f, 0.4f, 0.4f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LowRenderer::BeginFrame(const DepthFramebuffer& i_fbo, const float i_bias) const
{
	i_fbo.Bind();
	glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(i_bias, i_bias);
}

void LowRenderer::ContinueFrame(const Framebuffer& i_fbo) const
{
    i_fbo.Bind();
    glViewport(0, 0, i_fbo.GetWidth(), i_fbo.GetHeight());
    glEnable(GL_DEPTH_TEST);
}

void LowRenderer::EndFrame() const
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_BLEND);

	Framebuffer::Unbind();
}

void LowRenderer::SetWindowSize(const Vector2& i_size) const
{
    firstPassFBO->size = i_size;
    secondPassFBO->size = i_size;
}

void LowRenderer::RenderPoint(const Vector3& i_pos, const Vector3& i_color, const float i_size) const
{
	static Point point;
	point.shader->Use();
	point.shader->SetUniform("uModel", Matrix4::Translate(i_pos));
	point.shader->SetUniform("uSize", i_size);
	point.shader->SetUniform("uColor", i_color);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(point.VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
    Resources::Shader::Unuse();
}

void LowRenderer::RenderMeshOnce(const unsigned int i_VAO, const unsigned int i_count, const unsigned int i_texture) const
{
	glBindTextureUnit(0, i_texture);
	glBindTextureUnit(1, depthMapFBO->GetDepthMap());

	glBindVertexArray(i_VAO);
	glDrawArrays(GL_TRIANGLES, 0, i_count);

	// unbind
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 0);

	glBindVertexArray(0);
}

void LowRenderer::RenderMeshOnceOutline(const unsigned int i_VAO, const unsigned int i_count) const
{
	if (postProcessOutline)
		return;

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glLineWidth(10);

	glBindVertexArray(i_VAO);
	glDrawArrays(GL_TRIANGLES, 0, i_count);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// unbind
	glBindTextureUnit(0, 0);
	glBindVertexArray(0);
}

void LowRenderer::RenderLines(const unsigned int i_VAO, const unsigned int i_count, const Matrix4& i_model,
                              const float i_size, const Vector3& i_color, bool i_useLineStrip)
{
    static Point line;
    line.shader->Use();
    line.shader->SetUniform("uModel", i_model);
    line.shader->SetUniform("uSize", i_size);
    line.shader->SetUniform("uColor", i_color);

    glBindVertexArray(i_VAO);
	glLineWidth(i_size);
    glDrawArrays(i_useLineStrip ? GL_LINE_STRIP : GL_LINES, 0, i_count);

    // unbind
    glBindVertexArray(0);
    Resources::Shader::Unuse();
}

void LowRenderer::RenderPostProcess() const
{
	if (!outline && !postProcessOutline)
		return;

	static ScreenQuad postprocessQuad(true);
	postprocessQuad.shader->Use();
	glBindTextureUnit(0, firstPassFBO->GetColor0());
	glBindTextureUnit(1, firstPassFBO->GetDepthStencilMap());
	glBindVertexArray(postprocessQuad.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	Resources::Shader::Unuse();
}

void LowRenderer::RenderFinalScreen(const LowLevel::Framebuffer& i_fbo) const
{
	BeginFrame(i_fbo);

	RenderFinalScreen();

	EndFrame();
}

void LowRenderer::RenderFinalScreen() const
{
	static ScreenQuad finalQuad;
	finalQuad.shader->Use();
	glBindTextureUnit(0, firstPassFBO->GetColor0());
	if (outline && postProcessOutline)
	{
		finalQuad.shader->SetUniform("postprocess", true);
		glBindTextureUnit(1, secondPassFBO->GetColor0());
	}
	else
		finalQuad.shader->SetUniform("postprocess", false);

    glBindVertexArray(finalQuad.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
    Resources::Shader::Unuse();
}