// MapForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapForm.h"
#include "afxdialogex.h"
#include "syTextureMgr.h"

// MapForm 대화 상자

IMPLEMENT_DYNAMIC(MapForm, CFormView)


MapForm::MapForm()
	: CFormView(IDD_MapForm)
	, iRowLenth(4)
	
	, iCellCount(5)
	, icellSize(1)
	, b_SetHeight(FALSE)
	, b_SetTex(FALSE)
	, b_SetAlphaTex(FALSE)

	, iRadSlot(0)
{

}

MapForm::~MapForm()
{
}

MapForm* MapForm::CreateOne(CWnd* pParent)
{
	MapForm* pForm = new MapForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

void MapForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, iRowLen, iRowLenth);

	DDX_Text(pDX, iCellCnt, iCellCount);
	DDX_Text(pDX, iCellSize, icellSize);
	DDX_Check(pDX, chk_height, b_SetHeight);
	DDX_Check(pDX, chk_texture, b_SetTex);
	DDX_Check(pDX, chk_alphaDraw, b_SetAlphaTex);
	DDX_Control(pDX, IDC_SLIDER1, brushrad);
	DDX_Radio(pDX, IDC_RADIOR, iRadSlot);
	DDX_Control(pDX, IDC_LIST1, m_tex_list);
}


BEGIN_MESSAGE_MAP(MapForm, CFormView)
	ON_BN_CLICKED(IDOK, &MapForm::OnBnClickedOk)
	ON_EN_CHANGE(iRowLen, &MapForm::OnEnChangeirowlen)
	ON_BN_CLICKED(chk_height, &MapForm::OnBnClickedheight)
	ON_BN_CLICKED(chk_texture, &MapForm::OnBnClickedtexture)
	ON_BN_CLICKED(BUTTON_LoadAlphaTex, &MapForm::OnBnClickedLoadalphatex)
	ON_BN_CLICKED(chk_alphaDraw, &MapForm::OnBnClickedalphadraw)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &MapForm::OnNMReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_RADIOR, &MapForm::OnBnClickedRadior)
	ON_BN_CLICKED(IDC_RADIORG, &MapForm::OnBnClickedRadiorg)
	ON_BN_CLICKED(IDC_RADIORB, &MapForm::OnBnClickedRadiorb)
	ON_LBN_SELCHANGE(IDC_LIST1, &MapForm::OnSelchangeList1)
	ON_BN_DOUBLECLICKED(IDC_RADIOR, &MapForm::OnBnDoubleclickedRadior)
	ON_BN_DOUBLECLICKED(IDC_RADIORG, &MapForm::OnBnDoubleclickedRadiorg)
	ON_BN_DOUBLECLICKED(IDC_RADIORB, &MapForm::OnBnDoubleclickedRadiorb)
	ON_BN_CLICKED(IDC_BUTTON2, &MapForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &MapForm::OnBnClickedButton1)
END_MESSAGE_MAP()


// MapForm 메시지 처리기


void MapForm::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	iRowLenth = pow(2, iRowLenth);
	iCellCount = pow(2, iCellCount);
	pApp->m_Tool.LoadMap(iRowLenth, iCellCount, icellSize);
	/*pApp->m_Tool.m_BaseParticle.m_vVelocity.x = m_fGravityX;
	pApp->m_Tool.m_BaseParticle.m_vVelocity.y = m_fGravityY;
	pApp->m_Tool.m_BaseParticle.m_vVelocity.z = m_fGravityZ;*/
}


void MapForm::OnEnChangeirowlen()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MapForm::OnBnClickedheight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.IsSetMapHeight = b_SetHeight;
	 
}


void MapForm::OnBnClickedtexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.IsSetMapTex = b_SetTex;
}




void MapForm::OnBnClickedLoadalphatex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString selFileName;
	CFileDialog dlg(
		TRUE, L"dds", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"dds Files(*.dds)|*.dds|All Files(*.*)|*.*|", this
	);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();

		//selFileName += dlg.GetFileName();		
		//AfxMessageBox(selFileName);
	
		int iID = I_TextureMgr.Load(g_pDevice, selFileName.GetBuffer());
		if (iID > 0)
		{
			
			CString texname = dlg.GetFileTitle() + "." + dlg.GetFileExt();
			m_tex_list.InsertString(m_tex_list.GetCount(), texname);
			m_texMap.insert(make_pair(texname, iID));
		}
	}
	

}
void MapForm::OnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();

	int i = 0;
	CString str;
	m_tex_list.GetText(m_tex_list.GetCurSel(), str);

	ITOR itor = m_texMap.find(str);

	if (itor != m_texMap.end())
	{
		int texidx = (*itor).second;
		pApp->m_Tool.m_changedtexID = texidx;
	}
	
}



void MapForm::OnBnClickedalphadraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.IsDrawAlpha = b_SetAlphaTex;
}



void MapForm::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	float br = brushrad.GetPos();
	pApp->m_Tool.Setrad(br);
	*pResult = 0;
}


void MapForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	brushrad.SetRange(-50, 50);
	brushrad.SetPos(0);
	brushrad.SetTicFreq(1);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}




void MapForm::OnBnClickedSetbrushcolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString selCol;
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		selCol.Format(L"%u,%u,%u",
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		//AfxMessageBox(selCol);
		
	}
}


void MapForm::OnBnClickedRadior()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(1,0,0,1));
}


void MapForm::OnBnClickedRadiorg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(0, 1, 0, 1));
}


void MapForm::OnBnClickedRadiorb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(0, 0, 1, 1));
}




void MapForm::OnBnDoubleclickedRadior()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString selFileName;
	CFileDialog dlg(
		TRUE, L"dds", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"dds Files(*.dds)|*.dds|All Files(*.*)|*.*|", this
	);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();

		//selFileName += dlg.GetFileName();		
		//AfxMessageBox(selFileName);

		int iID = I_TextureMgr.Load(g_pDevice, selFileName.GetBuffer());
		CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
		pApp->m_Tool.m_Map.SetBGTex(0, iID);
		pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(1, 0, 0, 1));
	}
}


void MapForm::OnBnDoubleclickedRadiorg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString selFileName;
	CFileDialog dlg(
		TRUE, L"dds", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"dds Files(*.dds)|*.dds|All Files(*.*)|*.*|", this
	);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();

		//selFileName += dlg.GetFileName();		
		//AfxMessageBox(selFileName);

		int iID = I_TextureMgr.Load(g_pDevice, selFileName.GetBuffer());
		CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
		pApp->m_Tool.m_Map.SetBGTex(1, iID);
		pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(0, 1, 0, 1));
	}
}


void MapForm::OnBnDoubleclickedRadiorb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString selFileName;
	CFileDialog dlg(
		TRUE, L"dds", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"dds Files(*.dds)|*.dds|All Files(*.*)|*.*|", this
	);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();

		//selFileName += dlg.GetFileName();		
		//AfxMessageBox(selFileName);

		int iID = I_TextureMgr.Load(g_pDevice, selFileName.GetBuffer());
		CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
		pApp->m_Tool.m_Map.SetBGTex(2, iID);
		pApp->m_Tool.m_brush.SetColor(D3DXVECTOR4(0, 0, 1, 1));
	}
}


void MapForm::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.nodeTexList.clear();
	for (ITOR itor = m_texMap.begin(); itor != m_texMap.end(); itor++)
	{
		int texidx = (*itor).second;
		pApp->m_Tool.nodeTexList.push_back(texidx);
	}
	pApp->m_Tool.FileSave();
}


void MapForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMapToolApp* pApp = (CMapToolApp*)AfxGetApp();
	pApp->m_Tool.LoadMapFromFile();
}
