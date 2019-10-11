
// LANScannerDlg.h: 头文件
//

#pragma once
#include "NIC.h"
#include "Device.h"
#include "EthernetFrame.h"
#include "PDU.h"
#include "ARP.h"

// Declare user message ID
#define WM_COMPLETE (WM_USER + 100)

// CLANScannerDlg 对话框
class CLANScannerDlg : public CDHtmlDialog
{
	// 构造
public:
	CLANScannerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LANSCANNER_DIALOG, IDH = IDR_HTML_LANSCANNER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement* pElement);
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
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	NIC nic;
	int index;
	Device device;
	BYTE dstmac[6];
	BYTE srcmac[6];
	DWORD localip;
	DWORD netmask;
	DWORD ipbeg;
	DWORD ipend;
	bool started;
	CWinThread* hThreadsend;
	CWinThread* hThreadrecv;
	SYSTEMTIME timesend;
	SYSTEMTIME timerecv;
	time_t diff;
	DWORD hostip;
	BYTE hostmac[6];

	CComboBox m_CComboBox;
	long m_nic_index;
	long m_nic_select_index;
	CString m_nic_select_string;
	//CComPtr<IHTMLElement> m_nic_select;
	CString m_nic_name;
	CString m_nic_select;
	CString m_nic_ipaddress;
	CString m_nic_subnetmask;
	CString m_nic_macaddress;
	CString m_nic_gatewayipaddress;
	CString m_nic_gatewaymac;

public:
	//IHTMLElement* pElement
	afx_msg void OnSelectChangeNic();
};
