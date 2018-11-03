#pragma once

#ifdef ST_PLATFORM_WINDOWS


#else 
//-----------------------------------------------------------------------------
// nxlink support
//-----------------------------------------------------------------------------
#ifndef ENABLE_NXLINK
#define TRACE(fmt,...) ((void)0)
#else
#include <switch.h>
#include <iostream>
#include <unistd.h>
#define TRACE(fmt,...) printf("%s: " fmt "\n", __PRETTY_FUNCTION__, ## __VA_ARGS__)

static int s_nxlinkSock = -1;

static void initNxLink()
{
	if (R_FAILED(socketInitializeDefault()))
		return;

	s_nxlinkSock = nxlinkStdio();
	if (s_nxlinkSock >= 0)
		TRACE("printf output now goes to nxlink server");
	else
		socketExit();
}

static void deinitNxLink()
{
	if (s_nxlinkSock >= 0)
	{
		close(s_nxlinkSock);
		socketExit();
		s_nxlinkSock = -1;
	}
}

extern "C" void userAppInit()
{
	initNxLink();
}

extern "C" void userAppExit()
{
	deinitNxLink();
}
#endif

extern Strata::Application* Strata::CreateApplication();

int main()
{
	// Initialize romfs
	romfsInit();

	// Create an instance of our application and run
	auto app = Strata::CreateApplication();
	app->Run();
	delete app;

	// Cleanup romfs
	romfsExit();

	// Return
	return EXIT_SUCCESS;
}

#endif