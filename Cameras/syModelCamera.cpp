#include "syModelCamera.h"



syModelCamera::syModelCamera()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_mModelRot);
	D3DXMatrixIdentity(&m_mModelLastRot);
}


syModelCamera::~syModelCamera()
{
}
bool  syModelCamera::Frame()
{
	D3DXVECTOR3 len = m_vPosition - m_vTarget;
	float distance = sqrt(len.x * len.x + len.y * len.y + len.z *len.z);
	float Radius = distance;
	static int wheeldel = 0;
	if (m_iMouseWhellDelta)
	{
		//거리에 비례하여 크기 변경
		wheeldel +=(m_iMouseWhellDelta/120);
		m_iMouseWhellDelta = 0;
		
	}
	m_fRadius = distance + wheeldel * distance * 0.1f;

	D3DXMATRIX matRotation;
	D3DXMatrixInverse(&matRotation, NULL, &m_ViewArcBall.GetRotationMatrix());

	D3DXVECTOR3 vWorldUp, vWorldLook;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalLook = D3DXVECTOR3(0, 0, 1);
	// w= 1
	//camera
	D3DXVec3TransformCoord(	&vWorldUp, &vLocalUp, &matRotation);
	D3DXVec3TransformCoord(	&vWorldLook, &vLocalLook,&matRotation);
	m_vPosition = m_vTarget - vWorldLook * m_fRadius;
	D3DXMatrixLookAtLH(
		&m_matView,
		&m_vPosition,
		&m_vTarget,
		&vWorldUp);
	//world
	D3DXMATRIX mInvView;
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	mInvView._41 = mInvView._42 = mInvView._43 = 0;

	D3DXMATRIX mModelLastRotInv;
	D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	D3DXMATRIX mModelRot;
	mModelRot = m_WorldArcBall.GetRotationMatrix();
	m_mModelRot = m_mModelRot * m_matView * mModelLastRotInv * mModelRot * mInvView;
	m_mModelLastRot = mModelRot;
	m_mModelRot._41 = m_vTarget.x;
	m_mModelRot._42 = m_vTarget.y;
	m_mModelRot._43 = m_vTarget.z;

	m_matWorld = m_mModelRot;
	return UpdateVector();
}

