
#ifndef FILTER_GL_H_
#define FILTER_GL_H_

#include <cassert>
#include <algorithm>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#define __gl2_h_
#include <GLES2/gl2ext.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <ctime>
#include <opencv2/opencv.hpp>

#if NDEBUG
#define	CHECK_GL_ERROR(target)	\
	if (glGetError() != 0) {throw std::exception("OpenGL error: " target);}
#else
#define	CHECK_GL_ERROR(target)	\
	assert(glGetError() == 0);
#endif

// waifu2x shader
struct Waifu2xShader
{
	GLuint program;
	GLuint a_position;
	GLuint a_texCoord;
	
	GLuint bias;
	GLuint weightMatrix;
	GLuint inputTextures;
};

void filterGLInit(uint32_t width, uint32_t height);

void filterGLRelease();

void filterGLSetInputData(cv::Mat& inputPlane);

void filterGLGetOutputData(cv::Mat& outputPlane);

bool filterGLProcess(Waifu2xShader& shader, 
	int nInputPlanes, int nOutputPlanes,
	std::vector<cv::Mat> &weightMatrices, 
	std::vector<double> &biases, int modelIndex);

#endif
