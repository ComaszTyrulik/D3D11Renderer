#pragma once
#include <string>

class IApplication
{
public:
	virtual ~IApplication() = default;

	virtual void Init(const std::string& name, int positionX, int positionY, int width, int height) = 0;
	virtual int Start() = 0;
};