
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
#define WM_HOST (WM_USER + 101)

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

	// custom
	afx_msg LRESULT OnComplete(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	DWORD hostipp;

	NIC nic;
	int index;
	Device device;
	BYTE dstmac[6];
	BYTE srcmac[6];
	DWORD localip;
	DWORD netmask;
	DWORD ipbeg;
	DWORD ipend;
	bool started = false;
	CWinThread* hThreadsend;
	CWinThread* hThreadrecv;
	SYSTEMTIME timesend;
	SYSTEMTIME timerecv;
	time_t diff;
	DWORD hostip;
	BYTE hostmac[6];

	CString btnScanText;

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

	CString m_host_list;
	CString m_host_list_table_head = "<table width=100% class=list border=1 bgcolor=black cellspacing=0 style=font-size:12px><thead><tr><th width=25%>IP Address</th><th width=25%>MAC Address</th><th width=25%>Host Name</th><th width=25%>Arp Time</th></tr></thead><tbody style=text-align:center>";
	CString m_host_list_table_tail = "</tbody></table>";
	CString m_host_list_table_content;
public:
	//IHTMLElement* pElement
	afx_msg void OnSelectChangeNic();

	static UINT sendPacket(LPVOID lpParam);
	static UINT recvPacket(LPVOID lpParam);
};
