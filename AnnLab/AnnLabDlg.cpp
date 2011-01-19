// AnnLabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AnnLab.h"
#include "AnnLabDlg.h"
#include "CxList.h"
#include "CxMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAnnLabDlg 对话框

CAnnLabDlg::CAnnLabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnLabDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnnLabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAnnLabDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SHOWMESSAGE_IMP(CAnnLabDlg)

int CAnnLabDlg::BpNetwork_Test()
{
	CAnnNetwork net, *pNetwork;
	net.setNumInputs( 0 );
	net.setDivideFcn( _T("Dividerand") );
	int numLayers = net.numLayers;
	int indexLayer = 0;
	net.layers.append( 2 );
	try {
		indexLayer = net.layers[0].index;
	}
	catch( TCHAR *errStr ) {
		::ShowMessage( GetSafeHwnd(), errStr );
	}
	catch (...) {
		::ShowMessage( GetSafeHwnd(), _T("Error: indexLayer"), indexLayer );
	}
	net.setName( _T("BP Network") );

	CxMatrix trainPx, trainTx;
	trainPx.rands(64, 64);
	trainTx.rands(64, 1);

	CxMatrix inputMinMax;
	inputMinMax.CreateEx( _T("inputMinMax"), 64, 2, MAT_INIT_ONES );

	pNetwork = net.newff( &inputMinMax, _T("64, 65, 65, 33, 1"), _T("tansig, tansig, tansig, purelin"), _T("traincgf") );
	pNetwork = net.init();

	net.trainParam.show = TRUE;
	net.trainParam.epochs = 2000;

	CxMatrixList trainP, trainT;

	CxTrainRecord tr;
	pNetwork = net.train(&trainP, &trainT, &tr);
	return indexLayer;
}

void CAnnLabDlg::BpNetwork_ShowTest()
{
	int indexLayer;
#if 1
	indexLayer = BpNetwork_Test();
#else
	///*
	__try {
		indexLayer = BpNetwork_Test();
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		indexLayer = -1;
	}
	//*/
#endif
	::ShowMessage( GetSafeHwnd(), _T("indexLayer"), indexLayer );
	//ShowMessage( _T("indexLayer"), indexLayer );
}

// CAnnLabDlg 消息处理程序

BOOL CAnnLabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	BpNetwork_ShowTest();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAnnLabDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAnnLabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAnnLabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
