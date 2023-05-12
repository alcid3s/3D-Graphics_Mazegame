#include "Texture.h"
#include "stb/stb_image.h"

Texture::Texture(const std::string& path)
	: t_RendererID(0), t_Path(path), t_LocalBuffer(nullptr),
	t_Width(0), t_Height(0), t_BPP(0)
{
	// flips texture vertically. Bottom left of image is 0,0 for OpenGL
	stbi_set_flip_vertically_on_load(1);
	t_LocalBuffer = stbi_load(path.c_str(), &t_Width, &t_Height, &t_BPP, 4);

	glGenTextures(1, &t_RendererID);
	glBindTexture(GL_TEXTURE_2D, t_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// S and T are like X and Y for textures.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t_Width, t_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (t_LocalBuffer) {
		stbi_image_free(t_LocalBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &t_RendererID);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, t_RendererID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}