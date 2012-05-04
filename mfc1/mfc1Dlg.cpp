// mfc1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfc1.h"
#include "mfc1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMfc1Dlg dialog

CMfc1Dlg::CMfc1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMfc1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMfc1Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfc1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMfc1Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMfc1Dlg, CDialog)
	//{{AFX_MSG_MAP(CMfc1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMfc1Dlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_PIC, &CMfc1Dlg::OnStnClickedPic)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfc1Dlg message handlers

BOOL CMfc1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
		// 获得桌面大小
	CRect rectWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);   

	// 获得对话框大小
	CRect rectDlg;
	GetWindowRect(&rectDlg);
	int nW = rectDlg.Width();
	int nH = rectDlg.Height();

	// 将窗口设置到右下角
	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_NOACTIVATE);

	// 动画显示弹窗效果
	::AnimateWindow(GetSafeHwnd(),800,AW_SLIDE|AW_VER_NEGATIVE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMfc1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMfc1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMfc1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//void CMfc1Dlg::OnButton1() { _Module.Install();	}

//void CMfc1Dlg::OnButton2() { _Module.Uninstall(); }
void CMfc1Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CMfc1Dlg::OnStnClickedPic()
{
	// TODO: 在此添加控件通知处理程序代码
	::ShellExecute(NULL, "open", "http://www.baidu.com", NULL, NULL, 0);
	OnOK();
}
