#include <iostream>

#include <Windows.h>
#include "Apps/ApplicationFactory.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		const auto appFactory = ApplicationFactory();

		auto app = appFactory.Create(APP_ID::MODELS_RENDERING);
		app->Init("Test Window", 0, 0, 1920, 1080);

		return app->Start();
	}
	catch (const std::exception & exception)
	{
		MessageBox(nullptr, exception.what(), "ERROR", MB_OK);
	}

	return -1;
}