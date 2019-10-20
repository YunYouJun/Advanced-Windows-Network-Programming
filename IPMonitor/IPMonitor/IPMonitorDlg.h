
// IPMonitorDlg.h: 头文件
//

#pragma once
#include "Device.h"
#include "NIC.h"
#include "EthernetFrame.h"
#include "IP.h"

#define WM_RECV (WM_USER + 100)

// CIPMonitorDlg 对话框
class CIPMonitorDlg : public CDHtmlDialog
{
// 构造
public:
	CIPMonitorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPMONITOR_DIALOG, IDH = IDR_HTML_IPMONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonScan(IHTMLElement* pElement);
	HRESULT OnButtonCancel(IHTMLElement* pElement);
	HRESULT OnButtonAbout(IHTMLElement* pElement);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRecv(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	NIC nic;
	int index;
	Device device;
	bool started;
	CWinThread* hThreadrecv;

	DWORD local_ip;

	DWORD srcip;
	DWORD dstip;
	BYTE proto;

	CString btnScanText = "Scan";

	CComboBox m_CComboBox;
	CListCtrl m_CListCtrl;
	CString m_nic_ipaddress;
	CString m_nic_subnetmask;
	CString m_nic_macaddress;
	CString m_nic_gatewayipaddress;
	CString m_nic_gatewaymac;

	CButton m_CheckBoxSource;
	CButton m_CheckBoxDestination;

	bool src_checked;
	bool dst_checked;

	afx_msg void OnSelectChangeNic();
	afx_msg void OnBnClickedCheckSource();
	afx_msg void OnBnClickedCheckDestination();
	static UINT recvPacket(LPVOID lpParam);
};
