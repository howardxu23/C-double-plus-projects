// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week2App.h"

int main()
{
	Week2App *app = new Week2App();
	SOF::Window::CreateWindow(1024, 768, "Week2", app);
	SOF::Window::Run();

    return 0;
}

