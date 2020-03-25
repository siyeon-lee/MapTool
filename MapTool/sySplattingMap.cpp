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
	//imageload.Usage = D3D11_USAGE_STAGING;	//�ʼ�
	//imageload.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;	//�̰� �ʼ�
	//imageload.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//

	//ID3D11Resource* pMapHeight = NULL;
	//D3DX11CreateTextureFromFile(pd3dDevice, pMapFile, &imageload, NULL, &pMapHeight, NULL);

	//ID3D11Texture2D* pTex2D = (ID3D11Texture2D*)pMapHeight;	//������ �����������̽� ���ؼ� ��ȯ�ؾ���
	//D3D11_TEXTURE2D_DESC td;
	//pTex2D->GetDesc(&td);

	//m_fHeightList.resize(td.Width*td.Height);
	////���� ���� �ȼ��� �������� ��ü -> �ؽ�ó�� ũ��� ���� ������ ����� ��
	//D3D11_MAPPED_SUBRESOURCE mapsub;
	//if (SUCCEEDED(pContext->Map(pTex2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &mapsub)))
	//{									//D3D11CalcSubresource : ���ϴ� �Ӹ� ���� ����
	//	UCHAR* pTexels = (UCHAR*)mapsub.pData;	//�޸� ���� �ּҰ�
	//	for (UINT row = 0; row < td.Height; row++)
	//	{
	//		UINT rowStart = row * mapsub.RowPitch;		//RowPitch : �ؽ��� ���� ũ��� ������ ���� ũ�� �ٸ� �� �־�(����)
	//		for (UINT col = 0; col < td.Width; col++)
	//		{
	//			UINT uRed = pTexels[rowStart + col * 4];	//4����Ʈ�� �̵��Ͽ� 1�� �ȼ��� RGBA����Ʈ�� ��´�
	//			m_fHeightList[row*td.Width + col] = uRed;
	//		}
	//	}
	//	pContext->Unmap(pTex2D, D3D11CalcSubresource(0, 0, 1));
	//}


	return hr;
}