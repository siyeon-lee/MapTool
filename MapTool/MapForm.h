#pragma once


// MapForm 대화 상자

class MapForm : public CFormView
{
	DECLARE_DYNAMIC(MapForm)

public:
	MapForm();   // 표준 생성자입니다.
	virtual ~MapForm();
	static MapForm* CreateOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeirowlen();
	afx_msg void OnBnClickedCancel();
	

	int iRowLenth;
	int iCellCount;
	int icellSize;
	// 클릭시 높이 변경
	BOOL b_SetHeight;
	BOOL b_SetTex;
	typedef std::map<CString, int>::iterator ITOR;
	std::map<CString, int>   m_texMap;	//path+name+ext texindex, 
	afx_msg void OnBnClickedheight();
	afx_msg void OnBnClickedtexture();


	afx_msg void OnBnClickedLoadalphatex();
	afx_msg void OnBnClickedalphadraw();
	BOOL b_SetAlphaTex;
	CSliderCtrl brushrad;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedSetbrushcolor();
	int iRadSlot;
	afx_msg void OnBnClickedRadior();
	afx_msg void OnBnClickedRadiorg();
	afx_msg void OnBnClickedRadiorb();
	CListBox m_tex_list;
	afx_msg void OnSelchangeList1();

	afx_msg void OnBnDoubleclickedRadior();
	afx_msg void OnBnDoubleclickedRadiorg();
	afx_msg void OnBnDoubleclickedRadiorb();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
