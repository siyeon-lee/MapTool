#include "stdafx.h"
#include "sample.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "syStd.h"
#include "syTextureMgr.h"


sample::sample()
{
	m_BrushColor = D3DXVECTOR4(1, 1, 1, 1);
	m_iRenderDepth = 0;
}


sample::~sample()
{
}
void sample::FileSave()
{
	m_Quadtree.QuadTreeToMap();
	//Map To File
	FILE* m_pStream = nullptr;
	_wfopen_s(&m_pStream, L"../../data/mapTool/map.txt", _T("wb"));
	_ftprintf(m_pStream, _T("%s"), _T("syMapExporter100"));	//syMapExporter100 : 파일 검증
	_ftprintf(m_pStream, _T("\n%s "), L"#TILEMAP_INFO");
	_ftprintf(m_pStream, _T("%d %d %.0f"),
		m_Map.m_iNumRows, //tileCnt,
		m_Map.m_iCellNum,// cellCnt,
		m_Map.m_fCellDistance//cellDis,
	);

	_ftprintf(m_pStream, _T("\n%s"),
		L"#MAP_TEXTURE_INFO");

	ID3D11Resource* resource;
	m_RT.m_pSRV->GetResource(&resource);
	ID3D11Texture2D* texture;
	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&texture));
	ID3D11Resource* resource2;
	m_RT.m_pRTV->GetResource(&resource2);

	//0. mapTextureFile
	_ftprintf(m_pStream, _T("\n%s"),
		GetFullPath(m_Map.m_pMapTex->m_szPath + m_Map.m_pMapTex->m_szName).c_str());

	//1.Alpha srv 
	D3DX11SaveTextureToFile(m_pImmediateContext, texture, D3DX11_IFF_DDS, L"../../data/mapTool/srv.dds");
	texture->Release();


	_ftprintf(m_pStream, _T("\n%s"),
		GetFullPath(L"../../data/mapTool/srv.dds").c_str());	//절대경로로 바꿔야함
	_ftprintf(m_pStream, _T("\n%s"),
		GetFullPath(m_Map.m_backgroundTex[0]->m_szPath + m_Map.m_backgroundTex[0]->m_szName).c_str());
	_ftprintf(m_pStream, _T("\n%s"),
		GetFullPath(m_Map.m_backgroundTex[1]->m_szPath + m_Map.m_backgroundTex[1]->m_szName).c_str());
	_ftprintf(m_pStream, _T("\n%s"),
		GetFullPath(m_Map.m_backgroundTex[2]->m_szPath + m_Map.m_backgroundTex[2]->m_szName).c_str());
	//6. node texindex list
	_ftprintf(m_pStream, _T("\n%d"), nodeTexList.size());
	for (int inodetx = 0; inodetx < nodeTexList.size(); inodetx++)
	{
		_ftprintf(m_pStream, _T("\n%d %s"),
			nodeTexList[inodetx],
			GetFullPath(I_TextureMgr.GetPtr(nodeTexList[inodetx])->m_szPath + I_TextureMgr.GetPtr(nodeTexList[inodetx])->m_szName).c_str());
	}

	_ftprintf(m_pStream, _T("\n%s %d"),
		L"#NODE_TEXINDEX_INFO",
		m_Quadtree.m_LeafNodeList.size());
	for (int inodetx = 0; inodetx < m_Quadtree.m_LeafNodeList.size(); inodetx++)
	{
		if (!m_Quadtree.m_LeafNodeList[inodetx]->m_DrawTex) continue;
		_ftprintf(m_pStream, _T("\n%d %d"),
			inodetx,
			m_Quadtree.m_LeafNodeList[inodetx]->m_DrawTex->m_iIndex);
	}
	//map의 CreateVertexData()만 바꾸면 됨
	_ftprintf(m_pStream, _T("\n%s"), 
		L"#MAP_VERTEX_INFO");

	for (int i = 0; i < m_Quadtree.m_pMap->m_iNumVertices; i++)
	{
		_ftprintf(m_pStream, _T("\n%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f"),
			m_Map.m_VertexList[i].p.x,
			m_Map.m_VertexList[i].p.y,
			m_Map.m_VertexList[i].p.z,
			m_Map.m_VertexList[i].n.x,
			m_Map.m_VertexList[i].n.y,
			m_Map.m_VertexList[i].n.z,
			m_Map.m_VertexList[i].c.x,
			m_Map.m_VertexList[i].c.y,
			m_Map.m_VertexList[i].c.z,
			m_Map.m_VertexList[i].c.w,
			m_Map.m_VertexList[i].t.x,
			m_Map.m_VertexList[i].t.y
		);
	}

	fclose(m_pStream);
}
bool sample::LoadMapFromFile()
{
	
	static TCHAR CheckFile[MAX_PATH];
	static TCHAR CheckTile[MAX_PATH];
	static TCHAR CheckTexture[MAX_PATH];
	ZeroMemory(CheckFile, _countof(CheckFile));
	ZeroMemory(CheckTile, _countof(CheckTile));
	ZeroMemory(CheckTexture, _countof(CheckTexture));
	FILE* m_pStream = nullptr;
	_wfopen_s(&m_pStream, L"../../data/mapTool/map.txt", _T("rb"));
	_ftscanf_s(m_pStream, _T("%s"), CheckFile, _countof(CheckFile)); //syMapExporter100 : 파일 검증

	int tileCnt;
	int cellCnt;
	float cellDis;
	_ftscanf_s(m_pStream, _T("%s %d %d %f"), CheckTile, _countof(CheckTile),
		&tileCnt,
		&cellCnt,
		&cellDis);

	int NumVertex;
	int VertexSize;
	_ftscanf_s(m_pStream, _T("%s"), CheckTexture, _countof(CheckTexture));

	//0. mapTextureFile
	static TCHAR MapTexture[MAX_PATH];
	ZeroMemory(MapTexture, _countof(MapTexture));
	_ftscanf_s(m_pStream, _T("%s"),
		MapTexture, _countof(MapTexture));

	//1.Alpha srv 
	static TCHAR AlphaTexture[MAX_PATH];
	ZeroMemory(AlphaTexture, _countof(AlphaTexture));
	_ftscanf_s(m_pStream, _T("%s"),
		AlphaTexture, _countof(AlphaTexture));
	


	//2.R 3.G 4.B 5.map
	static TCHAR RTexture[MAX_PATH];
	ZeroMemory(RTexture, _countof(RTexture));
	_ftscanf_s(m_pStream, _T("%s"),
		RTexture, _countof(RTexture));

	static TCHAR GTexture[MAX_PATH];
	ZeroMemory(GTexture, _countof(GTexture));
	_ftscanf_s(m_pStream, _T("%s"),
		GTexture, _countof(GTexture));

	static TCHAR BTexture[MAX_PATH];
	ZeroMemory(BTexture, _countof(BTexture));
	_ftscanf_s(m_pStream, _T("%s"),
		BTexture, _countof(BTexture));

	
	//6. node texindex list
	int nodelisSize;
	_ftscanf_s(m_pStream, _T("%d"), &nodelisSize);
	nodeTexList.resize(nodelisSize);
	static TCHAR nodetex[MAX_PATH];
	for (int inodetx = 0; inodetx < nodelisSize; inodetx++)
	{
		ZeroMemory(nodetex, _countof(nodetex));
		_ftscanf_s(m_pStream, _T("%d %s"),
			&nodeTexList[inodetx],
			nodetex, _countof(nodetex));
	}

	
	static TCHAR NODETEXINFO[MAX_PATH];
	ZeroMemory(NODETEXINFO, _countof(NODETEXINFO));
	int leafnodesize;
	_ftscanf_s(m_pStream, _T("%s %d"),
		NODETEXINFO, _countof(NODETEXINFO),
		&leafnodesize);
	for (int inodetx = 0; inodetx < leafnodesize; inodetx++)
	{
		int a;
		int b;
		_ftscanf_s(m_pStream, _T("%d %d"),
			&a,
			&b);
			/*inodetx,
			m_Quadtree.m_LeafNodeList[inodetx]->m_DrawTex->m_iIndex);*/
	}

	syMapDesc md;
	md.iNumCols = tileCnt;
	md.iNumRows = tileCnt;
	md.fCellDistance = cellDis;
	md.iCellNum = cellCnt;
	md.fHeightScale = 1.0f;
	md.szShaderFile = L"../../data/shader/splatting.hlsl";



	md.szmapTextureFile = MapTexture;

	m_RT.Release();
	m_RTBrush.Release();
	m_Map.Release();
	m_Quadtree.Release();
	nodeTexList.clear();


	//m_RTBrush.Create(m_pDevice, md.fCellDistance*md.iCellNum*md.iNumCols, md.iNumCols*md.fCellDistance*md.iCellNum);

	/*HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(,
		m_pDevice,
		AlphaTexture,
		NULL,
		NULL,
		m_RT.m_pSRV.GetAddressOf(),
		NULL);
*/
	

	
	//m_RT.m_pSRV = I_TextureMgr.GetPtr(I_TextureMgr.Load(m_pDevice, AlphaTexture))->m_pSRV;


	//m_RT.Create(m_pDevice, md.fCellDistance*md.iCellNum*md.iNumCols, md.iNumCols*md.fCellDistance*md.iCellNum);
	//m_pImmediateContext->CopyResource(m_RT.m_pTexture.Get(), texture);
	//ID3D11Texture2D* texture;


	D3DX11_IMAGE_LOAD_INFO imageload;
	ZeroMemory(&imageload, sizeof(D3DX10_IMAGE_LOAD_INFO));
	imageload.MipLevels = 1;
	imageload.Usage = D3D11_USAGE_DEFAULT;	//필수
	imageload.CpuAccessFlags = 0;	//이거 필수
	imageload.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	imageload.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//imageload.Height = md.fCellDistance*md.iCellNum*md.iNumCols;
	//imageload.Width = md.fCellDistance*md.iCellNum*md.iNumCols;
	
	ID3D11Resource* resource;

	D3DX11CreateTextureFromFile(m_pDevice, AlphaTexture, &imageload,NULL, &resource,NULL);

	
	D3D11_TEXTURE2D_DESC td;

	ID3D11Texture2D* texture;
	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&texture));
	texture->GetDesc(&td);
	m_RT.Create(m_pDevice, md.fCellDistance*md.iCellNum*md.iNumCols, md.iNumCols*md.fCellDistance*md.iCellNum);

	m_pImmediateContext->CopyResource(m_RT.m_pTexture.Get(), texture);

	if (!m_Map.Load(m_pDevice, md))
	{
		return false;
	}
	m_Map.SetBGTex(0, I_TextureMgr.Load(
		m_pDevice,
		RTexture));
	m_Map.SetBGTex(1, I_TextureMgr.Load(
		m_pDevice,
		GTexture));
	m_Map.SetBGTex(2, I_TextureMgr.Load(
		m_pDevice,
		BTexture));

	static TCHAR MAPVERTEXINFO[MAX_PATH];
	ZeroMemory(MAPVERTEXINFO, _countof(MAPVERTEXINFO));
	_ftscanf_s(m_pStream, _T("%s"),
		MAPVERTEXINFO, _countof(MAPVERTEXINFO));

	for (int i = 0; i < m_Map.m_iNumVertices; i++)
	{
		_ftscanf_s(m_pStream, _T("%f %f %f %f %f %f %f %f %f %f %f %f"),
			&m_Map.m_VertexList[i].p.x,
			&m_Map.m_VertexList[i].p.y,
			&m_Map.m_VertexList[i].p.z,
			&m_Map.m_VertexList[i].n.x,
			&m_Map.m_VertexList[i].n.y,
			&m_Map.m_VertexList[i].n.z,
			&m_Map.m_VertexList[i].c.x,
			&m_Map.m_VertexList[i].c.y,
			&m_Map.m_VertexList[i].c.z,
			&m_Map.m_VertexList[i].c.w,
			&m_Map.m_VertexList[i].t.x,
			&m_Map.m_VertexList[i].t.y
		);
	}
	m_Map.m_dxObj.m_pVertexBuffer.Attach(
		DX::CreateVertexBuffer(m_Map.m_pDevice, &m_Map.m_VertexList.at(0),
			m_Map.m_dxObj.m_iNumVertex, m_Map.m_dxObj.m_iVertexSize));
	m_Quadtree.Build(&m_Map, log2(md.iNumCols), md.fCellDistance*md.iCellNum);
	m_cbMap.pickPos = D3DXVECTOR4(0, 0, 0, rad);
	m_cbMap.mapLenth[0] = md.fCellDistance * md.iCellNum * md.iNumCols;
	fclose(m_pStream);
	return true;
}
bool sample::LoadMap(int tileCnt, int cellCnt, int cellDis)
{
	m_RT.Release();
	m_RTBrush.Release();
	m_Map.Release();
	m_Quadtree.Release();
	syMapDesc md;
	md.iNumCols = tileCnt;
	md.iNumRows = tileCnt;
	md.fCellDistance = cellDis;
	md.iCellNum = cellCnt;
	md.fHeightScale = 1.0f;
	md.szShaderFile = L"../../data/shader/splatting.hlsl";
	md.szmapTextureFile = L"../../data/bitmap/grass.png";	//0번
	
	//md.szCubeFile = L"../../data/map/HEIGHT_MOUNDS.bmp";			//1번
	m_RT.Create(m_pDevice, (md.fCellDistance*md.iCellNum)*md.iNumCols, md.iNumCols*md.fCellDistance*md.iCellNum);
	m_RTBrush.Create(m_pDevice, (md.fCellDistance*md.iCellNum)*md.iNumCols, md.iNumCols*md.fCellDistance*md.iCellNum);

	
	float fColor[] = { 0,0,0,1 };

	m_pImmediateContext->ClearRenderTargetView(m_RT.m_pRTV.Get(), fColor);

	if (!m_Map.Load(m_pDevice, md))
	{
		return false;
	}
	m_Map.SetBGTex(0, I_TextureMgr.Load(
		m_pDevice,
		L"../../data/map/000.jpg"));
	m_Map.SetBGTex(1, I_TextureMgr.Load(
		m_pDevice,
		L"../../data/map/002.jpg"));
	m_Map.SetBGTex(2, I_TextureMgr.Load(
		m_pDevice,
		L"../../data/map/004.jpg"));
	mapsize = (md.fCellDistance*md.iCellNum)*md.iNumCols;
	m_Quadtree.Build(&m_Map, log2(md.iNumCols), md.fCellDistance*md.iCellNum);
	for (int iObj = 0; iObj < 10; iObj++)
	{
	//	m_Quadtree.AddObject(&m_tMapObj[iObj]);
	}
	m_cbMap.pickPos = D3DXVECTOR4(0, 0, 0, rad);
	m_cbMap.mapLenth[0] = md.fCellDistance * md.iCellNum * md.iNumCols;

}



bool sample::Init()
{
	m_cbLight.g_cAmbientMaterial = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1);
	m_cbLight.g_cDiffuseMaterial = D3DXVECTOR4(0.9, 0.9, 0.9, 1);
	m_cbLight.g_cAmbientLightColor = D3DXVECTOR4(1, 1, 1, 1);
	m_cbLight.g_cDiffuseLightColor = D3DXVECTOR4(1, 1, 1, 1);


	m_pConstantBufferLight.Attach(DX::CreateConstantBuffer(
		m_pDevice, &m_cbLight, 1, sizeof(LIGHT_CONSTANT_BUFFER)));
	if (!m_brush.Create(m_pDevice,
		L"../../data/shader/plane.hlsl",
		L"../../data/bitmap/3.bmp"))
	{
		return false;
	}
	m_brush.SetColor(D3DXVECTOR4(1, 0, 0, 1));
	if (!m_eraser.Create(m_pDevice,
		L"../../data/shader/plane.hlsl",
		L"../../data/bitmap/3.bmp"))
	{
		return false;
	}
	m_eraser.SetColor(D3DXVECTOR4(1, 1, 1, 1));
	if (!m_BoxObj.Create(m_pDevice,
		L"../../data/shader/shader.hlsl",
		L"../../data/bitmap/source.gif"))
	{
		return false;
	}

	if (!m_Screen.Create(m_pDevice, L"../../data/shader/shader.hlsl", L"../../data/bitmap/Mario - Walk2.gif"))
	{
		return false;
	};

	if (!m_Line.Create(m_pDevice,
		L"../../data/shader/Line.hlsl",
		nullptr))
	{
		return false;
	}

	LoadMap(16,32,1);
	m_pConstantBufferMap.Attach(DX::CreateConstantBuffer(
		m_pDevice, &m_cbMap, 1, sizeof(MAP_CONSTANT_BUFFER)));
	m_CameraBack.Init();
	m_CameraBack.CreateFrustumObject(m_pDevice);
	m_CameraBack.SetViewMatrix(D3DXVECTOR3(0.0f, 10.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_CameraBack.SetProjMatrix(D3DX_PI / 4, g_rtClient.right / g_rtClient.bottom, 1.0f, 1000.0f);
	m_pMainCamera = &m_CameraBack;

	m_MiniMapRT.Create(m_pDevice, 1024, 1024);
	return true;
}
bool sample::Frame()
{

	m_vLightVector.x = 1;// sinf(g_fGameTimer);// cosf(g_fGameTimer)/ 2 + 0.5f;
	m_vLightVector.y = 1; //0;// sinf(g_fGameTimer) / 2 + 0.5f;
	m_vLightVector.z = 1;// cosf(g_fGameTimer);// cosf(g_fGameTimer) * sinf(g_fGameTimer) / 2 + 0.5f;

	D3DXVec3Normalize(&m_vLightVector, &m_vLightVector);
	m_vLightVector *= -1.0f;

	if (g_ActionInput.bRightClick == KEY_PUSH)
	{
		if (m_iRenderDepth++ > 7) m_iRenderDepth = 0;
	}
	m_Map.Frame();
	m_Quadtree.Update(m_pMainCamera);
	m_Quadtree.Frame();

	if (g_ActionInput.bENTERKEY == KEY_PUSH)
	{
		FileSave();
	}

	return true;
}
bool sample::Render()
{
	//CheckPickTest();
	DX::ApplyDSS(m_pImmediateContext, DX::syDxState::m_pDSSDepthEnable);
	DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlend);

	m_cbLight.g_vLightDir.x = m_vLightVector.x;
	m_cbLight.g_vLightDir.y = m_vLightVector.y;
	m_cbLight.g_vLightDir.z = m_vLightVector.z;
	m_cbLight.g_vLightDir.w = 1;

	m_cbLight.g_vEyeDir.x = m_pMainCamera->GetLookVector()->x;
	m_cbLight.g_vEyeDir.y = m_pMainCamera->GetLookVector()->y;
	m_cbLight.g_vEyeDir.z = m_pMainCamera->GetLookVector()->z;
	m_cbLight.g_vEyeDir.w = 100;

	m_cbLight.g_vEyePos.x = m_pMainCamera->GetEyePt()->x;
	m_cbLight.g_vEyePos.y = m_pMainCamera->GetEyePt()->y;
	m_cbLight.g_vEyePos.z = m_pMainCamera->GetEyePt()->z;
	m_cbLight.g_vEyePos.w = 1;

	D3DXMatrixIdentity(&m_cbLight.g_matInvWorld);

	m_cbMap.pickPos.x = m_Select.m_vIntersection.x;
	m_cbMap.pickPos.y = m_Select.m_vIntersection.x;
	m_cbMap.pickPos.z = m_Select.m_vIntersection.x;
	m_cbMap.pickPos.w = rad;

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferMap.Get(), 0, NULL, &m_cbMap, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pConstantBufferMap.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBufferMap.GetAddressOf());

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight.Get(), 0, NULL, &m_cbLight, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(2, 1, m_pConstantBufferLight.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(2, 1, m_pConstantBufferLight.GetAddressOf());

	m_Map.SetMatrix(&m_pMainCamera->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	//m_Map.Render(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(1, 1, m_RT.m_pSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(5, 1, m_RTBrush.m_pSRV.GetAddressOf());
	m_Quadtree.Render(m_pImmediateContext);
	m_Line.SetMatrix(nullptr, (D3DXMATRIX*)&m_pMainCamera->m_matView, (D3DXMATRIX*)&m_pMainCamera->m_matProj);
	DrawQuadLine(m_Quadtree.m_pRootNode);
	


	D3DXMATRIX m_matTopView;
	D3DXMatrixLookAtLH(&m_matTopView, &D3DXVECTOR3(0, 500, 0), &D3DXVECTOR3(0, 0, 0.2), &D3DXVECTOR3(0, 1, 0));

	m_MiniMapRT.Begin(m_pImmediateContext);
	{
		m_Map.SetMatrix(nullptr, &m_matTopView, &m_pMainCamera->m_matProj);
		m_Quadtree.Render(m_pImmediateContext);
		m_pMainCamera->SetMatrix(nullptr, (D3DXMATRIX*)&m_matTopView, (D3DXMATRIX*)&m_pMainCamera->m_matProj);
		m_pMainCamera->Render(m_pImmediateContext);
		m_MiniMapRT.End(m_pImmediateContext);
	}


	//// 최종 출력본을 그대로 백버퍼에 출력한다.
	DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlendDisable);
	DX::ApplyDSS(m_pImmediateContext, DX::syDxState::m_pDSSDepthDisable);


	// //fullscreen
	//m_Screen.m_VertexList[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	//m_Screen.m_VertexList[1].p = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	//m_Screen.m_VertexList[2].p = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	//m_Screen.m_VertexList[3].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);

	//m_pImmediateContext->UpdateSubresource(m_Screen.m_dxObj.m_pVertexBuffer.Get(), 0, NULL, &m_Screen.m_VertexList.at(0), 0, 0);

	//m_Screen.SetMatrix(NULL, NULL, NULL);
	//m_Screen.PreRender(m_pImmediateContext);
	//m_pImmediateContext->PSSetShaderResources(0, 1, m_RT.m_pSRV.GetAddressOf());
	//m_Screen.PostRender(m_pImmediateContext);


	////// minimap
	m_Screen.m_VertexList[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_Screen.m_VertexList[1].p = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);
	m_Screen.m_VertexList[2].p = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	m_Screen.m_VertexList[3].p = D3DXVECTOR3(-1.0f, 0.5f, 0.0f);

	m_pImmediateContext->UpdateSubresource(m_Screen.m_dxObj.m_pVertexBuffer.Get(), 0, NULL, &m_Screen.m_VertexList.at(0), 0, 0);
	m_Screen.SetMatrix(NULL, NULL, NULL);
	m_Screen.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_MiniMapRT.m_pSRV.GetAddressOf());
	m_Screen.PostRender(m_pImmediateContext);

	return true;

}

bool sample::DrawQuadtree(syNode* pNode)
{
	if (pNode == nullptr) return true;
	//if (m_iRenderDepth == pNode->m_iDepth)
	{
		D3DXVECTOR4 vColor = D3DXVECTOR4(1, 0, 0, 1);


		D3DXVECTOR3 vMax = pNode->m_tBox.vMax;
		D3DXVECTOR3 vMin = pNode->m_tBox.vMin;

		D3DXVECTOR3 vStart = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		D3DXVECTOR3 vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);





		//z
		vStart = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

	
		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);



		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);
	
	}
	return true;
}
bool sample::Release()
{
	/*for (auto obj : m_ObjList)
	{
		obj.Release();
	}*/
	m_BoxObj.Release();
	m_Screen.Release();
	m_Map.Release();
	m_Quadtree.Release();
	m_Line.Release();
	m_brush.Release();
	m_eraser.Release();
	return true;
}

bool sample::DrawQuadLine(syNode* pNode)
{
	if (pNode == nullptr) return true;
	if (m_iRenderDepth == pNode->m_iDepth)
	{
		D3DXVECTOR4 vColor = D3DXVECTOR4(1, 1, 1, 1);
		if (pNode->m_iDepth == 1)
			vColor = D3DXVECTOR4(1, 0, 0, 1);
		if (pNode->m_iDepth == 2)
			vColor = D3DXVECTOR4(0, 1, 0, 1);
		if (pNode->m_iDepth == 3)
			vColor = D3DXVECTOR4(0, 0, 1, 1);
		if (pNode->m_iDepth == 4)
			vColor = D3DXVECTOR4(1, 1, 0, 1);
		if (pNode->m_iDepth == 5)
			vColor = D3DXVECTOR4(1, 0, 1, 1);


		D3DXVECTOR3 vMax = pNode->m_tBox.vMax;
		D3DXVECTOR3 vMin = pNode->m_tBox.vMin;
		D3DXVECTOR3 vStart = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		D3DXVECTOR3 vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);





		//z
		vStart = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);


		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);



		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);

		vStart = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
		vEnd = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
		m_Line.Draw(m_pImmediateContext,
			vStart,
			vEnd,
			vColor);




		for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
		{
			SY_Obj* tBox = (SY_Obj*)pNode->m_ObjectList[iObj];
			m_BoxObj.SetMatrix(
				&tBox->matWorld,
				(D3DXMATRIX*)&m_pMainCamera->m_matView,
				(D3DXMATRIX*)&m_pMainCamera->m_matProj);
			m_BoxObj.Render(m_pImmediateContext);
		}
	}

	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		DrawQuadLine(pNode->m_ChildList[iNode]);
	}
	return true;
}
syNode* sample::CheckPickTest()
{
	float pickDistant = 99999;
	syNode* pSelNode = 0;


	syTextArray keydata;
	D3DXVECTOR3 intersectionindex;
	D3DXVECTOR3 intersection;
	if (true)//(g_ActionInput.bRightClick == KEY_HOLD)
	{
		m_Select.SetMatrix(nullptr,
			&m_pMainCamera->m_matView,
			&m_pMainCamera->m_matProj);
	
		for (int iNode = 0; iNode <
			m_Quadtree.m_DrawNodeList.size(); iNode++)
		{
			syNode* pNode = m_Quadtree.m_DrawNodeList[iNode];

			/*if (!m_Select.ChkAABBToRay(&pNode->m_tBox))*/
			if (!m_Select.ChkOBBToRay(&pNode->m_tBox))
			{
				continue;
			}
			for (
				int iTri = 0;
				iTri < m_Quadtree.m_Sharedindex.size();
				iTri += 3) 
			{

				DWORD i0, i1, i2;
				i0 = m_Quadtree.m_Sharedindex[iTri + 0];
				i1 = m_Quadtree.m_Sharedindex[iTri + 1];
				i2 = m_Quadtree.m_Sharedindex[iTri + 2];
				D3DXVECTOR3 v0, v1, v2;
				v0 = pNode->m_VertexData[i0].p;
				v1 = pNode->m_VertexData[i1].p;
				v2 = pNode->m_VertexData[i2].p;

				if (m_Select.CheckPick(v0, v1, v2))
				{
					//가까운 곳 선택
					
					if (m_Select.m_Ray.t > pickDistant)
					{
						continue;
					}
						
					intersection = m_Select.m_vIntersection;
					pickDistant = m_Select.m_Ray.t;
					pSelNode = pNode;
					intersectionindex = D3DXVECTOR3(i0, i1, i2);

				}
				
			}
		}
		m_Select.m_vIntersection = intersection;
		m_brush.m_matWorld._41 = m_Select.m_vIntersection.x;
		m_brush.m_matWorld._42 = m_Select.m_vIntersection.y;
		m_brush.m_matWorld._43 = m_Select.m_vIntersection.z;

		m_RTBrush.Begin(m_pImmediateContext);
		{
			
			
			
				DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlend);
			
			m_pImmediateContext->PSSetShaderResources(0, 1, m_RTBrush.m_pSRV.GetAddressOf());
			m_brush.SetMatrix(&m_brush.m_matWorld, nullptr, nullptr);
			m_brush.Render(m_pImmediateContext);

		}
		m_RTBrush.EndAlphaTexture(m_pImmediateContext);
		return pSelNode;
	}

	return pSelNode;
}
void sample::DrawAlpha(syNode* pSelNode)
{
	if (!pSelNode) return;
	m_brush.m_matWorld._41 = m_Select.m_vIntersection.x;
	m_brush.m_matWorld._42 = m_Select.m_vIntersection.y;
	m_brush.m_matWorld._43 = m_Select.m_vIntersection.z;
	m_cbMap.pickPos.x = m_Select.m_vIntersection.x;
	m_cbMap.pickPos.y = m_Select.m_vIntersection.y;
	m_cbMap.pickPos.z = m_Select.m_vIntersection.z;
	m_cbMap.pickPos.w = rad;
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferMap.Get(), 0, NULL, &m_cbMap, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pConstantBufferMap.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBufferMap.GetAddressOf());

	m_RT.DrawAlphaTexture(m_pImmediateContext);
	{

		/*if (g_ActionInput.bSpace == KEY_HOLD)
		{

			DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlendminus);
		}
		else
		{
			DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlend2);
		}
*/
		m_pImmediateContext->PSSetShaderResources(0, 1, m_RT.m_pSRV.GetAddressOf());
		m_brush.SetMatrix(&m_brush.m_matWorld, nullptr, nullptr);
		m_eraser.SetMatrix(&m_brush.m_matWorld, nullptr, nullptr);
		DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlendminus);
		m_eraser.Render(m_pImmediateContext);
		DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlend2);
		m_brush.Render(m_pImmediateContext);

	}
	m_RT.EndAlphaTexture(m_pImmediateContext);

	DX::ApplyBS(m_pImmediateContext, DX::syDxState::m_pAlphaBlendDisable);
	DX::ApplyDSS(m_pImmediateContext, DX::syDxState::m_pDSSDepthDisable);
}
void sample::SetMapHeight(syNode* pSelNode)
{
	if (!pSelNode) return;
	
	
	SY_SPHERE rader;
	rader.vCenter = m_Select.m_vIntersection;
	rader.fRadius = rad;
	queue<syNode*> m_TargetNode;

	syNode* pNode = m_Quadtree.m_pRootNode;
	do {
		for (int iNode = 0;
			iNode < pNode->m_ChildList.size(); iNode++)
		{

			if (m_Quadtree.CheckNodeInSphere(pNode->m_ChildList[iNode], &rader))
			{

				m_TargetNode.push(pNode->m_ChildList[iNode]);
			}
		}

		if (m_TargetNode.empty()) break;
	
		pNode = m_TargetNode.front();
		if (pNode->m_isLeaf) break;
		m_TargetNode.pop();

	} while (pNode);

	while (!m_TargetNode.empty()) {
		pNode = m_TargetNode.front();
		m_TargetNode.pop();
		vector<int> NormalRefNum;
		vector<float> heightList;
		vector<D3DXVECTOR3> NormalList;
		NormalRefNum.resize(pNode->m_VertexSize);
		heightList.resize(pNode->m_VertexSize);
		NormalList.resize(pNode->m_VertexSize, D3DXVECTOR3(0,0,0));
		for (
			int iTriNum = 0;
			iTriNum < m_Quadtree.m_Sharedindex.size();
			iTriNum += 3)
		{			
			int chk = 0;
			DWORD i0, i1, i2;
			i0 = m_Quadtree.m_Sharedindex[iTriNum + 0];
			i1 = m_Quadtree.m_Sharedindex[iTriNum + 1];
			i2 = m_Quadtree.m_Sharedindex[iTriNum + 2];
			D3DXVECTOR3 delvec = (m_Select.m_vIntersection - pNode->m_VertexData[i0].p);
			delvec.y = 0;
			
			float delLen = D3DXVec3Length(&delvec);
			if (delLen <= rad)
			{
				//pNode->m_VertexData[i0].c = D3DXVECTOR4(1, 0, 0, 1);
				heightList[i0] += upSize * (1 - sinf((delLen / rad) * (D3DX_PI / 2)));
				//pNode->m_VertexData[i0].p.y += upSize;
				NormalRefNum[i0]+=1;// *(1 - sinf(delLen / rad * D3DX_PI / 2));
				//pNode->m_VertexData[i0].c = D3DXVECTOR4(rand(), rand(), rand(),1);
				chk++;

			}

			delvec = (m_Select.m_vIntersection - pNode->m_VertexData[i1].p);
			delvec.y = 0;
			delLen = D3DXVec3Length(&delvec);
			if (delLen <= rad)
			{
				//pNode->m_VertexData[i1].c = D3DXVECTOR4(1, 0, 0, 1);
				heightList[i1] += upSize * (1 - sinf((delLen / rad) * (D3DX_PI / 2)));
				//pNode->m_VertexData[i1].p.y += upSize;// *  (1 - sinf(delLen / rad * D3DX_PI / 2));
				NormalRefNum[i1]+=1;
				//pNode->m_VertexData[i1].c = D3DXVECTOR4(rand(), rand(), rand(),1);
				chk++;
			}

			delvec = (m_Select.m_vIntersection - pNode->m_VertexData[i2].p);
			delvec.y = 0;
			delLen = D3DXVec3Length(&delvec);
			if (delLen <= rad)
			{
				//pNode->m_VertexData[i2].c = D3DXVECTOR4(1, 0, 0, 1);
				heightList[i2] += upSize * (1 - sinf((delLen / rad) * (D3DX_PI / 2)));
				//pNode->m_VertexData[i2].p.y += upSize;// * (1 - sinf(delLen / rad * D3DX_PI / 2));
				NormalRefNum[i2]+=1;
				//pNode->m_VertexData[i2].c = D3DXVECTOR4(rand(), rand(), rand(),1);
				chk++;
			}
			if (chk == 0) continue;
			D3DXVECTOR3 vNormal;
			D3DXVECTOR3 v0, v1, v2;
			v0 = pNode->m_VertexData[i0].p;
			if(NormalRefNum[i0])
				v0.y += heightList[i0] / NormalRefNum[i0];
			v1 = pNode->m_VertexData[i1].p;
			if (NormalRefNum[i1])
				v1.y += heightList[i1] / NormalRefNum[i1];
			v2 = pNode->m_VertexData[i2].p;
			if (NormalRefNum[i2])
				v2.y += heightList[i2] / NormalRefNum[i2];

			D3DXVECTOR3 vEdge0 = v1 - v0;
			D3DXVECTOR3 vEdge1 = v2 - v0;

			D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
			D3DXVec3Normalize(&vNormal, &vNormal);
			NormalList[i0] += vNormal;
			NormalList[i1] += vNormal;
			NormalList[i2] += vNormal;

		}
		for (int iVerchk = 0; iVerchk < pNode->m_VertexSize; iVerchk++)
		{
			if (NormalRefNum[iVerchk] == 0) continue;
			pNode->m_VertexData[iVerchk].p.y += heightList[iVerchk] / NormalRefNum[iVerchk];
			NormalList[iVerchk] /= NormalRefNum[iVerchk];
			D3DXVec3Normalize(&pNode->m_VertexData[iVerchk].n, &NormalList[iVerchk]);
			
		}

		m_Quadtree.UpdateBoundingBox(pNode);
		pNode->m_VertexBuffer.Attach(
			DX::CreateVertexBuffer(g_pDevice,
				&pNode->m_VertexData.at(0), pNode->m_VertexSize, sizeof(PNCT_VERTEX)));
		

	} 
}