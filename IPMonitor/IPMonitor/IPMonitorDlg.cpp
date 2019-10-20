
// IPMonitorDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "IPMonitor.h"
#include "IPMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPMonitorDlg 对话框

BEGIN_DHTML_EVENT_MAP(CIPMonitorDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonScan"), OnButtonScan)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("ButtonAbout"), OnButtonAbout)
END_DHTML_EVENT_MAP()


CIPMonitorDlg::CIPMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_IPMONITOR_DIALOG, IDR_HTML_IPMONITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_NIC, m_CComboBox);
	DDX_Control(pDX, IDC_LIST_IP, m_CListCtrl);

	DDX_Control(pDX, IDC_CHECK_SOURCE, m_CheckBoxSource);
	DDX_Control(pDX, IDC_CHECK_DESTINATION, m_CheckBoxDestination);

	DDX_DHtml_ElementInnerText(pDX, _T("ButtonScan"), btnScanText);
	DDX_DHtml_ElementValue(pDX, _T("ipAddress"), m_nic_ipaddress);
	DDX_DHtml_ElementValue(pDX, _T("subnetMask"), m_nic_subnetmask);
	DDX_DHtml_ElementValue(pDX, _T("macAddress"), m_nic_macaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayIPAddress"), m_nic_gatewayipaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayMac"), m_nic_gatewaymac);
}

BEGIN_MESSAGE_MAP(CIPMonitorDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_CBN_SELCHANGE(IDC_COMBO_NIC, &CIPMonitorDlg::OnSelectChangeNic)
	ON_MESSAGE(WM_RECV, OnRecv)
	ON_BN_CLICKED(IDC_CHECK_SOURCE, &CIPMonitorDlg::OnBnClickedCheckSource)
	ON_BN_CLICKED(IDC_CHECK_DESTINATION, &CIPMonitorDlg::OnBnClickedCheckDestination)
END_MESSAGE_MAP()


// CIPMonitorDlg 消息处理程序

BOOL CIPMonitorDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	index = -1;
	nic.initNIC();
	vector<char*> al = nic.getAdapterList();
	int row = 0;

	// display nic info
	for (vector<char*>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	{
		m_CComboBox.InsertString(row, *iter);
		++row;
	}

	//https://docs.microsoft.com/en-us/cpp/mfc/reference/clistctrl-class
	//The leftmost column in a list view control must be left - aligned.
	m_CListCtrl.InsertColumn(0, _T("Source"), LVCFMT_LEFT, 150, -1);
	m_CListCtrl.InsertColumn(1, _T("Destination"), LVCFMT_LEFT, 150, -1);
	m_CListCtrl.InsertColumn(2, _T("Protocol"), LVCFMT_LEFT, 90, -1);
	m_CListCtrl.InsertColumn(3, _T("Packets"), LVCFMT_LEFT, 90, -1);

	src_checked = m_CheckBoxSource.GetCheck();
	dst_checked = m_CheckBoxDestination.GetCheck();

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIPMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIPMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIPMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CIPMonitorDlg::OnButtonScan(IHTMLElement* /*pElement*/)
{
	if (-1 == index)
		return -1;

	if (!started)
	{
		started = true;

		btnScanText = "Stop";
		GetDlgItem(IDC_COMBO_NIC)->EnableWindow(FALSE);
		m_CListCtrl.DeleteAllItems();

		device.obtainDeviceList();
		device.openAdapter(nic.getNICname());
		string filter = "ip";
		device.setFilter(filter.c_str());

		hThreadrecv = AfxBeginThread(recvPacket, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else
	{
		started = false;
		btnScanText = "Scan";
		GetDlgItem(IDC_COMBO_NIC)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
	return S_OK;
}

HRESULT CIPMonitorDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

HRESULT CIPMonitorDlg::OnButtonAbout(IHTMLElement* /*pElement*/)
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	return S_OK;
}

void CIPMonitorDlg::OnSelectChangeNic()
{
	index = m_CComboBox.GetCurSel();
	nic.selectNIC(index);
	m_nic_ipaddress = CString(nic.getIPAddress());

	inet_pton(AF_INET, nic.getIPAddress(), &local_ip);	// inet_addr deprecated

	m_nic_subnetmask = CString(nic.getSubnetMask());
	m_nic_macaddress = CString(Common::mac2str(nic.getMacAddress()));
	m_nic_gatewayipaddress = CString(nic.getGatewayIPAddress());
	m_nic_gatewaymac = CString(Common::mac2str(nic.getGatewayMac()));

	UpdateData(FALSE);
}

UINT CIPMonitorDlg::recvPacket(LPVOID lpParam)
{
	CIPMonitorDlg *p = (CIPMonitorDlg *)lpParam;
	pcap_t *adhandle = p->device.getHandle();
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	EthernetFrame ef;
	IP ip, *pip;
	PDU *pdu = &ip;
	ef.SetMACDATA(pdu);

	bool display = false;
	/* Retrieve the packets */
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (!p->started) {
			break;
		}

		if (res == 0)
			/* Timeout elapsed */
			continue;

		ef.Read((BYTE *)pkt_data);
		pip = dynamic_cast<IP *>(pdu);

		if (p->src_checked == false && p->dst_checked == false) {
			display = true;
		}
		else if (p->src_checked == true && p->dst_checked == false) {
			display = p->local_ip == pip->getSourceaddress();
		}
		else if (p->src_checked == false && p->dst_checked == true) {
			display = p->local_ip == pip->getDestinationaddress();
		}
		else if (p->src_checked == true && p->dst_checked == true) {
			display = p->local_ip == pip->getSourceaddress() || p->local_ip == pip->getSourceaddress();
		}

		if (display)
		{
			GlobalLock((HGLOBAL)&p);
			p->srcip = pip->getSourceaddress();
			p->dstip = pip->getDestinationaddress();
			p->proto = pip->getProtocol();
			AfxGetMainWnd()->SendMessage(WM_RECV, (WPARAM)p, 0);
		}
	}

	if (res == -1) {
		return -1;
	}
	return 0;
}

LRESULT CIPMonitorDlg::OnRecv(WPARAM wParam, LPARAM lParam)
{
	CIPMonitorDlg *p = (CIPMonitorDlg *)wParam;

	//char ipsrc[16];
	//char ipdst[16];
	//Common::dword2char(p->srcip, ipsrc);
	//Common::dword2char(p->dstip, ipdst);
	CString ipsrc(Common::dword2char(p->srcip));
	CString ipdst(Common::dword2char(p->dstip));
	CString proto(Common::GetProtocol(p->proto));

	if (!ipdst) {
		return 0;
	}

	int count = m_CListCtrl.GetItemCount();
	for (int i = 0; i < count; i++)
	{
		if (ipsrc == m_CListCtrl.GetItemText(i, 0) &&
			ipdst == m_CListCtrl.GetItemText(i, 1) &&
			proto == m_CListCtrl.GetItemText(i, 2))
		{
			int pcount = atoi((LPCTSTR)m_CListCtrl.GetItemText(i, 3));
			CString pcountstr;
			pcountstr.Format("%d", pcount + 1);
			m_CListCtrl.SetItemText(i, 3, pcountstr);
			UpdateData(FALSE);
			return 0;
		}
	}

	m_CListCtrl.InsertItem(count, ipsrc);
	m_CListCtrl.SetItemText(count, 1, ipdst);
	m_CListCtrl.SetItemText(count, 2, proto);
	m_CListCtrl.SetItemText(count, 3, CString("1"));

	UpdateData(FALSE);
	GlobalUnlock((HGLOBAL)p);
	return 0;
}

void CIPMonitorDlg::OnBnClickedCheckSource()
{
	// TODO: 在此添加控件通知处理程序代码
	src_checked = m_CheckBoxSource.GetCheck();
}

void CIPMonitorDlg::OnBnClickedCheckDestination()
{
	// TODO: 在此添加控件通知处理程序代码
	dst_checked = m_CheckBoxDestination.GetCheck();
}