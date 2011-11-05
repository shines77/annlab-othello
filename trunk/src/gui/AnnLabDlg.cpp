// AnnLabDlg.cpp : 实现文件
//

#include "../../include/gui/stdafx.h"
#include "../../include/gui/AnnLabApp.h"
#include "../../include/gui/AnnLabDlg.h"
#include "../../include/annlab/annlab_stddef.h"
#include "../../include/annlab/sys/AnnList.h"
#include "../../include/annlab/sys/AnnMatrix.h"
#include "../../include/annlab/sys/matrixt.h"
#include "../../include/annlab/toolbox/matlab/elmat.h"
#include "../../include/annlab/toolbox/nnet/nninit.h"
#include "../../include/annlab/toolbox/nnet/nntrain.h"
#include "../../include/annlab/toolbox/nnet/nnnetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace annlab;

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
	int nRetCode = 0;
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
	net.set_name( _T("BP Network") );

	CAnnMatrix trainPx, trainTx;
	trainPx.rands(64, 64);
	trainTx.rands(64, 1);

	CAnnMatrix temp, rands;
	temp = annlab::ones(16, 16);
	rands = annlab::rands2(16, 16);
	temp = 1.0 - rands;
	temp = 1.0 + rands;
	temp *= 3.0;
	temp /= 3.0;
	temp = 3.0 / rands;

	CAnnMatrix inputMinMax;
	inputMinMax.create_ex( _T("inputMinMax"), 64, 2, MAT_INIT_ONES );

	pNetwork = net.newff( &inputMinMax, _T("64, 65, 65, 33, 1"), _T("tansig, tansig, tansig, purelin"), _T("traincgf") );
	pNetwork = net.init();

	nRetCode = annlab::newff( &net, &inputMinMax, _T("64, 65, 65, 33, 1"), _T("tansig, tansig, tansig, purelin"), _T("traincgf") );
	nRetCode = annlab::init( &net );

	net.trainParam.show = TRUE;
	net.trainParam.epochs = 2000;

	CAnnMatrixList trainP, trainT;
	trainP.size();

	CAnnTrainRecord tr;
	pNetwork = net.train( &trainP, &trainT, &tr );
	nRetCode = annlab::train( &net, NULL, &trainP, &trainT, NULL );

	CAnnMatrix m(3, 4), m2(3, 4), m3(3, 4), n;
	//double data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	double data[] = {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12};
	m.set_data(data, 3, 4);
    m2.set_data(data, 3, 4);
    m3.set_data(data, 3, 4);
    m3 = m2 = m + m2 + m3;
    m3 = m2 - m;
	n = annlab::normr(m);
	//n.display(_T("n = normr(m);"));

	n = annlab::norm(m);
	//n.display(_T("n = norm(m);"));

	n = annlab::diag(m);
	//n.display(_T("n = diag(m);"));

    MatrixT<double> mt1;
    mt1.size();
    mt1.resize(4, 5);

    MatrixT<double> mt2(5, 5);
    //mt2.resize(4, 5);
    mt2[1][1] = 0.2783;
    mt2[2][2] = 3.32178E+64;
    mt2.display_ex();

    MatrixT<int> mt3;
    mt3.set_name(_T("mt3"));
    mt3.resize(6, 6);
    mt3.clear(INIT_FCN_SPECIFIED, 3);
    mt3(0, 0) = 2;
    mt3[1][1] = 5;
    mt3.display_ex();

    mt1 = mt2;
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
	//::ShowMessage( GetSafeHwnd(), _T("indexLayer"), indexLayer );
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
