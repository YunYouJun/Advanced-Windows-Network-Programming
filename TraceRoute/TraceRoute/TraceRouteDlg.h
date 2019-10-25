
// TraceRouteDlg.h: 头文件
//

#pragma once

#include "NIC.h"
#include "Device.h"
#include "EthernetFrame.h"
#include "ICMP.h"

#define WM_RECV (WM_USER + 100)
#define WM_HOST (WM_USER + 101)

#define ICMP_REQUEST_ID 0x1234

// CTraceRouteDlg 对话框
class CTraceRouteDlg : public CDialogEx
{
// 构造
public:
	CTraceRouteDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRACEROUTE_DIALOG };
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

	afx_msg LRESULT OnRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHost(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	NIC nic;
	int index;
	Device device;
	bool started;
	DWORD m_dstip;
	DWORD m_srcip;
	DWORD m_mask;
	BYTE *m_srcmac;
	BYTE *m_dstmac;
	BYTE *m_gwmac;
	CWinThread *hThread;
	BYTE ttl;
	SYSTEMTIME timesend;
	SYSTEMTIME timerecv;
	DWORD routerip;
	time_t timesub;

	CListCtrl m_CListCtrl;
	CIPAddressCtrl m_CIPAddressCtrl;
public:
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonTrace();
	void icmpRequest(BYTE *buffer, BYTE ttl);
	static UINT CTraceRouteDlg::sendAndRecv(LPVOID lpParam);
	static UINT CTraceRouteDlg::getHostName(LPVOID lpParam);
	afx_msg void OnBnClickedCancel();
};
