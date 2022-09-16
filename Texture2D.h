#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

class Texture2D 
{
public:
	unsigned int ID;
	int width, height, nrChannels;
	Texture2D(const char* name, bool flip = false, int wrappingMode = GL_REPEAT, int textureFiltersMin = GL_LINEAR_MIPMAP_LINEAR, int textureFiltersMag = GL_LINEAR, int rgb = GL_RGB, float borderColor[] = {})
	{
		// create texture object
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);
		if (wrappingMode == GL_TEXTURE_BORDER_COLOR) {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		
		// load image, create texture and generate mipmaps
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(name, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
		}
		stbi_image_free(data);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFiltersMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFiltersMag);
	}
	void bind() 
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};

#endif