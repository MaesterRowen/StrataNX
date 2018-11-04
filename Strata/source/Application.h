#pragma once
#include "Core.h"
#include "TimeInfo.h"

#include <switch.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glad/glad.h>

namespace Strata
{
	class STRATA_API Application
	{
	protected:
		Application();

		virtual void Initialize();
		virtual void Update(float deltaTime);

	public:
		virtual ~Application();
		void Run();

		TimeInfo Time;
	};

	// To be defined by client
	Application *CreateApplication();
}