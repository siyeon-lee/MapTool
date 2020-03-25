#pragma once
#include "syStd.h"
#include "syMatrix.h"
#include "syArcBall.h"
#include "syFrustum.h"
class syDebugCamera : public syFrustum
{
public:
	syDebugCamera();
	virtual ~syDebugCamera();

public:
	//syFrustum		m_Frustum;
	syArcBall      m_WorldArcBall;
	syArcBall      m_ViewArcBall;

public:
	D3DXVECTOR3		m_ModelCenter;
	D3DXMATRIX		m_mModelLastRot;    // Last arcball rotation matrix for model 
	D3DXMATRIX		m_mModelRot;            // Rotation matrix of model
	D3DXMATRIX		m_mCameraRotLast;

	//�̵��ӵ�
	float			m_fSpeed;
	//��ȸ���� ���� ���ʹϾ�
	D3DXQUATERNION	m_qRotation;
	// �� �࿡ ���� ������ w=������( �������κ����� �Ÿ� )
	D3DXVECTOR3		m_vPosDelta;

	//����� �� �������
public:
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	//ī�޶� ���⺤�� ��ġ / Ÿ�ٺ���
	D3DXVECTOR3		m_vLook;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3		m_vRight;
	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3		m_vTarget;
	D3DXVECTOR3		m_vDefaultEye;          // Default camera eye position
	D3DXVECTOR3		m_vDefaultLookAt;       // Default LookAt position

	int			  m_iMouseWhellDelta;
//	D3DXVECTOR4   m_vValue;	//yaw pitch roll��

	// ���� ����� �Ӽ�����
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
	float				m_fAspect;              // Aspect ratio
	float				m_fNearPlane;           // Near plane
	float				m_fFarPlane;            // Far plane
	// �߰� : ���Ϸ� �� ���� 	
	float				m_fCameraYawAngle;
	float				m_fCameraPitchAngle;
	float				m_fCameraRollAngle;
	// �߰� : ������( �ʱ� �������κ����� �Ÿ�) �� ���� ���� 	
	float				m_fRadius;
	float				m_fDefaultRadius;       // Distance from the camera to model 
	float				m_fMinRadius;           // Min radius
	float				m_fMaxRadius;           // Max radius
	//float		  m_fFarPlane = 3000.0f;
	//float		  m_fNearPlane = 1.0f;
	//bool			isCameraMove = true;
	//ī�޶��̵�
	void Forward();
	void Backward();
public:
	//--------------------------------------------------------------------------------------
	// Ÿ�� ��ġ�� ���� �Ÿ�(������)�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	void						SetRadius(float fDefaultRadius = 5.0f, float fMinRadius = 1.0f, float fMaxRadius = FLT_MAX);
public:
	//--------------------------------------------------------------------------------------
	// �߰� : �� ���⺤�� ���� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			UpdateVector();
	//--------------------------------------------------------------------------------------
	//ī�޶� �̵�
	virtual void				MoveLook(float fValue);
	virtual void				MoveSide(float fValue);
	virtual void				MoveUp(float fValue);
	//--------------------------------------------------------------------------------------
	// �� ��� ���  
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			SetViewMatrix(D3DXVECTOR3 vPos,
		D3DXVECTOR3 vTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//--------------------------------------------------------------------------------------
	// ���� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			SetProjMatrix(FLOAT fFOV,
		FLOAT fAspect,
		FLOAT fNearPlane,
		FLOAT fFarPlane);

	//--------------------------------------------------------------------------------------
	// ���Ϸ� ���� ���� �� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual D3DXMATRIX			Update(D3DXVECTOR4 vDirValue);
	virtual bool				Frame();
	virtual bool				Render(ID3D11DeviceContext*	pContext);
	//--------------------------------------------------------------------------------------
	// ������ ����� ������ �ݿ��Ѵ�. 
	//--------------------------------------------------------------------------------------
	virtual bool				Init();
	//--------------------------------------------------------------------------------------
	// ������ �޼����� ����Ͽ� ī�޶� ���� 
	//--------------------------------------------------------------------------------------
	virtual LRESULT					MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//--------------------------------------------------------------------------------------
	// �� ȸ�� �߽���ġ�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	void						SetModelCenter(D3DXVECTOR3 vModelCenter);

	void						SetTargetPos(D3DXVECTOR3 vPos) { m_vTarget = vPos; };

public:
	//--------------------------------------------------------------------------------------
	// ����Լ�
	//--------------------------------------------------------------------------------------
	void				SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	
	D3DXMATRIX*			GetWorldMatrix() { return &m_matWorld; };
	D3DXMATRIX*			GetViewMatrix() { return &m_matView; }
	D3DXMATRIX*			GetProjMatrix() { return &m_matProj; }

	D3DXVECTOR3*		GetEyePt() { return &m_vPosition; }
	D3DXVECTOR3*		GetLookAtPt() { return &m_vTarget; }
	float				GetNearClip() { return m_fNearPlane; }
	float				GetFarClip() { return m_fFarPlane; }

	float				GetYawAngle() { return m_fCameraYawAngle; }
	float				GetPitchAngle() { return m_fCameraPitchAngle; }
	float				GetRollAngle() { return m_fCameraRollAngle; }

	D3DXVECTOR3*		GetRightVector() { return &m_vRight; }
	D3DXVECTOR3*		GetLookVector() { return &m_vLook; }
	D3DXVECTOR3*		GetUpVector() { return &m_vUp; }

	float				GetRadius() { return m_fRadius; }
};


class syGameCamera : public syDebugCamera
{
public:
	syGameCamera();
	virtual ~syGameCamera();
public:
	float m_fOffsetPos;
	D3DXMATRIX    Update(D3DXVECTOR4 vPos);
	bool    Frame();
};