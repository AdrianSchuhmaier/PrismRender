#include "pch.h"
#include "Application.h"

Prism::Application::Application()
{
	std::cout << "Application constructed.\n";
}

Prism::Application::~Application()
{
	std::cout << "Application destructed.\n";
}

void Prism::Application::Run()
{
	std::cout << "Application running.\n";
}