// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week3App1.h"

int main()
{
	Week3App1 *app = new Week3App1();
	SOF::Window::CreateWindow(1024, 768, "Week3", app);
	SOF::Window::Run();

    return 0;
}

