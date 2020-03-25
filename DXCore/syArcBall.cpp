#include "syArcBall.h"


bool syArcBall::Init()
{
	SetWindow(g_rtClient.right, g_rtClient.bottom);
	return true;
}
void syArcBall::SetWindow(INT nWidth, INT nHeight, FLOAT fRadius)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_fRadius = fRadius;
	m_vCenter = D3DXVECTOR2(m_nWidth / 2.0f, m_nHeight / 2.0f);
}

// ���� ������ ���������� �������� ���� �ݱ��� �������� ȭ���� ���̿� ���̷� ��ġ���� 
// ȭ���� ���̿� ���̿� ���� �� ���а��� ���� ���� �������Ŀ� ���� ���а��� ���Ѵ�.
D3DXVECTOR3 syArcBall::ScreenToVector(float fScreenPtX, float fScreenPtY)
{
	// <<�� ������ : ȭ����� = �� ���а� : ȭ�鼺�а�>> ��ʽ� �̿�
	FLOAT x = -(fScreenPtX - m_nWidth / 2) / (m_fRadius*m_nWidth / 2);
	FLOAT y = -(fScreenPtY - m_nHeight / 2) / (m_fRadius*m_nHeight / 2);
	FLOAT z = 0.0f;

	FLOAT mag = x * x + y * y;

	//ȭ�� ���̸� �� ������ 1�� �ξ��� ������ 1���� ũ�� ȭ�� ��. ���� ����� ������ ��ü�Ѵ�.

	if (mag > 1.0f)
	{
		FLOAT scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else  //ȭ�� ��
	{
		z = sqrtf(1.0f - mag);	//���� ������ 1
	}

	return D3DXVECTOR3(x, y, z);
}
D3DXQUATERNION syArcBall::QuatFromBallPoints(const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTo)
{
	// ������ ��� ������� ����� (ȸ���� : ȸ����/2 )�̱� ������
	// �Ʒ��� ���� ������� ���� �ι��� ������ ȸ���ȴ�. 
	// �̴� ȭ���� ���� �� ���� �Ѱ踦 1 ~ -1�� �ξ��� ������ �ڻ��� 1( 90��)���� 2���� 180�� ȸ���ȴ�.

	D3DXVECTOR3 vPart;

	float fDot = D3DXVec3Dot(&vFrom, &vTo);	//ȸ����
	D3DXVec3Cross(&vPart, &vFrom, &vTo);	//ȸ����

	return D3DXQUATERNION(vPart.x, vPart.y, vPart.z, fDot);
}
syArcBall::syArcBall()
{
	m_fSpeed = 0.2f;
	m_bDrag = false;
	m_vDragAngle.x = 0;
	m_vDragAngle.y = 0;
	m_vEularAngle.x = 0;
	m_vEularAngle.y = 0;
	m_vEularAngle.z = 0;
	D3DXQuaternionIdentity(&m_qDown);
	D3DXQuaternionIdentity(&m_qNow);
}


syArcBall::~syArcBall()
{
}
D3DXMATRIX syArcBall::GetRotationMatrix()
{
	//D3DXMatrixRotationYawPitchRoll(
	//	&m_matRotation,
	//	m_vEularAngle.y,
	//	m_vEularAngle.x,
	//	0.0f);
	//return m_matRotation;
	return *D3DXMatrixRotationQuaternion(&m_mRotation, &m_qNow);
}
void	syArcBall::OnBegin(int nX, int nY)
{
	m_bDrag = true;
	m_vDragPt.x = nX;
	m_vDragPt.y = nY;
	m_qDown = m_qNow;
	m_vDownPt = ScreenToVector((float)nX, (float)nY);
}
void	syArcBall::OnMove(int nX, int nY)
{
	if (m_bDrag)
	{
		m_vDragAngle.x = (nX - m_vDragPt.x);
		m_vDragAngle.y = (nY - m_vDragPt.y);
		m_vDragPt.x = nX;
		m_vDragPt.y = nY;
		m_vEularAngle.x += D3DXToRadian(m_vDragAngle.y /*/ 400.0f*/) * m_fSpeed;
		m_vEularAngle.y += D3DXToRadian(m_vDragAngle.x /*/ 300.0f*/) * m_fSpeed;

		// �߰�
		m_vCurrentPt = ScreenToVector((float)nX, (float)nY);
		// ���� ȸ���� ������� ���� ������ ������� ���Ͽ� ������Ų��.
		m_qNow = m_qDown * QuatFromBallPoints(m_vDownPt, m_vCurrentPt);
	}
}
void	syArcBall::OnEnd(int nX, int nY)
{
	m_bDrag = false;
}