// AnnLabDlg.cpp : ʵ���ļ�
//

#include "../../include/gui/stdafx.h"
#include "../../include/gui/AnnLabApp.h"
#include "../../include/gui/AnnLabDlg.h"
#include "../../include/annlab/annlab_stddef.h"
#include "../../include/annlab/tickcount_t.h"
#include "../../include/annlab/itimer_t.h"
#include "../../include/annlab/sys/AnnList.h"
#include "../../include/annlab/sys/AnnMatrix.h"
#include "../../include/annlab/sys/matrixt.h"
#include "../../include/annlab/sys/matrix.h"
#include "../../include/annlab/toolbox/matlab/elmat.h"
#include "../../include/annlab/toolbox/nnet/nninit.h"
#include "../../include/annlab/toolbox/nnet/nntrain.h"
#include "../../include/annlab/toolbox/nnet/nnnetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace annlab;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAnnLabDlg �Ի���

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
    ON_BN_CLICKED(IDC_BTN_TEST1, &CAnnLabDlg::OnBnClickedBtnTest)
    ON_BN_CLICKED(IDC_BTN_TEST2, &CAnnLabDlg::OnBnClickedBtnTest2)
    ON_BN_CLICKED(IDC_BTN_TEST3, &CAnnLabDlg::OnBnClickedBtnTest3)
    ON_BN_CLICKED(IDC_BTN_TEST4, &CAnnLabDlg::OnBnClickedBtnTest4)
END_MESSAGE_MAP()

SHOWMESSAGE_IMP(CAnnLabDlg)


int CAnnLabDlg::Matrix_Test()
{
#if _DEBUG
    int size = 500;
    int N = 1;
#else
    int size = 1024;
    int N = 2;
#endif

    SetProcessAffinityMask(GetCurrentProcess(), 2);
    SetThreadAffinityMask(GetCurrentProcess(), 2);

    // MatrixT
    // size and N are specified by the user on the command line
    annlab::MatrixT<double> A = annlab::MatrixT<double>::_rands(size, size);
    annlab::MatrixT<double> B = annlab::MatrixT<double>::_rands(size, size);
    annlab::MatrixT<double> C = annlab::MatrixT<double>::_rands(size, size);
    annlab::MatrixT<double> Z = annlab::MatrixT<double>::_zeros(size, size);
    int i;

    annlab::tickcount_t timer;
    TCHAR szBuffer[256], szText[512];

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("MatrixT: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscpy_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("MatrixT: time taken for multiplication = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    //::ShowMessage( GetSafeHwnd(), szText );
    //_tcscpy_s(szText, _countof(szText), _T(""));

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B + C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("MatrixT: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscat_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B * C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("MatrixT: time taken for multiplication = %0.5f sec(s)."), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    ::ShowMessage( GetSafeHwnd(), szText );

    return 0;
}

int CAnnLabDlg::Matrix_Test2()
{
#if _DEBUG
    int size = 500;
    int N = 1;
#else
    int size = 1000;
    int N = 2;
#endif

    SetProcessAffinityMask(GetCurrentProcess(), 2);
    SetThreadAffinityMask(GetCurrentProcess(), 2);

    //Armadillo
    // size and N are specified by the user on the command line
    annlab::Matrix<double> A(size, size);
    annlab::Matrix<double> B(size, size);
    annlab::Matrix<double> C(size, size);
    annlab::Matrix<double> Z(size, size);
    int i;
    A.fill(1);
    B.fill(1);
    C.fill(1);
    Z.fill(2);

    annlab::tickcount_t timer;
    TCHAR szBuffer[256], szText[512];

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("splab::matrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscpy_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("splab::matrix: time taken for multiplication = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    //::ShowMessage( GetSafeHwnd(), szText );
    //_tcscpy_s(szText, _countof(szText), _T(""));

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B + C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("splab::matrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscat_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B * C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("splab::matrix: time taken for multiplication = %0.5f sec(s)."), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    ::ShowMessage( GetSafeHwnd(), szText );

    return 0;
}

int CAnnLabDlg::Matrix_Test3()
{
#if _DEBUG
    int size = 500;
    int N = 1;
#else
    int size = 1000;
    int N = 1;
#endif

    SetProcessAffinityMask(GetCurrentProcess(), 2);
    SetThreadAffinityMask(GetCurrentProcess(), 2);

    //Armadillo
    // size and N are specified by the user on the command line
    annlab::CAnnMatrix A(size, size);
    annlab::CAnnMatrix B(size, size);
    annlab::CAnnMatrix C(size, size);
    annlab::CAnnMatrix Z(size, size);
    int i;
    A.rands();
    B.rands();
    C.rands();
    Z.zeros();

    annlab::tickcount_t timer;
    TCHAR szBuffer[256], szText[512];

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("CAnnMatrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscpy_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("CAnnMatrix: time taken for multiplication = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    //::ShowMessage( GetSafeHwnd(), szText );
    //_tcscpy_s(szText, _countof(szText), _T(""));

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B + C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("CAnnMatrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscat_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B * C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("CAnnMatrix: time taken for multiplication = %0.5f sec(s)."), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    ::ShowMessage( GetSafeHwnd(), szText );

    return 0;
}

int CAnnLabDlg::BpNetwork_Test()
{
    annlab::itimer_t timer;
    TCHAR szBuffer[256], szText[512];

    timer.begin();

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
    double a = abs(0.0);

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
    mt3.fill(INIT_FCN_SPECIFIED, 3);
    mt3(0, 0) = 2;
    mt3[1][1] = 5;
    mt3.display_ex();

    mt1 = mt2;

    _stprintf_s(szBuffer, _countof(szBuffer), _T("BpNetwork_Test() used time = %0.5f sec(s)."), timer.end().seconds());
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage( GetSafeHwnd(), szText );
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

// CAnnLabDlg ��Ϣ�������

BOOL CAnnLabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//BpNetwork_ShowTest();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAnnLabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAnnLabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAnnLabDlg::OnBnClickedBtnTest()
{
    // �������һ
    int indexLayer = Matrix_Test();
}

void CAnnLabDlg::OnBnClickedBtnTest2()
{
    // ������Զ�
    int indexLayer = Matrix_Test2();
}

void CAnnLabDlg::OnBnClickedBtnTest3()
{
    // ���������
    int indexLayer = Matrix_Test3();
}

void CAnnLabDlg::OnBnClickedBtnTest4()
{
    // �����������
    BpNetwork_ShowTest();
}
