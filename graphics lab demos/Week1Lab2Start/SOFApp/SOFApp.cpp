// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week1App.h"

int main()
{
	Week1App *app = new Week1App();
	SOF::Window::CreateWindow(1024, 768, "Week1", app);
	SOF::Window::Run();

    return 0;
}

