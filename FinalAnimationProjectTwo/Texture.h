#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include "Object.h"
#include "stb_image.h"

class Texture
{
public:
	std::string m_file_path;
	int width;
	int height;
	int bpp;
	GLuint texture; 
	unsigned char* image;
public:
	Texture() {};
	~Texture() {};
	void init(std::string path)
	{
		int width, height, bpp;
		unsigned char* image;
		glGenTextures(1, &texture);
		image = stbi_load(path.c_str(), &width, &height, &bpp, 4);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(image);

	};
	void Bind(Object *o) 
	{
		glBindVertexArray(o->vao); 
		glActiveTexture(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	};

};