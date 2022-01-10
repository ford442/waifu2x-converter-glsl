
#include <cstdio>
#include <exception>
#include "filterGL.h"

struct FilterVertex
{
	float x, y;
	float tu, tv;
};

static GLuint frameBuffer = 0;
static GLuint textureBuffers[2] = {0};
static cv::Size planeSize;
using namespace std;
using namespace std::chrono;
static high_resolution_clock::time_point t1,t2,t3;
static GLuint EBO,VBO,CBO,tex2d[4],shader_program,shader,frame,attrib_position,sampler_channel[4];
static GLuint uniform_dtime,uniform_fps,uniform_date,VCO,ECO,CCO,vtx,frag,uniform_frame,uniform_time,uniform_res,uniform_mouse;
static long double Ttime,Dtime;
static EGLDisplay display;
static EGLSurface surface;
static EGLContext contextegl;
static GLsizei nsources,i,S;
static GLsizei s4=4;
static EGLint config_size,major,minor;
static EGLConfig eglconfig=NULL;
static EmscriptenWebGLContextAttributes attr;
static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx;
static EGLint v0=0,v1=1,v2=2,v3=3,v4=4,v6=6,v8=8,v32=32,a,b;
struct timespec rem;
struct timespec req={0,25000000};
static GLfloat F=1.0f;
static GLfloat F0=0.0f;
static GLfloat Fm1=-1.0f;
static GLfloat F2=2.0f;
static GLfloat Fm2=-2.0f;

void filterGLInit(uint32_t width, uint32_t height)
{

	S=EM_ASM_INT({return parseInt(document.getElementById('pmhig').innerHTML,10);});
static const EGLint attribut_list[]={EGL_NONE};
static const EGLint anEglCtxAttribs2[]={
EGL_CONTEXT_CLIENT_VERSION,v3,
// EGL_COLOR_COMPONENT_TYPE_EXT,EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT,
EGL_CONTEXT_PRIORITY_LEVEL_IMG,EGL_CONTEXT_PRIORITY_REALTIME_NV,
EGL_NONE};
static const EGLint attribute_list[]={
// EGL_COLOR_COMPONENT_TYPE_EXT,EGL_COLOR_COMPONENT_TYPE_FLOAT_EXT,
// EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR,
EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT,
EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT,EGL_TRUE,
EGL_DEPTH_ENCODING_NV,EGL_DEPTH_ENCODING_NONLINEAR_NV,
EGL_RENDER_BUFFER,EGL_QUADRUPLE_BUFFER_NV,
EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE,EGL_TRUE,
EGL_RED_SIZE,v32,
EGL_GREEN_SIZE,v32,
EGL_BLUE_SIZE,v32,
EGL_ALPHA_SIZE,v32,
EGL_DEPTH_SIZE,v32,
EGL_STENCIL_SIZE,v32,
EGL_BUFFER_SIZE,v32,
EGL_NONE
};
// emscripten_webgl_init_context_attributes(&attr);
attr.alpha=EM_TRUE;
attr.stencil=EM_TRUE;
attr.depth=EM_TRUE;
attr.antialias=EM_FALSE;
attr.premultipliedAlpha=EM_FALSE;
attr.preserveDrawingBuffer=EM_FALSE;
attr.enableExtensionsByDefault=EM_FALSE;
attr.powerPreference=EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
attr.failIfMajorPerformanceCaveat=EM_FALSE;
attr.majorVersion=v2;
attr.minorVersion=v0;
ctx=emscripten_webgl_create_context("#canvas",&attr);
emscripten_webgl_enable_extension(ctx,"EXT_color_buffer_float");
display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
eglInitialize(display,&v3,&v0);
eglChooseConfig(display,attribute_list,&eglconfig,1,&config_size);
eglBindAPI(EGL_OPENGL_ES_API);
contextegl=eglCreateContext(display,eglconfig,EGL_NO_CONTEXT,anEglCtxAttribs2);
surface=eglCreateWindowSurface(display,eglconfig,NULL,attribut_list);
eglMakeCurrent(display,surface,surface,contextegl);
emscripten_webgl_make_context_current(ctx);
glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT,GL_NICEST);
glViewport(0,0,S,S);
glDisable(GL_DITHER);
glDisable(GL_STENCIL_TEST);
glDisable(GL_DEPTH_TEST);
glClearColor(F0,F0,F0,F);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
glGenTextures(2, textureBuffers);
glGenTextures(2, textureBuffers);
for (int i = 0; i < 2; i++) {
glBindTexture(GL_TEXTURE_2D_ARRAY, textureBuffers[i]);
glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R32F, width, height, 128, 0, GL_RED, GL_FLOAT, nullptr);
}
glGenFramebuffers(1, &frameBuffer);
glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void filterGLRelease()
{
	glDeleteFramebuffers(1, &frameBuffer);
	frameBuffer = 0;
	glDeleteTextures(2, textureBuffers);
	memset(textureBuffers, 0, sizeof(textureBuffers));
}

void filterGLSetInputData(cv::Mat& inputPlane)
{
	planeSize = inputPlane.size();
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureBuffers[0]);
	void *pixels = inputPlane.data;
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 
	planeSize.width, planeSize.height, 1, GL_RED, GL_FLOAT, pixels);
}

void filterGLGetOutputData(cv::Mat& outputPlane)
{
	cv::Size opSize = outputPlane.size();
	GLuint outputPixelBuffer = 0;
	glGenBuffers(1, &outputPixelBuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, outputPixelBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, planeSize.width * planeSize.height * sizeof(float), 0, GL_DYNAMIC_DRAW);
	glReadPixels(0, 0, planeSize.width, planeSize.height, GL_RED, GL_FLOAT, 0);
	void *resultAddr = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	memcpy(outputPlane.data, resultAddr, opSize.width * opSize.height * sizeof(float));
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glDeleteBuffers(1, &outputPixelBuffer);
}

bool filterGLProcess(Waifu2xShader& shader, 
	int nInputPlanes, int nOutputPlanes,
	std::vector<cv::Mat> &weightMatrices, 
	std::vector<double> &biases, int modelIndex)
{
	GLuint inputTextures  = textureBuffers[(modelIndex + 0) % 2];
	GLuint outputTextures = textureBuffers[(modelIndex + 1) % 2];
	const FilterVertex vertices[4] = {
		{-1.0f,  1.0f, 0.0f, 1.0f},
		{-1.0f, -1.0f, 0.0f, 0.0f},
		{ 1.0f,  1.0f, 1.0f, 1.0f},
		{ 1.0f, -1.0f, 1.0f, 0.0f},
	};
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FilterVertex) * 4, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.a_position);
	glVertexAttribPointer(shader.a_position, 2, GL_FLOAT, GL_FALSE, sizeof(FilterVertex), (void*)0);
	glEnableVertexAttribArray(shader.a_texCoord);
	glVertexAttribPointer(shader.a_texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(FilterVertex), (void*)8);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(shader.program);
	glBindVertexArray(vao);
	float vWeightMatrices[3 * 3 * 128];
	for (int opIndex = 0; opIndex < nOutputPlanes; opIndex++) {
glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outputTextures, 0, opIndex);
glViewport(0, 0, planeSize.width, planeSize.height);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D_ARRAY, inputTextures);
glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glUniform1i(shader.inputTextures, 0);
glUniform1f(shader.bias, (float)biases[opIndex]);
		for (int ipIndex = 0; ipIndex < nInputPlanes; ipIndex++) {
			memcpy(&vWeightMatrices[ipIndex * 3 * 3], 
				weightMatrices[opIndex * nInputPlanes + ipIndex].data,
				3 * 3 * sizeof(float));
		}
		glUniform3fv(shader.weightMatrix, 3 * nInputPlanes, vWeightMatrices);
		glDisable(GL_BLEND);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vao);
	return true;
}
