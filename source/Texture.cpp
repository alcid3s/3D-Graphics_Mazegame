#include "Texture.h"
#include "stb/stb_image.h"

Texture::Texture(const std::string& path)
	: t_TextureID(0), t_Path(path), t_Buffer(nullptr),
	t_Width(0), t_Height(0), t_BPP(0)
{
	// flips texture vertically. Bottom left of image is 0,0 for OpenGL
	stbi_set_flip_vertically_on_load(1);
	t_Buffer = stbi_load(path.c_str(), &t_Width, &t_Height, &t_BPP, 4);

	glGenTextures(1, &t_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t_Width, t_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_Buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// S and T are like X and Y for textures.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (t_Buffer) {
		stbi_image_free(t_Buffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &t_TextureID);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, t_TextureID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}