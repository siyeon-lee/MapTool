#pragma once
#include "syCamera.h"
class syBackViewCamera : public syCamera
{
public:
	syBackViewCamera();
	virtual ~syBackViewCamera();
public:
	bool  Frame();
};

