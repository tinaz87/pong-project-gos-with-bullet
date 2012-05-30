#pragma once
#include <string>

class Background
{
public:
	Background(const char* backgroundFilename);
	~Background();

private:
	std::string m_backgroundFilename;
};