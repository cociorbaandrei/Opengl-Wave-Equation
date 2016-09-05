#ifndef GLSHADER_H
#define GLSHADER_H
#pragma once

class GLShader
{
public:
	GLShader(const char *vertex_path, const char *fragment_path);
	~GLShader();
	void useProgram();
	void stopUsing();
	GLuint getProgram();
private:
	std::string readFile(const char *filePath);
	GLuint _program;
};

#endif