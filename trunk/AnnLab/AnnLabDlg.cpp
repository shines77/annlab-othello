// AnnLabDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AnnLab.h"
#include "AnnLabDlg.h"
#include "CxList.h"
#include "CxMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

	BpNetwork_ShowTest();

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
