/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3
  
	Shader.h
  
	Header file for container class for openGL shader programs
	Includes tessellation shaders, so opengl v. 4.0+
  
	Simply create class, call loadShader providing file names as parameters, then bind() to use
	Don't forget to call deleteShader() at the end
*/

#pragma once
#include <string>

using namespace std;

class Shader
{

public:

	//functions to create shader from file and delete
	bool loadShader(const char* vFile, const char* tcFile, const char* teFile, const char* gFile, const char* fFile);
	void deleteShader();

	void bind(); //bind shader to context
	void unbind(); //unbind shader
	GLuint handle(); //returns shader program handle

	string getError(); //returns error messages from compilation
	
private:
  
	//Handles to shaders
	//Vertex, tessellation control, tessellation evaluator, geometry and fragment
	GLuint vShader;
	GLuint tcShader;
	GLuint teShader;
	GLuint gShader;
	GLuint fShader;
	
	//Handle to shader program
	GLuint hProgram;

	//Contains error messages from compilation
	string error;

	//Function returns contents of shader file as string
	string readFile(const char* fileName);
};
