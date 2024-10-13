#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Texture
{
public:
	unsigned int id;
	std::string type;
	aiString path;

	static unsigned int TextureFromFile(const char* file, const std::string& dir = "")
	{
		return GetTextureFromFile(file, dir);
	}

	static unsigned int TransparentTextureFromFile(const char* file, const std::string& dir = "")
	{
		return GetTextureFromFile(file, dir, GL_RGBA);
	}

	static unsigned int ColorAttachment(const int& screenWidth, const int& screenHeight)
	{
		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		return tex;
	}

	static unsigned int CubemapFromFiles(const char* (&files)[6])
	{
		unsigned int cubemap1;
		glGenTextures(1, &cubemap1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap1);


		int width, height, channelsNum;
		for (unsigned int i = 0; i < 6; i++)//number of cube faces is 6
		{
			unsigned char* data = stbi_load(files[i], &width, &height, &channelsNum, 0);
			if (data)
			{
				//internalFormat argument: how opengl is going to store the texture data
				//format argument: format of the pixel data provided
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cerr << "Failed to load texture " << files[i];
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return cubemap1;
	}

private:
	static unsigned int GetTextureFromFile(const char* file, const std::string& dir = "", GLenum format = GL_RGB)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channelsNum;
		std::string path = dir + (dir == "" ? "" : "/") + file;
		unsigned char* texData = stbi_load(path.c_str(), &width, &height, &channelsNum, 0);


		if (texData)
		{
			//internalFormat argument: how opengl is going to store the texture data
			//format argument: format of the pixel data provided
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(texData);
		}
		else
		{
			std::cerr << "Failed to load texture " << file;
		}

		//operations done on buffer are stored in the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
};

#endif // !TEXTURE_H

