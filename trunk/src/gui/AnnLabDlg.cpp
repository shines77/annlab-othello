// AnnLabDlg.cpp : 实现文件
//

#include "../../include/gui/stdafx.h"
#include "../../include/gui/AnnLabApp.h"
#include "../../include/gui/AnnLabDlg.h"
#include "../../include/annlab/annlab_stddef.h"
#include "../../include/annlab/tickcount_t.h"
#include "../../include/annlab/itimer_t.h"
#include "../../include/annlab/sys/AnnList.h"
#include "../../include/annlab/sys/AnnMatrix.h"
#include "../../include/annlab/sys/fastmatrix.h"
#include "../../include/annlab/sys/matrix.h"
#include "../../include/annlab/toolbox/matlab/elmat.h"
#include "../../include/annlab/toolbox/nnet/nninit.h"
#include "../../include/annlab/toolbox/nnet/nntrain.h"
#include "../../include/annlab/toolbox/nnet/nnnetwork.h"

#include <malloc.h>

//#include <vld.h>

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
    ON_BN_CLICKED(IDC_BTN_TEST1, &CAnnLabDlg::OnBnClickedBtnTest1)
    ON_BN_CLICKED(IDC_BTN_TEST2, &CAnnLabDlg::OnBnClickedBtnTest2)
    ON_BN_CLICKED(IDC_BTN_TEST3, &CAnnLabDlg::OnBnClickedBtnTest3)
    ON_BN_CLICKED(IDC_BTN_TEST4, &CAnnLabDlg::OnBnClickedBtnTest4)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

SHOWMESSAGE_IMP(CAnnLabDlg)


int CAnnLabDlg::Matrix_Test1()
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
    annlab::FastMatrix<double> A = annlab::FastMatrix<double>::_rands(size, size);
    annlab::FastMatrix<double> B = annlab::FastMatrix<double>::_rands(size, size);
    annlab::FastMatrix<double> C = annlab::FastMatrix<double>::_rands(size, size);
    annlab::FastMatrix<double> Z = annlab::FastMatrix<double>::_zeros(size, size);
    int i;

    annlab::tickcount_t timer;
    TCHAR szBuffer[256], szText[512];

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("FastMatrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscpy_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("FastMatrix: time taken for multiplication = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    //::ShowMessage( GetSafeHwnd(), szText );
    //_tcscpy_s(szText, _countof(szText), _T(""));

    timer.begin();
    for(i=0; i<N*10; ++i)
        Z = A + B + C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("FastMatrix: time taken for addition = %0.5f sec(s).\r\n\r\n"), timer.end().seconds() / double(N*10));
    _tcscat_s(szText, _countof(szText), szBuffer);

    timer.begin();
    for(i=0; i<N; ++i)
        Z = A * B * C;  //  or Z = A+B+C ... etc

    _stprintf_s(szBuffer, _countof(szBuffer), _T("FastMatrix: time taken for multiplication = %0.5f sec(s)."), timer.end().seconds() / double(N));
    _tcscat_s(szText, _countof(szText), szBuffer);

    ::ShowMessage( GetSafeHwnd(), szText );

    return 0;
}

int CAnnLabDlg::Matrix_Test2()
{
#if _DEBUG
    int size = 512;
    int N = 1;
#else
    int size = 1024;
    int N = 1;
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
    int size = 512;
    int N = 1;
#else
    int size = 1024;
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

    FastMatrix<double> mt1;
    mt1.size();
    mt1.resize(4, 5);

    FastMatrix<double> mt2(5, 5);
    //mt2.resize(4, 5);
    mt2[1][1] = 0.2783;
    mt2[2][2] = 3.32178E+64;
    mt2.display_ex();

    FastMatrix<int> mt3;
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

void CAnnLabDlg::init_matrix_cell(double *C, unsigned int M, unsigned int N,
                                  int order /* =0 */)
{
    unsigned int m, n;
    int index;

    if (order == 0) {
        for (n = 0; n < N; ++n) {
            for (m = 0; m < M; ++m) {
                index = n * M + m;
                C[n * M + m] = (double)index;
            }
        }
    }
    else if (order == 1) {
        for (n = 0; n < N; ++n) {
            for (m = 0; m < M; ++m) {
                index = m * N + n;
                C[n * M + m] = (double)index;
            }
        }
    }
    else {
        // unknown order type
    }
}

bool CAnnLabDlg::verify_transpose(double *C, unsigned int M, unsigned int N,
                                  int *err_counter /* =NULL */, int order /* =0 */)
{
    bool verify_ok = false;
    const double err_limited = 0.00001;
    unsigned int m, n;
    double cell;
    int index, err_num, last_m = -1, err_num_m;

    if (order == 0) {
        err_num = 0;
        for (n = 0; n < N; ++n) {
            err_num_m = 0;
            for (m = 0; m < M; ++m) {
                cell = C[n * M + m];
                index = m * N + n;
                if (fabs(cell - (double)index) > err_limited) {
                    err_num++;
                    err_num_m++;
                    last_m = m;
                }
#if defined(_DEBUG) && 0
                else {
                    last_m = last_m;
                }
#endif
            }
        }
        if (err_num == 0)
            verify_ok = true;
    }
    else if (order == 1) {
        err_num = 0;
        for (n = 0; n < N; ++n) {
            err_num_m = 0;
            for (m = 0; m < M; ++m) {
                cell = C[n * M + m];
                index = n * M + m;
                if (fabs(cell - (double)index) > err_limited) {
                    err_num++;
                    err_num_m++;
                    last_m = m;
                }
            }
        }
        if (err_num == 0)
            verify_ok = true;
    }
    else {
        // unknown order type
    }
    if (err_counter != NULL)
        *err_counter = err_num;
    return verify_ok;
}

void CAnnLabDlg::fast_transpose()
{
    annlab::itimer_t timer;
    TCHAR szBuffer[256], szText[512];
    const unsigned int M = 1024, N = 1024;
    unsigned int m, n;
    unsigned int M_start, M_end;
    unsigned int N_start, N_end;
    unsigned int step_M, step_N;
    int index;
    double temp;
    int err_num;
    bool verify_ok;

    double *C1;
    C1 = (double *)_aligned_malloc(sizeof(double) * M * N, 4096);

    index = 0;
    for (n = 0; n < N; ++n) {
        for (m = 0; m < M; ++m) {
            C1[n * M + m] = (double)index;
            index++;
        }
    }
    err_num = 0;
    verify_ok = verify_transpose(C1, M, N, &err_num, 1);

    init_matrix_cell(C1, M, N);

    err_num = 0;
    verify_ok = verify_transpose(C1, M, N, &err_num, 1);

    timer.begin();
    step_M = 128;
    step_N = 128;

    M_start = 0;
    M_end   = M_start + step_M;

    while (M_start <= M) {
        N_start = 0;
        N_end   = N_start + step_N;
        while (N_start <= N) {
            // (0 - 512), 512x8 = 4096 bytes(page size), need one TLB entry
            for (m = M_start; m < MIN(M_end, M); ++m) {
                // (0 - 248), need 248 TLB entry
                if (MAX(N_start, m + 1) > MIN(N_end, N))
                    break;
                for (n = MAX(N_start, m + 1); n < MIN(N_end, N); ++n) {
                    //if (n > m) {
                        temp = C1[n * M + m];
                        C1[n * M + m] = C1[m * N + n];
                        C1[m * N + n] = temp;
                    //}
                }
            }
            N_start += step_N;
            N_end   += step_N;
        }
        M_start += step_M;
        M_end   += step_M;
    }
    timer.end();

    err_num = 0;
    verify_ok = verify_transpose(C1, M, N, &err_num);

    _stprintf_s(szBuffer, _countof(szBuffer),
        _T("Matrix_Transpose() - [fast_transpose] used time = %0.5f sec(s).\n\nverify_ok = %d"),
        timer.end().seconds(), verify_ok);
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage(GetSafeHwnd(), szText);

    if (C1) {
        _aligned_free(C1);
    }
}

void CAnnLabDlg::Matrix_Transpose()
{
#if 1
    TCHAR szBuffer[256], szText[512];
    const unsigned int M = 1024, N = 1024;
    unsigned int m, n;
    double temp;
    annlab::itimer_t timer;

    CAnnMatrix mat(M, N);
    //mat.ones();

    mat.order();
    //mat.display();

    timer.begin();
    mat.transpose();
    //mat.display();
    timer.end();

    _stprintf_s(szBuffer, _countof(szBuffer), _T("Matrix_Transpose() - [CAnnMatrix] used time = %0.5f sec(s)."), timer.end().seconds());
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage(GetSafeHwnd(), szText);

    FastMatrix<double> fmat(M, N);
    //fmat.ones();

    fmat.order();
    //fmat.display();

    timer.begin();
    fmat = fmat.transpose();
    //fmat.display();
    timer.end();

    _stprintf_s(szBuffer, _countof(szBuffer), _T("Matrix_Transpose() - [FastMatrix<double>] used time = %0.5f sec(s)."), timer.end().seconds());
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage(GetSafeHwnd(), szText);

    double *C1, *C2;
    C1 = (double *)malloc(sizeof(double) * M * N);
    timer.begin();
    for (m = 0; m < M; ++m) {
        for (n = m + 1; n < N; ++n) {
            temp = C1[n * M + m];
            C1[n * M + m] = C1[m * N + n];
            C1[m * N + n] = temp;
        }
    }
    timer.end();

    _stprintf_s(szBuffer, _countof(szBuffer), _T("Matrix_Transpose() C1 used time = %0.5f sec(s)."), timer.end().seconds());
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage(GetSafeHwnd(), szText);

    C2 = (double *)malloc(sizeof(double) * M * N);
    timer.begin();
    for (n = 0; n < N; ++n) {
        for (m = n + 1; m < M; ++m) {
            temp = C1[n * M + m];
            C1[n * M + m] = C1[m * N + n];
            C1[m * N + n] = temp;
        }
    }
    timer.end();

    _stprintf_s(szBuffer, _countof(szBuffer), _T("Matrix_Transpose() C2 used time = %0.5f sec(s)."), timer.end().seconds());
    _tcscpy_s(szText, _countof(szText), szBuffer);

    ::ShowMessage(GetSafeHwnd(), szText);

    if (C1) {
        free(C1);
    }
    if (C2) {
        free(C2);
    }
#endif

    fast_transpose();
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

	//BpNetwork_ShowTest();

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

void CAnnLabDlg::OnBnClickedBtnTest1()
{
    // 矩阵测试一
    int indexLayer = Matrix_Test1();
}

void CAnnLabDlg::OnBnClickedBtnTest2()
{
    // 矩阵测试二
    int indexLayer = Matrix_Test2();
}

void CAnnLabDlg::OnBnClickedBtnTest3()
{
    // 矩阵测试三
    int indexLayer = Matrix_Test3();
}

void CAnnLabDlg::OnBnClickedBtnTest4()
{
    // 其他矩阵测试
    //BpNetwork_ShowTest();
    Matrix_Transpose();
}
