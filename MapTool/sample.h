#pragma once
#include "syCore.h"
#include "syQuadTree.h"
#include "syMap.h"
#include "syDxRT.h"
#include "sySelect.h"
struct SY_Obj : SY_BaseObj
{
	D3DXVECTOR3    vPos;
	D3DXMATRIX     matWorld;
	SY_Obj()
	{
		vPos.x = -20.0f + (rand() % 40);
		vPos.z = -20.0f + (rand() % 40);
		vPos.y = 0.0f;
		box.vMin.x = vPos.x - 1.0f;
		box.vMin.y = vPos.y - 1.0f;
		box.vMin.z = vPos.z - 1.0f;

		box.vMax.x = vPos.x + 1.0f;
		box.vMax.y = vPos.y + 1.0f;
		box.vMax.z = vPos.z + 1.0f;
		D3DXMatrixTranslation(&matWorld, vPos.x, vPos.y, vPos.z);
	}
};
struct LIGHT_CONSTANT_BUFFER
{
	D3DXMATRIX			g_matInvWorld;
	// Material
	D3DXVECTOR4			g_cAmbientMaterial;
	D3DXVECTOR4			g_cDiffuseMaterial;
	D3DXVECTOR4			g_cSpecularMaterial;
	D3DXVECTOR4			g_cEmissionMaterial;
	// Light
	D3DXVECTOR4			g_cAmbientLightColor;
	D3DXVECTOR4			g_cDiffuseLightColor;
	D3DXVECTOR4			g_cSpecularLightColor;
	D3DXVECTOR4			g_vLightDir; // w = light damping(attenuation) : °¨¼è
	D3DXVECTOR4			g_vLightPos; // w = light radius
	D3DXVECTOR4			g_vEyeDir;// w = light intensity : °­µµ
	D3DXVECTOR4			g_vEyePos;// w = light radius	
};

struct MAP_CONSTANT_BUFFER {
	D3DXVECTOR4 pickPos;	//w = brushRad
	//float		brushRad;
	float		mapLenth[4];
};
class sample : public syCore
{
public:
	sample();
	~sample();
public:
	std::vector<int>			nodeTexList;
	sySelect					m_Select;
	syMap						m_Map;
	syDxRT						m_MiniMapRT;
	syDxRT						m_RT;
	syDxRT						m_RTBrush;
	syDebugCamera				m_CameraBack;
	syQuadTreeIndex				m_Quadtree;
	syLineObject				m_Line;
	D3DXVECTOR4					m_BrushColor;
	int							m_iRenderDepth;
	SY_Obj						m_tMapObj[10];
	syBoxObject					m_BoxObj;
	syPlaneObject				m_Screen;
	syPlaneObject				m_brush;
	syPlaneObject				m_eraser;
	//syLineObject				test;
	D3DXVECTOR3					m_vLightVector;
	LIGHT_CONSTANT_BUFFER		m_cbLight;
	MAP_CONSTANT_BUFFER			m_cbMap;

	int							m_changedtexID = -1;


	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pConstantBufferMap;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pConstantBufferLight;
public:
	bool IsSetMapHeight = false;
	bool IsSetMapTex = false;
	bool IsSetbgTex = false;
	bool IsDrawAlpha = false;
	int		mapsize;
	float	rad = 10.0f;
	float	upSize = 10.0f;
public:
	wstring GetFullPath(wstring texname)
	{
		char full[_MAX_PATH];
		ZeroMemory(full, sizeof(full));
		
		_fullpath(full, wtm(texname).c_str(), _MAX_PATH);
		return  texname = GetMtW(full);
	};
	void FileSave();
	void Setrad(float ratio)
	{
		if (abs(ratio) < 1)
		{
			rad = 1;
			upSize = 1;
		}
	//	rad = mapsize / 2 * ratio;
		upSize = ratio;
		rad = abs(ratio);

	}
	bool DrawQuadtree(syNode* pNode);
	bool DrawQuadLine(syNode* pNode);
	bool LoadMapFromFile();
	bool LoadMap(int tileCnt, int cellCnt, int cellDis);
	bool Init();
	bool Frame() override;
	//bool RenderObject(D3DXMATRIX& matView, D3DXMATRIX& matProj);
	bool Render() override;
	bool Release() override;
	syNode* CheckPickTest();
	void SetMapHeight(syNode* pSelNode);
	void DrawAlpha(syNode* pSelNode);
	
};
