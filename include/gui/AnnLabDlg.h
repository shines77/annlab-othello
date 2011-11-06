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
    afx_msg void OnBnClickedBtnTest();
	DECLARE_MESSAGE_MAP()

private:
	int  BpNetwork_Test();
    int  Matrix_Test();
    int  Matrix_Test2();
    int  Matrix_Test3();
	void BpNetwork_ShowTest();
	SHOWMESSAGE_DECLEAR;

private:
	CAnnNetwork m_net;
public:
    afx_msg void OnBnClickedBtnTest2();
    afx_msg void OnBnClickedBtnTest3();
    afx_msg void OnBnClickedBtnTest4();
};
