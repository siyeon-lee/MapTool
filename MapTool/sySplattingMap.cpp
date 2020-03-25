#include "stdafx.h"
#include "sySplattingMap.h"


sySplattingMap::sySplattingMap()
{
}


sySplattingMap::~sySplattingMap()
{
}

HRESULT sySplattingMap::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};
	int iNumElement = sizeof(layout) / sizeof(layout[0]);
	m_dxObj.m_pInputLayout.Attach(
		DX::CreateInputLayout(m_pDevice,
			m_dxObj.m_pVSBlob->GetBufferSize(),
			m_dxObj.m_pVSBlob->GetBufferPointer(),
			layout, iNumElement));
	return hr;
}

HRESULT sySplattingMap::LoadTextures(ID3D11Device* pd3dDevice, const TCHAR* pLoadTextureFile)
{
	HRESULT hr = S_OK;

	m_alphaRT.Create(m_pDevice, (m_iNumCellRows - 1 * m_fCellDistance), (m_iNumCellRows - 1 * m_fCellDistance));



	//D3DX11_IMAGE_LOAD_INFO imageload;
	//ZeroMemory(&imageload, sizeof(D3DX10_IMAGE_LOAD_INFO));
	//imageload.MipLevels = 1;
	//imageload.Usage = D3D11_USAGE_STAGING;	//필수
	//imageload.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;	//이거 필수
	//imageload.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//

	//ID3D11Resource* pMapHeight = NULL;
	//D3DX11CreateTextureFromFile(pd3dDevice, pMapFile, &imageload, NULL, &pMapHeight, NULL);

	//ID3D11Texture2D* pTex2D = (ID3D11Texture2D*)pMapHeight;	//원래는 쿼리인터페이스 통해서 변환해야함
	//D3D11_TEXTURE2D_DESC td;
	//pTex2D->GetDesc(&td);

	//m_fHeightList.resize(td.Width*td.Height);
	////가로 세로 픽셀을 정점으로 대체 -> 텍스처의 크기와 같은 정점이 만들어 짐
	//D3D11_MAPPED_SUBRESOURCE mapsub;
	//if (SUCCEEDED(pContext->Map(pTex2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &mapsub)))
	//{									//D3D11CalcSubresource : 원하는 밉맵 레벨 조정
	//	UCHAR* pTexels = (UCHAR*)mapsub.pData;	//메모리 시작 주소값
	//	for (UINT row = 0; row < td.Height; row++)
	//	{
	//		UINT rowStart = row * mapsub.RowPitch;		//RowPitch : 텍스쳐 가로 크기와 지정된 가로 크기 다를 수 있어(압축)
	//		for (UINT col = 0; col < td.Width; col++)
	//		{
	//			UINT uRed = pTexels[rowStart + col * 4];	//4바이트씩 이동하여 1개 픽셀의 RGBA바이트를 얻는다
	//			m_fHeightList[row*td.Width + col] = uRed;
	//		}
	//	}
	//	pContext->Unmap(pTex2D, D3D11CalcSubresource(0, 0, 1));
	//}


	return hr;
}