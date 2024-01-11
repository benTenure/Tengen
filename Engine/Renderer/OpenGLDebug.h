#pragma once

// Super old school way of debugging
#ifdef FALSE

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		const char* errorString;
		switch (error) {
		case GL_NO_ERROR:
			errorString = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			errorString = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errorString = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errorString = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errorString = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errorString = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errorString = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
			// Additional error codes for OpenGL 4.3 and above
		case GL_CONTEXT_LOST:
			errorString = "GL_CONTEXT_LOST";
			break;
		default:
			errorString = "Unknown OpenGL Error";
			break;
		}

		std::cerr << "OpenGL Error: " << errorString << " in file " << file << " at line " << line << std::endl;

}
#endif
