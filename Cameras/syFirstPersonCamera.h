#pragma once
#include "syCamera.h"
class syFirstPersonCamera : public syCamera
{
public:
	syFirstPersonCamera();
	virtual ~syFirstPersonCamera();
public:
	bool  Frame();
};

