#pragma once

#include "TaskSystem/TaskSystem.h"
#include "Log/Log.h"

#include "Application.h"

// Creating an entry point:
// PRISM_ENTRY_POINT { return your new Prism::Application here }
#define PRISM_ENTRY_POINT Prism::Application* Prism::CreateApplication()

extern Prism::Application* Prism::CreateApplication();

int main(int argc, char** argv)
{
	Prism::Log::Init();
	Prism::TaskSystem::Init();

	PR_CORE_HEAD("Creating application...");
	Prism::Application* app = Prism::CreateApplication();

	PR_CORE_HEAD("Running application...");
	app->Run();

	Prism::TaskSystem::Finish();
	delete app;
	return 0;
}