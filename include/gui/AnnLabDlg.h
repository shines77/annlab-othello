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
	DECLARE_MESSAGE_MAP()

private:
	int  BpNetwork_Test();
	void BpNetwork_ShowTest();

	SHOWMESSAGE_DECLEAR;

private:
	CAnnNetwork m_net;
};
