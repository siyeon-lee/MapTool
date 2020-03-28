# DirectX를 이용한 맵 툴 제작
![스크린샷1](./img/KakaoTalk_20200325_163542561.png)

## 1. 소개
- 제목
  + Map Tool
- 제작 기간 
  + 2019. ~ 
  + 1인 프로젝트
  + VisualC++, MFC, DirectX11 이용
- 맵을 제작 및 저장할 수 있는 응용프로그램
- 키
  + 이동 : 방향키
  + 프레임 확인 : 스페이스바
  
## 2. 구현내용
### 2.1 DXCore
 - 엔진 라이브러리
#### 2.1.1 ArcBall
 - 마우스를 이용한 모델 회전
#### 2.1.2 카메라
#### 2.1.3 State
  - Rasterizer State
  - Alpha Blend State
  - SamplerState
  - Depth Stencil State
#### 2.1.4 충돌
  - OBB, AABB
#### 2.1.4 기타
  - 입출력, 스카이박스, 벡터, 텍스쳐, 사운드
  
### 2.3 쉐이더
 
### 2.4 스플레팅 맵

### 2.5 쿼드트리

#### 2.5.1 카메라 프러스텀과 충돌영역 OBB의 포함 여부
- DrawNode(화면 상에 그려지고 있는 노드 목록) 리스트의 충돌박스와 피킹 지점을 충돌 테스트 
```C++
bool sySelect::ChkOBBToRay(SY_BOX* pBox)
{
	SY_Ray* pRay = &m_Ray;
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	D3DXVECTOR3 vR = pRay->vOrigin - pBox->vCenter;
	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection);
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	D3DXVECTOR3 vDxR;
	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		return false;
	}
	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		return false;
	}
	// D x pBox->vAxis[2]  분리축
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
	if (fCross[2] > fRhs)
	{
		return false;
	}
	return true;

}

```
- 박스의 중심에서 반지름 역할을 하는 좌표축 Extent[3] 존재함. 해당 OBB 좌표축 투영 거리의 합과 카메라 프러스텀 평면과 노드 OBB중심의 거리를 비교하면 카메라 프러스텀과 해당 OBB포함 여부를 알 수 있다. 
#### 2.5.2 마우스 피킹
```C++
bool  sySelect::IntersectTriangle(D3DXVECTOR3& vOrig, D3DXVECTOR3& vDir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	float* t, float* u, float* v)
{
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;
	D3DXVECTOR3 pvec, tvec, qvec;
	D3DXVec3Cross(&pvec, &vDir, &edge2);
	float det = D3DXVec3Dot(&edge1, &pvec);
	if (det > 0)
	{
		tvec = vOrig - v0;
	}
	else
	{
		tvec = v0 - vOrig;
		det = -det;
	}
	if (det < 0.00001f) return false;
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u <0.0f || *u > det)
	{
		return false;
	}

	D3DXVec3Cross(&qvec, &tvec, &edge1);
	*v = D3DXVec3Dot(&vDir, &qvec);
	if (*v <0.0f || *u + *v > det)
	{
		return false;
	}

	*t = D3DXVec3Dot(&edge2, &qvec);
	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;
	return true;
}
```

```C++
bool sySelect::CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2)
{
	float t, u, v;
	if (IntersectTriangle(m_Ray.vOrigin,
		m_Ray.vDirection,
		v0, v1, v2, &t, &u, &v))
	{
		m_vIntersection = m_Ray.vOrigin + m_Ray.vDirection*t;
		m_Ray.t = t;
		return true;
	}
	return false;
}

```
- 삼각형의 한 변과 카메라부터 삼각형의한 점까지의 거리를 외적하고, 마찬가지로 다른 점까지의 거리도 외적하여 평면의 노말 벡터를 만든다. 그것과 카메라의 위치벡터, 방향벡터를 이용해 계산하면 카메라로부터 피킹 지점까지의 거리t를 구할 수 있다. T의 최소값을 가진 피킹 지점이 가장 가까운 마우스 피킹 좌표다.
