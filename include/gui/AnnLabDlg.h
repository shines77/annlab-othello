// AnnLabDlg.h : ͷ�ļ�
//

#pragma once

#include "../../include/annlab/sys/AnnNetwork.h"

// CAnnLabDlg �Ի���
class CAnnLabDlg : public CDialog
{
// ����
public:
	CAnnLabDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ANNLAB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnTest1();
    afx_msg void OnBnClickedBtnTest2();
    afx_msg void OnBnClickedBtnTest3();
    afx_msg void OnBnClickedBtnTest4();
	DECLARE_MESSAGE_MAP()

	int  BpNetwork_Test();
    int  Matrix_Test1();
    int  Matrix_Test2();
    int  Matrix_Test3();
	void BpNetwork_ShowTest();
	void Matrix_Transpose();
    SHOWMESSAGE_DECLEAR;

private:
    void fast_transpose();
    bool verify_transpose(double *C, unsigned int M, unsigned int N,
        int *err_counter = NULL, int order = 0);
    void init_matrix_cell(double *C, unsigned int M, unsigned int N,
        int order = 0);

private:
	CAnnNetwork m_net;
};
