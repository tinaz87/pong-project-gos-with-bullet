#pragma once
#include <string>
#include "dataTypes.h"

class Bumper
{
public:
	Bumper(const std::string& bumperId, const vector3& posOffset, bool setKinematic);
	~Bumper();

private:
};