#include "pch.h"
#include "Application.h"

Prism::Application::Application()
{
	PR_CORE_TRACE("Application constructed.");
}

Prism::Application::~Application()
{
	PR_CORE_TRACE("Application destructed.");
}

void Prism::Application::Run()
{
	PR_CORE_TRACE("Application running.");
}