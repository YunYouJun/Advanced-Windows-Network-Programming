
// TraceRouteDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TraceRoute.h"
#include "TraceRouteDlg.h"
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


// CTraceRouteDlg 对话框



CTraceRouteDlg::CTraceRouteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRACEROUTE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTraceRouteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST, m_CListCtrl);
	DDX_Control(pDX, IDC_IPADDRESS, m_CIPAddressCtrl);
}

BEGIN_MESSAGE_MAP(CTraceRouteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CTraceRouteDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_TRACE, &CTraceRouteDlg::OnBnClickedButtonTrace)
	ON_MESSAGE(WM_RECV, OnRecv)
	ON_MESSAGE(WM_HOST, OnHost)
	ON_BN_CLICKED(IDCANCEL, &CTraceRouteDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTraceRouteDlg 消息处理程序

BOOL CTraceRouteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	vector<char *> al = nic.getAdapterList();
	// don't need select manually
	// if ip is none
	GetDlgItem(IDC_BUTTON_TRACE)->EnableWindow(FALSE);
	for (vector<char *>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	{
		index++;
		nic.selectNIC(index);
		inet_pton(AF_INET, nic.getIPAddress(), &m_srcip);

		if (m_srcip) {
			m_srcmac = nic.getMacAddress();
			inet_pton(AF_INET, nic.getSubnetMask(), &m_mask);
			m_gwmac = nic.getGatewayMac();
			GetDlgItem(IDC_BUTTON_TRACE)->EnableWindow(TRUE);
			break;
		}
	}

	m_CListCtrl.InsertColumn(0, _T("Hop"), LVCFMT_LEFT, 40, -1);
	m_CListCtrl.InsertColumn(1, _T("Time(ms)"), LVCFMT_LEFT, 80, -1);
	m_CListCtrl.InsertColumn(2, _T("IP Address"), LVCFMT_LEFT, 120, -1);
	m_CListCtrl.InsertColumn(3, _T("RouterName"), LVCFMT_LEFT, 300, -1);
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTraceRouteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTraceRouteDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTraceRouteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTraceRouteDlg::OnBnClickedButtonAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

UINT CTraceRouteDlg::getHostName(LPVOID lpParam)
{
	DWORD *ip = (DWORD *)lpParam;
	hostent *h = gethostbyaddr((char *)ip, 4, AF_INET);
	GlobalLock((HGLOBAL)ip);
	GlobalLock((HGLOBAL)h);
	// invoke private varible
	if (h)
		AfxGetMainWnd()->SendMessage(WM_HOST, (WPARAM)h->h_name, (LPARAM)ip);
	return 0;
}

UINT CTraceRouteDlg::sendAndRecv(LPVOID lpParam)
{
	CTraceRouteDlg *p = (CTraceRouteDlg *)lpParam;
	pcap_t *adhandle = p->device.getHandle();
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	EthernetFrame ef;
	ICMP icmp, *picmp;
	PDU *pdu = &icmp;
	ef.SetMACDATA(pdu);

	BYTE buffer[106]; // ?
	WORD id;
	/* Retrieve the packets */
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		// exit loop
		if (!p->started)
			break;

		GetLocalTime(&p->timerecv);
		p->timesub = 1000 * (p->timerecv.wSecond - p->timesend.wSecond) + (p->timerecv.wMilliseconds - p->timesend.wMilliseconds);
		// if wait time more than 3s, judge time out, ++ttl, resend icmp request packet
		if (p->timesub > 3000)
		{
			p->icmpRequest(buffer, ++p->ttl);
			p->device.sendPacket(buffer);
			GlobalUnlock((HGLOBAL)p);
			GetLocalTime(&p->timesend);
		}

		if (res == 0)
			/* Timeout elapsed */
			continue;

		ef.Read((BYTE *)pkt_data);
		picmp = dynamic_cast<ICMP *>(pdu);

		if (picmp->getType() != 0x8)
		{
			switch (picmp->getType())
			{
			// pdf P21
			// dest port unreachable
			case 0x3:
				// subtype
				if (picmp->getCode() != 0x3) {
					break;
				}
			// echo reply(ping)
			case 0x0:
				if (picmp->getId() == htons(ICMP_REQUEST_ID))
				{
					GetLocalTime(&p->timerecv);
					p->timesub = 1000 * (p->timerecv.wSecond - p->timesend.wSecond) + (p->timerecv.wMilliseconds - p->timesend.wMilliseconds);
					p->routerip = picmp->getSourceaddress();
					GlobalUnlock((HGLOBAL)p);
					AfxGetMainWnd()->SendMessage(WM_RECV, 0, 0);
					CWinThread *hThreadhost = AfxBeginThread(getHostName, &p->routerip, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
					return 0;
				}
				break;
			// TTL expired
			case 0xb:
				// icmp error datagram is different from icmp
				// icmp error datagram have error ip header(20) and icmp header(8)
				// 66 = 6+6+2(ethernet header) + 20(IP header) + 1(type) + 1(code) + 2(checksum) + 20(error ip header) + 8(error icmp header)  id:2 seq:2 data:4
				memcpy(&id, pkt_data + 66, 2);
				if (id == htons(ICMP_REQUEST_ID))
				{
					GetLocalTime(&p->timerecv);
					p->timesub = 1000 * (p->timerecv.wSecond - p->timesend.wSecond) + (p->timerecv.wMilliseconds - p->timesend.wMilliseconds);
					p->routerip = picmp->getSourceaddress();
					GlobalUnlock((HGLOBAL)p);
					AfxGetMainWnd()->SendMessage(WM_RECV, 0, 0);
					p->icmpRequest(buffer, ++p->ttl);
					p->device.sendPacket(buffer);
					GetLocalTime(&p->timesend);

					CWinThread *hThreadhost = AfxBeginThread(getHostName, &p->routerip, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				}
				break;
			default:
				break;
			}
		}
	}

	if (res == -1) {
		return -1;
	}
	return 0;
}

LRESULT CTraceRouteDlg::OnRecv(WPARAM wParam, LPARAM lParam)
{
	CString ttl_str;
	ttl_str.Format("%d", ttl);
	CString time_str;
	time_str.Format("%d", timesub);

	char routerip_str[16];
	Common::dword2char(routerip, routerip_str);
	int count = m_CListCtrl.GetItemCount();
	m_CListCtrl.InsertItem(count, ttl_str);
	m_CListCtrl.SetItemText(count, 1, time_str);
	m_CListCtrl.SetItemText(count, 2, routerip_str);

	UpdateData(FALSE);
	GlobalUnlock((HGLOBAL)this);
	return 0;
}

LRESULT CTraceRouteDlg::OnHost(WPARAM wParam, LPARAM lParam)
{
	char *h = (char *)wParam;
	DWORD * ip = (DWORD *)lParam;
	CString hostname(h);

	char cur_ipstr[16];
	Common::dword2char(*ip, cur_ipstr);

	// set host name
	int n = m_CListCtrl.GetItemCount();
	for (int i = 0; i < n; ++i)
	{
		CString ip_str = m_CListCtrl.GetItemText(i, 2);
		if (cur_ipstr == ip_str)
			m_CListCtrl.SetItemText(i, 3, hostname);
	}

	GlobalUnlock((HGLOBAL)h);
	GlobalUnlock((HGLOBAL)ip);
	UpdateData(FALSE);

	return 0;
}

void CTraceRouteDlg::OnBnClickedButtonTrace()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == index)
		return;

	if (!started)
	{
		started = true;
		GetDlgItem(IDC_BUTTON_TRACE)->SetWindowText("&Stop");
		m_CListCtrl.DeleteAllItems();

		BYTE field[4];
		m_CIPAddressCtrl.GetAddress(field[0], field[1], field[2], field[3]);
		CString dstip;
		dstip.Format("%d.%d.%d.%d", field[0], field[1], field[2], field[3]);

		inet_pton(AF_INET, dstip, &m_dstip);

		// send in lan or out
		if ((m_dstip&m_mask) == (m_srcip&m_mask))
			m_dstmac = nic.getLanMac(m_dstip);	// send to dest mac directly
		else
			m_dstmac = m_gwmac;

		device.obtainDeviceList();
		device.openAdapter(nic.getNICname());
		string filter = "icmp";
		device.setFilter(filter.c_str());

		hThread = AfxBeginThread(sendAndRecv, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		BYTE buffer[106];
		ttl = 1;
		icmpRequest(buffer, ttl);
		device.sendPacket(buffer);
		GetLocalTime(&timesend);
	}
	else
	{
		started = false;
		GetDlgItem(IDC_BUTTON_TRACE)->SetWindowText("&Trace");
	}
}

void CTraceRouteDlg::icmpRequest(BYTE *buffer, BYTE ttl)
{
	EthernetFrame ef;
	ICMP icmp;
	PDU *pdu = &icmp;
	ef.SetMACDATA(pdu);

	//ethernetframe header
	ef.SetDestAddress(m_dstmac);
	ef.SetSourceAddress(m_srcmac);
	ef.SetEtherType(htons(ETHERTYPE_IPV4));

	//ip header
	icmp.setVersionIHL(0x45);
	icmp.setDifferentiatedServices(0);
	icmp.setTotallength(htons(92));
	icmp.setIdentification(htons(ICMP_REQUEST_ID));
	icmp.setUnusedDFMFFragmentoffset(0);
	icmp.setTimetolive(ttl);
	icmp.setProtocol(1); // icmp
	icmp.setHeaderchecksum(0);
	icmp.setSourceaddress(m_srcip);
	icmp.setDestinationaddress(m_dstip);

	//icmp header
	// icmp messages type8 code0 is echo request(ping)
	icmp.setType(8);
	icmp.setCode(0);
	icmp.setChecksum(0);
	icmp.setId(htons(ICMP_REQUEST_ID));
	icmp.setSeq(htons(ICMP_REQUEST_ID));
	BYTE data[64] = { 0 };
	icmp.setData(data);

	//ip checksum recalculate because of ttl change
	ef.Write(buffer);
	WORD ipchecksum = Common::CalculateCheckSum(buffer + 14, 20);
	icmp.setHeaderchecksum(ipchecksum);

	//icmp checksum recalculate
	WORD icmpchecksum = Common::CalculateCheckSum(buffer + 34, 8);
	icmp.setChecksum(icmpchecksum);

	ef.Write(buffer);
}


void CTraceRouteDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
