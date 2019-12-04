#include <iostream>

#include <Windows.h>
#include "Apps/ApplicationFactory.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const int WIDTH = 1920;
	const int HEIGHT = 1080;

	try {
		const auto appFactory = ApplicationFactory();

		auto app = appFactory.Create(APP_ID::DYNAMIC_LIGHTING);
		app->Init("Test Window", 1920 / 2 - WIDTH / 2, 1080 / 2 - HEIGHT / 2, WIDTH, HEIGHT);

		return app->Start();
	}
	catch (const std::exception & exception)
	{
		MessageBox(nullptr, exception.what(), "ERROR", MB_OK);
	}

	return -1;
}