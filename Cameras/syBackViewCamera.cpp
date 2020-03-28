#include "syBackViewCamera.h"



syBackViewCamera::syBackViewCamera()
{
	m_fRadius = 10.0f;
}


syBackViewCamera::~syBackViewCamera()
{
}

bool syBackViewCamera::Frame()
{
	if (m_iMouseWhellDelta)
	{
		m_fRadius += (m_iMouseWhellDelta / 120.0f) * g_fSecondPerFrame * 50.0f;
		m_iMouseWhellDelta = 0;
	}

	D3DXMATRIX matRotation =
		m_ViewArcBall.GetRotationMatrix();
	D3DXVECTOR3 vWorldUp, vWorldLook;
	D3DXVECTOR3 vLocalUp =
		D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalLook =
		D3DXVECTOR3(0, 0, 1);
	// w= 1
	D3DXVec3TransformCoord(
		&vWorldUp, &vLocalUp,
		&matRotation);
	D3DXVec3TransformCoord(
		&vWorldLook, &vLocalLook,
		&matRotation);

	m_vPosition = m_vTarget - vWorldLook * m_fRadius;
	D3DXMatrixLookAtLH(
		&m_matView,
		&m_vPosition,
		&m_vTarget,
		&vWorldUp);
	return UpdateVector();
}