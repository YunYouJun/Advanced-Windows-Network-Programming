
// LANScannerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "LANScanner.h"
#include "LANScannerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_COMBO_NIC                   1000

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


// CLANScannerDlg 对话框
// _T 转换为宽字节

BEGIN_DHTML_EVENT_MAP(CLANScannerDlg)
	//DHTML_EVENT_ONCHANGE(_T("nic-select"), OnSelectChangeNic)
	DHTML_EVENT_ONCLICK(_T("ButtonScan"), OnButtonScan)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("ButtonAbout"), OnButtonAbout)
END_DHTML_EVENT_MAP()


CLANScannerDlg::CLANScannerDlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_LANSCANNER_DIALOG, IDR_HTML_LANSCANNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// https://docs.microsoft.com/en-us/cpp/mfc/reference/cdhtmldialog-class?redirectedfrom=MSDN&view=vs-2019
void CLANScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NIC, m_CComboBox);

	DDX_DHtml_ElementInnerText(pDX, _T("ButtonScan"), btnScanText);
	DDX_DHtml_ElementValue(pDX, _T("ipAddress"), m_nic_ipaddress);
	DDX_DHtml_ElementValue(pDX, _T("subnetMask"), m_nic_subnetmask);
	DDX_DHtml_ElementValue(pDX, _T("macAddress"), m_nic_macaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayIPAddress"), m_nic_gatewayipaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayMac"), m_nic_gatewaymac);

	DDX_DHtml_ElementInnerHtml(pDX, _T("host-list"), m_host_list);
}

// Map the message to the handling function
BEGIN_MESSAGE_MAP(CLANScannerDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_CBN_SELCHANGE(IDC_COMBO_NIC, &CLANScannerDlg::OnSelectChangeNic)

	ON_MESSAGE(WM_COMPLETE, OnComplete)//ON_MESSAGE是自定义消息WM_COMPLETE
END_MESSAGE_MAP()


// CLANScannerDlg 消息处理程序

BOOL CLANScannerDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	btnScanText = "Scan";
	m_host_list_table_content = "";
	m_host_list = m_host_list_table_head + m_host_list_table_content + m_host_list_table_tail;

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

	for (vector<char*>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	{
		m_CComboBox.InsertString(row, *iter);
		++row;
	}
	m_nic_select += "</select>";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLANScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLANScannerDlg::OnPaint()
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
HCURSOR CLANScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CLANScannerDlg::OnButtonScan(IHTMLElement* /*pElement*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == index)
		return -1;

	if (!started)
	{
		started = true;
		m_host_list_table_content = "";
		m_host_list = m_host_list_table_head + m_host_list_table_content + m_host_list_table_tail;
		btnScanText = "Stop";

		GetDlgItem(IDC_COMBO_NIC)->EnableWindow(FALSE);

		device.obtainDeviceList();
		device.openAdapter(nic.getNICname());
		string filter = "ether protoarp";
		device.setFilter(filter.c_str());

		memset(dstmac, 0xFF, 6);//broadcast
		memcpy(srcmac, nic.getMacAddress(), 6);
		inet_pton(AF_INET, nic.getIPAddress(), &localip);
		inet_pton(AF_INET, nic.getSubnetMask(), &netmask);
		ipbeg = ntohl(localip & netmask);	// ntohl() 将一个无符号长整形数从网络字节顺序转换为主机字节顺序
		ipend = ipbeg + ~ntohl(netmask);

		hThreadsend = AfxBeginThread(sendPacket, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		hThreadrecv = AfxBeginThread(recvPacket, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else
	{
		started = false;
		btnScanText = "Scan";
		GetDlgItem(IDC_COMBO_NIC)->EnableWindow(TRUE);

		// Multithreading: Terminating Threads in MFC -MSDN 
		// https://docs.microsoft.com/en-us/cpp/parallel/multithreading-terminating-threads?view=vs-2019
		// i use loop detect started to exit thread

		// Seems to be invalid
		//TerminateThread(hThreadsend, NULL);
		//TerminateThread(hThreadrecv, NULL);

		// retrieving the exit code of a thread
		// but exit directly
		//DWORD exitcode;
		//GetExitCodeThread(hThreadsend, &exitcode);
		//ExitThread(exitcode);
		//GetExitCodeThread(hThreadrecv, &exitcode);
		//ExitThread(exitcode);
	}
	UpdateData(FALSE);
	return S_OK;	// 操作成功
}

HRESULT CLANScannerDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

HRESULT CLANScannerDlg::OnButtonAbout(IHTMLElement* /*pElement*/)
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	return S_OK;
}

void CLANScannerDlg::OnSelectChangeNic()
{
	index = m_CComboBox.GetCurSel();
	nic.selectNIC(index);
	m_nic_ipaddress = CString(nic.getIPAddress());
	m_nic_subnetmask = CString(nic.getSubnetMask());
	m_nic_macaddress = CString(Common::mac2str(nic.getMacAddress()));
	m_nic_gatewayipaddress = CString(nic.getGatewayIPAddress());
	m_nic_gatewaymac = CString(Common::mac2str(nic.getGatewayMac()));

	m_host_list_table_content = "";
	m_host_list = m_host_list_table_head + m_host_list_table_content + m_host_list_table_tail;
	UpdateData(FALSE);
}

UINT CLANScannerDlg::sendPacket(LPVOID lpParam)
{
	CLANScannerDlg* p = (CLANScannerDlg*)lpParam;
	EthernetFrame ef;
	ARP arp;
	PDU* pdu = &arp;
	ef.SetMACDATA(pdu);

	//EthernetFrame Header
	ef.SetDestAddress(p->dstmac);
	ef.SetSourceAddress(p->srcmac);
	ef.SetEtherType(htons(ETHERTYPE_ARP));

	ARP Header;
	arp.setHardwareType(htons(HRD_ETHERNET));
	arp.setProtocolType(htons(PRO_IP));
	arp.setHardwareAddrLen(6);
	arp.setProtocolAddrLen(4);
	arp.setOpCode(htons(OP_REQUEST));
	arp.setSenderHardwareAddr(p->srcmac);
	arp.setSenderProtocolAddr(p->localip);
	arp.setTargetHardwareAddr(p->dstmac);

	BYTE buffer[60] = { 0 };
	GetLocalTime(&p->timesend);

	for (DWORD i = p->ipbeg; i <= p->ipend; ++i)
	{
		if (!p->started) {
			return 0;
		}
		arp.setTargetProtocolAddr(htonl(i));
		ef.Write(buffer);
		p->device.sendPacket(buffer);
	}
	return 0;
}

UINT CLANScannerDlg::recvPacket(LPVOID lpParam)
{
	CLANScannerDlg* p = (CLANScannerDlg*)lpParam;
	pcap_t* adhandle = p->device.getHandle();
	int res;
	struct pcap_pkthdr* header;
	const u_char* pkt_data;
	EthernetFrame ef;
	ARP arp, *parp;
	PDU* pdu = &arp;
	ef.SetMACDATA(pdu);

	/* Retrieve the packets */
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
			/* Timeout elapsed */
			continue;

		if (!p->started) {
			return 0;
		}

		ef.Read((BYTE*)pkt_data);
		parp = dynamic_cast<ARP*>(pdu);
		if (parp->getOpCode() == htons(OP_REPLY) && parp->getTargetProtocolAddr() == p->localip)
		{
			GetLocalTime(&p->timerecv);
			p->diff = 1000 * (p->timerecv.wSecond - p->timesend.wSecond) + (p->timerecv.wMilliseconds - p->timesend.wMilliseconds);
			memcpy(p->hostmac, parp->getSenderHardwareAddr(), 6);
			p->hostip = parp->getSenderProtocolAddr();
			//GlobalLock((HGLOBAL)p);
			//AfxGetMainWnd()->SendMessage(WM_COMPLETE, (WPARAM)p, 0);

			// update list
			CString content = "";
			char hostip[16];
			Common::dword2char(p->hostip, hostip);

			// get arp time
			// static_cast是一个c++运算符，功能是把一个表达式转换为某种类型类型，但没有运行时类型检查来保证转换的安全性。
			// https://docs.microsoft.com/zh-cn/visualstudio/code-quality/c6328?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev16.query%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(C6328)%26rd%3Dtrue%26f%3D255%26MSPPError%3D-2147217396&view=vs-2019
			CString time;
			time.Format("%d ms", (int)p->diff);

			// get host name
			HOSTENT* h = gethostbyaddr((char*)hostip, 4, AF_INET);
			CString hostname = "";
			if (h) {
				hostname = h->h_name;
			}

			content = "<tr><td>" + CString(hostip) + "</td><td>" + CString(Common::mac2str(p->hostmac)) + "</td><td>" + hostname + "</td><td>" + time + "</td></tr>";
			p->m_host_list_table_content += content;
			p->m_host_list = p->m_host_list_table_head + p->m_host_list_table_content + p->m_host_list_table_tail;

			// uodatedata
			AfxGetMainWnd()->SendMessage(WM_COMPLETE, (WPARAM)p, 0);
		}
	}

	if (res == -1) {
		return -1;
	}
	return 0;
}

LRESULT CLANScannerDlg::OnComplete(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}