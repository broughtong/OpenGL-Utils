/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3

	texture.cpp

	Header file for container class for openGL texture class

	Simply create class, call loadTexture providing file name as parameters, then bind() to use
	Don't forget to call deleteTexture() at the end
*/

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "loadTexture.h"
#include <string>

using namespace std;

//loadTexture
//parameter is the filename of the texture to be loaded
//returns true if the texture was loaded successfully
//if the function returns false, the cause of the error may be ascertained by calling the getError() function, which returns a string
bool Texture::loadTexture(const char* fileName)
{
	SDL_Surface *surface = NULL;
	surface = SDL_LoadBMP(fileName);

	if(surface != NULL)
	{
		Uint32 rmask, gmask, bmask, amask;

		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		}
		else
		{
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		}

		SDL_Surface* optimizedSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
		
		if(optimizedSurface == NULL)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating surface", SDL_GetError(), NULL);
			SDL_FreeSurface(surface);
			return false;
		}
		
		if(SDL_BlitSurface(surface, NULL, optimizedSurface, NULL) == 0)
		{
			GLint colours = optimizedSurface->format->BytesPerPixel;
			GLenum textureFormat;

			if(colours == 4)
			{
				if(optimizedSurface->format->Rmask == 0x000000ff)
				{
					textureFormat = GL_RGBA;
				}
				else
				{
					textureFormat = GL_BGRA;
				}
			}
			else if(colours == 3)
			{
				if(optimizedSurface->format->Rmask == 0x000000ff)
				{
					textureFormat = GL_RGB;
				}
				else
				{
					textureFormat = GL_BGR;
				}
			}
			
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, optimizedSurface->w, optimizedSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, optimizedSurface->pixels);
		
			SDL_FreeSurface(optimizedSurface);
			SDL_FreeSurface(surface);		
		}
		else
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error blitting surface", SDL_GetError(), NULL);
			SDL_FreeSurface(surface);
			return false;
		}
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading texture", SDL_GetError(), NULL);
		return false;
	}

	return true;
}

//deleteTexture
//Used to delete the texture once it is no longer of use
void Texture::deleteTexture()
{
	unbind();
	glDeleteTextures(1, &texture);
	isTexture = false;
}

//bind
//binds the texture to the current opengl context
//if no texture exists, this function does nothing
void Texture::bind()
{
	if(isTexture == true)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

//unbind
//unbinds the current texture from the opengl context
void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
	
//getHandle
//Returns a handle to the current texture
//returns 0 if no texture exists
GLuint Texture::getHandle()
{
	if(isTexture == true)
	{
		return texture;
	}

	return 0;
}
	
//getError
//returns error string generated during loadTexture function
//error string refers to why the texture could not be created
string Texture::getError()
{
	return error;
}
