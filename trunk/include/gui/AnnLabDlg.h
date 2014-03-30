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
