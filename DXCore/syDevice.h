#pragma once
#include "syWindow.h"

class syDevice : public syWindow
{
public:
	syDevice();
	virtual ~syDevice();
public:

	IDXGIFactory*			m_pGIFactory;
	ID3D11Device*			m_pDevice;						//����̽� ��ü
	ID3D11DeviceContext*	m_pImmediateContext;			//����̽� ���ؽ�Ʈ ��ü
	IDXGISwapChain*			m_pSwapChain;					//����ü�� ��ü
	ID3D11RenderTargetView*	m_pRTView;						//���� ����Ÿ�� ��
	ID3D11DepthStencilView* m_pDepthStencilView;			//z���� )����_
	D3D_FEATURE_LEVEL		m_FeatureLevel;					//DirectD Ư�� ���� �Ӽ���
	D3D_DRIVER_TYPE			m_DirverType;					//����̽� Ÿ�� �Ӽ���
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;				//����ü�� �Ӽ���
	D3D11_VIEWPORT			m_ViewPort;						//����Ʈ �Ӽ���

public:
	bool  InitDevice();
		HRESULT CreateDevice();
		HRESULT CreateGIFactory();
		HRESULT CreateSwapChain();
		HRESULT SetRenderTargetView();
		HRESULT SetDepthStencilView();
		HRESULT SetViewPort();

	bool ReleaseDevice();
	bool ResizeDevice(UINT width, UINT height) override;
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource();
};

