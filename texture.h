/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3
  
	texture.h
  
	Header file for container class for openGL texture class
  
	Simply create class, call loadTexture providing file name as parameters, then bind() to use
	Don't forget to call deleteTexture() at the end
*/

#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

using namespace std;

class Texture
{

public:
	
	//loadTexture
	//parameter is the filename of the texture to be loaded
	//returns true if the texture was loaded successfully
	//if the function returns false, the cause of the error may be ascertained by calling the getError() function, which returns a string
	bool loadTexture(const char* fileName);

	//deleteTexture
	//Used to delete the texture once it is no longer of use
	void deleteTexture();

	//bind
	//binds the texture to the current opengl context
	//if no texture exists, this function does nothing
	void bind();

	//unbind
	//unbinds the current texture from the opengl context
	void unbind();
	
	//getHandle
	//Returns a handle to the current texture
	//returns 0 if no texture exists
	GLuint getHandle();
	
	//getError
	//returns error string generated during loadTexture function
	//error string refers to why the texture could not be created
	string getError();

private:

	//Handle to texture
	GLuint texture;

	//If shader program currently exists
	bool isShader;

	//Contains error messages from compilation
	string error;
};
