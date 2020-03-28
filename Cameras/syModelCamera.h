#pragma once
#include "syCamera.h"
class syModelCamera : public syCamera
{
public:
	syModelCamera();
	virtual ~syModelCamera();
public:
	bool  Frame();
public:
	D3DXMATRIX			m_mModelLastRot;
	D3DXMATRIX			m_mModelRot;
};

 