// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week4App.h"

int main()
{
	Week4App *app = new Week4App();
	SOF::Window::CreateWindow(1024, 768, "Week4", app);
	SOF::Window::Run();

    return 0;
}

