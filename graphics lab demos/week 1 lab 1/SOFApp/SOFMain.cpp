// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ColourCubeApp.h"

int main()
{
	ColourCubeApp*app = new ColourCubeApp();
	SOF::Window::CreateWindow(1024, 768, "SOF Template", app);
	SOF::Window::Run();

    return 0;
}

