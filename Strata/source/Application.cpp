#include "Application.h"

#ifndef ENABLE_NXLINK
#define TRACE(fmt,...) ((void)0)
#else
#include <iostream>
#include <unistd.h>

#define TRACE(fmt,...) printf("%s: " fmt "\n", __PRETTY_FUNCTION__, ## __VA_ARGS__)
#endif

//-----------------------------------------------------------------------------
// EGL initialization
//-----------------------------------------------------------------------------

static EGLDisplay s_display;
static EGLContext s_context;
static EGLSurface s_surface;

static bool initEgl()
{
	// Connect to the EGL default display
	s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!s_display)
	{
		TRACE("Could not connect to display! error: %d", eglGetError());
		goto _fail0;
	}

	// Initialize the EGL display connection
	eglInitialize(s_display, nullptr, nullptr);

	// Select OpenGL (Core) as the desired graphics API
	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
	{
		TRACE("Could not set API! error: %d", eglGetError());
		goto _fail1;
	}

	// Get an appropriate EGL framebuffer configuration
	EGLConfig config;
	EGLint numConfigs;
	static const EGLint framebufferAttributeList[] =
	{
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_NONE
	};
	eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
	if (numConfigs == 0)
	{
		TRACE("No config found! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL window surface
	s_surface = eglCreateWindowSurface(s_display, config, (char*)"", nullptr);
	if (!s_surface)
	{
		TRACE("Surface creation failed! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL rendering context
	static const EGLint contextAttributeList[] =
	{
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
		EGL_CONTEXT_MINOR_VERSION_KHR, 3,
		EGL_NONE
	};
	s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
	if (!s_context)
	{
		TRACE("Context creation failed! error: %d", eglGetError());
		goto _fail2;
	}

	// Connect the context to the surface
	eglMakeCurrent(s_display, s_surface, s_surface, s_context);
	return true;

_fail2:
	eglDestroySurface(s_display, s_surface);
	s_surface = nullptr;
_fail1:
	eglTerminate(s_display);
	s_display = nullptr;
_fail0:
	return false;
}

static void deinitEgl()
{
	if (s_display)
	{
		eglMakeCurrent(s_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (s_context)
		{
			eglDestroyContext(s_display, s_context);
			s_context = nullptr;
		}
		if (s_surface)
		{
			eglDestroySurface(s_display, s_surface);
			s_surface = nullptr;
		}
		eglTerminate(s_display);
		s_display = nullptr;
	}
}

//-----------------------------------------------------------------------------
// Main program
//-----------------------------------------------------------------------------

static void setMesaConfig()
{
	// Uncomment below to disable error checking and save CPU time (useful for production):
	//setenv("MESA_NO_ERROR", "1", 1);

	// Uncomment below to enable Mesa logging:
	//setenv("EGL_LOG_LEVEL", "debug", 1);
	//setenv("MESA_VERBOSE", "all", 1);
	//setenv("NOUVEAU_MESA_DEBUG", "1", 1);

	// Uncomment below to enable shader debugging in Nouveau:
	//setenv("NV50_PROG_OPTIMIZE", "0", 1);
	//setenv("NV50_PROG_DEBUG", "1", 1);
	//setenv("NV50_PROG_CHIPSET", "0x120", 1);
}


namespace Strata
{
	Application::Application()
	{ 
	}

	Application::~Application()
	{
	}

	void Application::Initialize()
	{

	}

	void Application::Update(float deltaTime)
	{

	}

	void Application::Run()
	{
		//// Set mesa configuration (useful for debugging)
		setMesaConfig();

		// Initialize EGL
		if (!initEgl())
			return;

		// Load OpenGL routines using glad 
		gladLoadGL();

		// Call our initialize
		Initialize();

		Time.Initialize();

		while (appletMainLoop())
		{
			// Get and process input
			hidScanInput();
			u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS)
				break;

			Time.UpdateTime();
			float elapsedTime = Time.GetElapsedTime();

			// Update
			Update(elapsedTime);

			glClearColor(91.0f/255.0f, 142.0f/255.0f, 193.0f/255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			eglSwapBuffers(s_display, s_surface);

			// Tag the end of our frame
			Time.EndFrame();
		}

		// Cleanup
		deinitEgl();
	}
}