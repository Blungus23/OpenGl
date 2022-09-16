#ifndef TEXTUREBuffer_H
#define TEXTUREBUFFER_H

#include <glad/glad.h>
#include <iostream>

class TextureBuffer
{
public:
	unsigned int ID;
	unsigned int colorAttachment;
	TextureBuffer(int textureWidth, int textureHeight,unsigned int _colorAttatchment)
	{
		// generate texture
		colorAttachment = _colorAttatchment;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D,  ID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, _colorAttatchment, GL_TEXTURE_2D, ID, 0);
	}
	void bind()
	{
		glBindTexture(GL_TEXTURE_2D,  ID);
	}
	void resize(int WIDTH, int HEIGHT) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
};

#endif