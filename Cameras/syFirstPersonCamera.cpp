#include "syFirstPersonCamera.h"


syFirstPersonCamera::syFirstPersonCamera()
{
	m_fRadius = 10.0f;
}


syFirstPersonCamera::~syFirstPersonCamera()
{
}
bool  syFirstPersonCamera::Frame()
{
	if (m_iMouseWhellDelta)
	{
		m_fRadius += (m_iMouseWhellDelta / 120.0f) * g_fSecondPerFrame * 50.0f;
		m_iMouseWhellDelta = 0;
	}
	m_vValue.x = m_ViewArcBall.m_vEularAngle.x;
	m_vValue.y = m_ViewArcBall.m_vEularAngle.y;
	m_vValue.z = m_ViewArcBall.m_vEularAngle.z;
	m_vValue.w = m_fRadius;

	D3DXMATRIX matRotation;
	D3DXQUATERNION m_qRotation;
	D3DXQuaternionRotationYawPitchRoll(
		&m_qRotation,
		m_vValue.y, m_vValue.x, m_vValue.z);
	// 力 1 规过
	/*D3DXMatrixAffineTransformation(
	&matRotation, 1.0f, NULL,
		&m_qRotation, &m_vPosition);*/
		// 力 2 规过	
	D3DXMatrixRotationQuaternion(
		&matRotation, &m_qRotation);
	matRotation._41 = m_vPosition.x;
	matRotation._42 = m_vPosition.y;
	matRotation._43 = m_vPosition.z;
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);

	return UpdateVector();
}

