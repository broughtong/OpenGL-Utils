/*
	OpenGL-Utils
	Copyright (C) 2015 George Broughton
	GNU GPL Version 3
  
	Shader.cpp
  
	C++ file for container class for openGL shader programs
	Includes tessellation shaders, so opengl v. 4.0+
  
	Simply create class, call loadShader providing file names as parameters, then bind() to use
	Don't forget to call deleteShader() at the end
*/

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "shader.h"
#include <string>
#include <fstream>

using namespace std;

//loadShader
//parameters are filenames to source files for vertex, tessellation control, tessellation evaluator, geometry and fragment shaders
//returns true if shader program was successfully created
//if the function returns false, the cause of the error may be ascertained by calling the getError() function, which returns a string
bool Shader::loadShader(const char* vFile, const char* tcFile, const char* teFile, const char* gFile, const char* fFile)
{
	error = "";

	vShader = glCreateShader(GL_VERTEX_SHADER);
	tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	gShader = glCreateShader(GL_GEOMETRY_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	if(vShader == 0)
	{
		error += "Error creating vertex shader\n";
		return false;
	}
	if(tcShader == 0)
	{
		error += "Error creating tessellation control shader\n";
		return false;
	}
	if(teShader == 0)
	{
		error += "Error creating tessellation evaluator shader\n";
		return false;
	}
	if(gShader == 0)
	{
		error += "Error creating geometry shader\n";
		return false;
	}
	if(fShader == 0)
	{
		error += "Error creating fragment shader\n";
		return false;
	}

	string vTextString = readFile(vFile);
	string tcTextString = readFile(tcFile);
	string teTextString = readFile(teFile);
	string gTextString = readFile(gFile);
	string fTextString = readFile(fFile); 

	if(vTextString.empty()) return false;
	if(tcTextString.empty()) return false;
	if(teTextString.empty()) return false;
	if(gTextString.empty()) return false;
	if(fTextString.empty()) return false;

	const char* vText = vTextString.c_str();
	const char* tcText = tcTextString.c_str();
	const char* teText = teTextString.c_str();
	const char* gText = gTextString.c_str();
	const char* fText = fTextString.c_str();

	glShaderSource(vShader, 1, &vText, NULL);
	glShaderSource(tcShader, 1, &tcText, NULL);
	glShaderSource(teShader, 1, &teText, NULL);
	glShaderSource(gShader, 1, &gText, NULL);
	glShaderSource(fShader, 1, &fText, NULL);

	glCompileShader(vShader);
	glCompileShader(tcShader);
	glCompileShader(teShader);
	glCompileShader(gShader);
	glCompileShader(fShader);

	GLint vStatus;
	GLint tcStatus;
	GLint teStatus;
	GLint gStatus;
	GLint fStatus;

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vStatus);
	glGetShaderiv(tcShader, GL_COMPILE_STATUS, &tcStatus);
	glGetShaderiv(teShader, GL_COMPILE_STATUS, &teStatus);
	glGetShaderiv(gShader, GL_COMPILE_STATUS, &gStatus);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fStatus);

	if(vStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetShaderInfoLog(vShader, logLength, 0, compilerLog);
		error += "Error compiling vertex shader: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}
	if(tcStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(tcShader, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetShaderInfoLog(tcShader, logLength, 0, compilerLog);
		error += "Error compiling tessellation control shader: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}
	if(teStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(teShader, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetShaderInfoLog(teShader, logLength, 0, compilerLog);
		error += "Error compiling tessellation evaluator shader: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}
	if(gStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(gShader, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetShaderInfoLog(gShader, logLength, 0, compilerLog);
		error += "Error compiling geometry shader: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}
	if(fStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetShaderInfoLog(fShader, logLength, 0, compilerLog);
		error += "Error compiling fragment shader: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}

	hProgram = glCreateProgram();
	if(hProgram == 0)
	{
		error += "Error creating shader program: shader returned ID 0\n";
		return false;
	}

	glAttachShader(hProgram, vShader);
	glAttachShader(hProgram, tcShader);
	glAttachShader(hProgram, teShader);
	glAttachShader(hProgram, gShader);
	glAttachShader(hProgram, fShader);

	glLinkProgram(hProgram);

	GLint linkStatus;
	glGetProgramiv(hProgram, GL_LINK_STATUS, &linkStatus);

	if(linkStatus == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetProgramInfoLog(hProgram, logLength, 0, compilerLog);
		error += "Error linking shader program: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}

	glValidateProgram(hProgram);

	GLint programValidation;
	glGetProgramiv(hProgram, GL_VALIDATE_STATUS, &programValidation);

	if(programValidation == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH , &logLength);
		GLchar* compilerLog = new GLchar[logLength];
		glGetProgramInfoLog(hProgram, logLength, 0, compilerLog);
		error += "Error validating shader program: ";		
		error += compilerLog;
		error += "\n";
		delete compilerLog;
		return false;
	}

	isShader = true;

	return true;
}

//deleteShader
//Used to delete a shader program once it is no longer of use
void Shader::deleteShader()
{
	glDetachShader(hProgram, vShader);
	glDetachShader(hProgram, tcShader);
	glDetachShader(hProgram, teShader);
	glDetachShader(hProgram, gShader);
	glDetachShader(hProgram, fShader);

	glDeleteShader(vShader);
	glDeleteShader(tcShader);
	glDeleteShader(teShader);
	glDeleteShader(gShader);
	glDeleteShader(fShader);

	glDeleteProgram(hProgram);

	isShader = false;
}

//bind
//binds the shader program to the current opengl context
//if no shader program exists, this function does nothing
void Shader::bind()
{
	if(isShader == true)
	{
		glUseProgram(hProgram);
	}
}

//unbind
//unbinds the current shader program from the opengl context
void Shader::unbind()
{
	glUseProgram(0);
}

//getHandle
//Returns a handle to the current shader program
//returns 0 if no shader program exists
GLuint Shader::getHandle()
{
	if(isShader == true)
	{
		return hProgram;
	}

	return 0;
}

//getError
//returns error string generated during loadProgram or readFile(private, called by loadProgram) functions
//error string refers to why the shader program could not be created
string Shader::getError()
{
	return error;
}

//readFile
//returns contents of text file contained at fileName as string
//only called from loadProgram
//any errors opening file can be read using getError()
string Shader::readFile(const char* fileName)
{
	string content;
	ifstream fileStream(fileName, ios::in);

	if(!fileStream.is_open())
	{
		error += "Error opening file: ";
		error += fileName;
		error += "\n";
		return content;
	}

	string line = "";

	while(!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	
	fileStream.close();

	if(content.empty())
	{
		error += "File is empty: ";
		error += fileName;
		error += "\n";
	}

	return content;
}
