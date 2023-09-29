#include "../anm/texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

unsigned int loadTexture(const char* filepath, int wrapMode, int filterMode)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numComps;
	unsigned char* data = stbi_load(filepath, &width, &height, &numComps, 0);
	if (data == NULL) {
		printf("Failed to load image %s", filepath);
		stbi_image_free(data);
		return 0;
	}

	unsigned int texture;
	GLint glFormat[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
	glGenTextures(1, &texture); // new texture name
	glBindTexture(GL_TEXTURE_2D, texture); // binds name and creates a new 2d texture
	//V reserve memory and set the data of a texture V
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat[numComps - 1], width, height, 0, glFormat[numComps - 1], GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return texture;
};