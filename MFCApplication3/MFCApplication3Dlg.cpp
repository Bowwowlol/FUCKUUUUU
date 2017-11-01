
// MFCApplication3Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;

//Fuck VS

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援


// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 對話方塊



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Cam_Start, m_Cam_Start);
	DDX_Control(pDX, IDC_Pause, m_Pause);
	DDX_Control(pDX, IDC_Method_Selection, m_Method_Selection);
	DDX_Control(pDX, IDC_VARIABLE1, m_Variable1);
	DDX_Control(pDX, IDC_VARIABLE2, m_Variable2);
	DDX_Control(pDX, IDC_VARIABLE3, m_Variable3);
	DDX_Control(pDX, IDC_PARAMETER1, m_Parameter1);
	DDX_Control(pDX, IDC_PARAMETER2, m_Parameter2);
	DDX_Control(pDX, IDC_PARAMETER3, m_Parameter3);
	DDX_Control(pDX, IDC_SLIDERCONTROL1, m_SliderControl1);
	DDX_Control(pDX, IDC_SLIDERCONTROL2, m_SliderControl2);
	DDX_Control(pDX, IDC_SLIDERCONTROL3, m_SliderControl3);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Cam_Start, &CMFCApplication3Dlg::OnBnClickedCamStart)
	ON_BN_CLICKED(IDC_Pause, &CMFCApplication3Dlg::OnBnClickedPause)
	ON_CBN_SELENDOK(IDC_Method_Selection, &CMFCApplication3Dlg::CBN_SELENDOK_Method_Selection)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL1, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL2, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCONTROL3, &CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol3)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 訊息處理常式

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	
	m_Method_Selection.AddString(_T("TemplateMatch"));
	m_Method_Selection.AddString(_T("MeamShift"));
	m_Method_Selection.AddString(_T("CamShift"));
	m_Method_Selection.SetCurSel(0);

	m_Parameter1.SetWindowTextW(L"TM");
	m_Parameter2.SetWindowTextW(L"NULL");
	m_Parameter3.SetWindowTextW(L"NULL");

	m_SliderControl1.SetRange(0, 10);
	m_SliderControl1.SetPos(0);

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl1.GetPos());

	m_Variable1.SetWindowTextW(buffer);


	m_Parameter2.EnableWindow(0);
	m_Parameter3.EnableWindow(0);

	m_SliderControl2.EnableWindow(0);
	m_SliderControl3.EnableWindow(0);

	m_Variable2.EnableWindow(0);
	m_Variable3.EnableWindow(0);

	m_Pause.EnableWindow(0);

	Cam = STOP;

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication3Dlg::OnBnClickedCamStart()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (Cam != START)
	{
		m_Cam_Start.SetWindowTextW(L"Stop");
		m_Pause.EnableWindow(1);
		Cam = START;
		
	}
	else if (Cam == START)
	{
		m_Cam_Start.SetWindowTextW(L"Start");
		m_Pause.SetWindowTextW(L"Pause");
		m_Pause.EnableWindow(0);
		Cam = STOP;
	}
}



void CMFCApplication3Dlg::OnBnClickedPause()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (Cam != START)
	{
		m_Pause.SetWindowTextW(L"Pause");
	}

	if (Cam != PAUSE)
	{
		Cam = PAUSE;
		m_Cam_Start.EnableWindow(0);
		m_Pause.SetWindowTextW(L"Continue");
	}
	else if (Cam == PAUSE)
	{
		Cam = START;
		m_Cam_Start.EnableWindow(1);
		m_Pause.SetWindowTextW(L"Pause");
	}
	
}


void CMFCApplication3Dlg::CBN_SELENDOK_Method_Selection()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	Get_Combobox_Index = m_Method_Selection.GetCurSel();



	switch (Get_Combobox_Index)
	{
	case 0:

		m_Parameter1.SetWindowTextW(L"TM");
		m_Parameter2.SetWindowTextW(L"NULL");
		m_Parameter3.SetWindowTextW(L"NULL");
	
		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(0);
		m_SliderControl3.EnableWindow(0);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(0);
		m_Parameter3.EnableWindow(0);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(0);
		m_Variable3.EnableWindow(0);

		m_SliderControl1.SetRange(0, 10);
		m_SliderControl1.SetPos(0);

		break;

	case 1:

		m_Parameter1.SetWindowTextW(L"Vmin");
		m_Parameter2.SetWindowTextW(L"Vmax");
		m_Parameter3.SetWindowTextW(L"NULL");

		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(1);
		m_SliderControl3.EnableWindow(0);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(1);
		m_Parameter3.EnableWindow(0);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(1);
		m_Variable3.EnableWindow(0);

		m_SliderControl1.SetRange(0, 255);
		m_SliderControl1.SetPos(0);
		m_SliderControl2.SetRange(0, 255);
		m_SliderControl2.SetPos(0);


		break;

	case 2:
		m_Parameter1.SetWindowTextW(L"Vmin");
		m_Parameter2.SetWindowTextW(L"Vmax");
		m_Parameter3.SetWindowTextW(L"Smin");


		m_SliderControl1.EnableWindow(1);
		m_SliderControl2.EnableWindow(1);
		m_SliderControl3.EnableWindow(1);

		m_Parameter1.EnableWindow(1);
		m_Parameter2.EnableWindow(1);
		m_Parameter3.EnableWindow(1);

		m_Variable1.EnableWindow(1);
		m_Variable2.EnableWindow(1);
		m_Variable3.EnableWindow(1);

		m_SliderControl1.SetRange(0, 255);
		m_SliderControl1.SetPos(0);
		m_SliderControl2.SetRange(0, 255);
		m_SliderControl2.SetPos(0);
		m_SliderControl3.SetRange(0, 255);
		m_SliderControl3.SetPos(0);
		break;
	}



	
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼

	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl1.GetPos());
	m_Variable1.SetWindowTextW(buffer);

	*pResult = 0;
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl2.GetPos());
	m_Variable2.SetWindowTextW(buffer);
	*pResult = 0;
}


void CMFCApplication3Dlg::OnNMCustomdrawSlidercontrol3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_SliderControl3.GetPos());
	m_Variable3.SetWindowTextW(buffer);
	*pResult = 0;
}
