
// FileClientDlg.h: 头文件
//

#pragma once
#include "../CBlockingSocket.h"
#include "../UdpSocket.hpp"
#include "../Common.h"

// CFileClientDlg 对话框
class CFileClientDlg : public CDialogEx
{
// 构造
public:
	CFileClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILECLIENT_DIALOG };
#endif

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
	DECLARE_MESSAGE_MAP()
public:
	CBlockingSocket fileClient;
	char buffer[MAX_BUF_SIZE];

	bool m_started;

	CString m_ip;
	CIPAddressCtrl m_ip_ctrl;
	CString m_port;
	CString m_filepath;
	CString m_content;
	CString m_savepath;

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRequestBtn();
};
