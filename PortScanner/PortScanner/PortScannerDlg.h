
// PortScannerDlg.h: 头文件
//

#pragma once
#include "NIC.h"
#include "Device.h"
#include "EthernetFrame.h"
#include "TCP.h"

#define WM_RECV (WM_USER + 100)
#define WM_RECV_FAIL (WM_USER + 101)
#define WM_END (WM_USER + 102)
#define WM_UPDATE (WM_USER + 103)
#define MY_PORT 0xabcd

// CPortScannerDlg 对话框
class CPortScannerDlg : public CDialogEx
{
// 构造
public:
	CPortScannerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PORTSCANNER_DIALOG };
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
	afx_msg LRESULT OnRecvFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonScan();
	static UINT send(LPVOID lpParam);
	static UINT recv(LPVOID lpParam);
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
	WORD m_sport;
	WORD m_eport;
	int count;
	WORD sp;
	WORD dp;

	CComboBox m_CComboBox;
	CListCtrl m_CListCtrl;
	CString m_startingport;
	CString m_endingport;
	CIPAddressCtrl m_ipaddress;
	UINT checked;
	CWinThread *hThreadsend;
	CWinThread *hThreadrecv;

	int m_curport;
};
