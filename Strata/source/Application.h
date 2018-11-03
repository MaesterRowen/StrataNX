#pragma once
#include "Core.h"

namespace Strata
{
	class STRATA_API Application
	{
	protected:
		Application();

	public:
		virtual ~Application();
		void Run();
	};

	// To be defined by client
	Application *CreateApplication();
}