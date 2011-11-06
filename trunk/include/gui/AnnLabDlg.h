// AnnLabDlg.h : 头文件
//

#pragma once

#include "../../include/annlab/sys/AnnNetwork.h"

// CAnnLabDlg 对话框
class CAnnLabDlg : public CDialog
{
// 构造
public:
	CAnnLabDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ANNLAB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
