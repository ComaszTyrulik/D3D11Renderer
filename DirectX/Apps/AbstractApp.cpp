#include "AbstractApp.h"

int AbstractApp::Start()
{
	while (true)
	{
		if (const auto result = m_window->ProcessMessages())
		{
			return result;
		}

		DoFrame();
	}

	return 0;
}
