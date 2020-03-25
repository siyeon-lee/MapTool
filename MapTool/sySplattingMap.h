#pragma once
#include"syMap.h"
#include "syDxRT.h"
class sySplattingMap : public syMap
{
public:
	sySplattingMap();
	virtual ~sySplattingMap();
public:
	ComPtr<ID3D11ShaderResourceView>	m_pMapAlphaTex;
	syDxRT							m_alphaRT;
public:
	HRESULT LoadTextures(ID3D11Device* pd3dDevice, const TCHAR* pLoadTextureFile);
	HRESULT SetInputLayout();
};

