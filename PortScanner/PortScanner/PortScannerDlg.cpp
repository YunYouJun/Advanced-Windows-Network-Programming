
// PortScannerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PortScanner.h"
#include "PortScannerDlg.h"
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


// CPortScannerDlg 对话框



CPortScannerDlg::CPortScannerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PORTSCANNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_CListCtrl);

	DDX_Text(pDX, IDC_EDIT_STARTINGPORT, m_startingport);
	DDX_Text(pDX, IDC_EDIT_ENDINGPORT, m_endingport);
	DDX_Text(pDX, IDC_EDIT_CURPORT, m_curport);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipaddress);
}

BEGIN_MESSAGE_MAP(CPortScannerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CPortScannerDlg::OnBnClickedButtonScan)
	ON_MESSAGE(WM_RECV, OnRecv)
	ON_MESSAGE(WM_RECV_FAIL, OnRecvFailed)
	ON_MESSAGE(WM_END, OnEnd)
	ON_MESSAGE(WM_UPDATE, OnUpdate)
END_MESSAGE_MAP()


// CPortScannerDlg 消息处理程序

BOOL CPortScannerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
	// remember start end set false
	for (vector<char *>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	{
		index++;
		nic.selectNIC(index);
		inet_pton(AF_INET, nic.getIPAddress(), &m_srcip);

		if (m_srcip) {
			m_srcmac = nic.getMacAddress();
			inet_pton(AF_INET, nic.getSubnetMask(), &m_mask);
			m_gwmac = nic.getGatewayMac();
			GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
			break;
		}
	}

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPortScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPortScannerDlg::OnPaint()
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
HCURSOR CPortScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPortScannerDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == index)
		return;

	if (!started)
	{
		count = 0;
		started = true;
		GetDlgItem(IDC_BUTTON_SCAN)->SetWindowText("&Stop");

		UpdateData(TRUE);
		BYTE field[4];
		m_ipaddress.GetAddress(field[0], field[1], field[2], field[3]);
		CString dstip;
		dstip.Format("%d.%d.%d.%d", field[0], field[1], field[2], field[3]);

		GetDlgItem(IDC_EDIT_STARTINGPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ENDINGPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);

		checked = IsDlgButtonChecked(IDC_CHECK);
		m_CListCtrl.DeleteAllItems();
		m_CListCtrl.InsertItem(count++, "Starting scan");

		m_sport = atoi(m_startingport);
		m_eport = atoi(m_endingport);
		m_dstip = inet_addr(dstip);

		if ((m_dstip&m_mask) == (m_srcip&m_mask))
			m_dstmac = nic.getLanMac(m_dstip);
		else
			m_dstmac = m_gwmac;

		device.obtainDeviceList();
		device.openAdapter(nic.getNICname());
		string filter = "tcp";
		device.setFilter(filter.c_str());

		hThreadrecv = AfxBeginThread(recv, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		hThreadsend = AfxBeginThread(send, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else
	{
		AfxGetMainWnd()->PostMessage(WM_END, 0, 0);
	}
}

UINT CPortScannerDlg::recv(LPVOID lpParam)
{
	CPortScannerDlg *p = (CPortScannerDlg *)lpParam;
	pcap_t *adhandle = p->device.getHandle();
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	EthernetFrame ef;
	TCP tcp, *ptcp;
	PDU *pdu = &tcp;
	ef.SetMACDATA(pdu);

	WORD dp;
	BYTE flags;
	/* Retrieve the packets */
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (!p->started) {
			return 0;
		}

		if (res == 0)
			/* Timeout elapsed */
			continue;

		ef.Read((BYTE *)pkt_data);
		ptcp = dynamic_cast<TCP *>(pdu);
		dp = ptcp->getDestinationport();
		flags = ptcp->getUnusedFlags();
		p->dp = tcp.getDestinationport();
		// 0x12: ack = 1, syn = 1
		if (dp == htons(MY_PORT) && flags == 0x12)
		{
			p->sp = ptcp->getSourceport();
			GlobalLock((HGLOBAL)p);
			AfxGetMainWnd()->SendMessage(WM_RECV, (WPARAM)p, 0);
		}
		else {
			if (p->checked == 1)
			{
				p->dp = tcp.getDestinationport();
				GlobalLock((HGLOBAL)p);
				AfxGetMainWnd()->SendMessage(WM_RECV_FAIL, (WPARAM)p, 0);

			}
			if (ntohs(p->dp) >= p->m_eport)
			{
				AfxGetMainWnd()->SendMessage(WM_END, 0, 0);
				return 0;
			}
		}
	}

	if (res == -1) {
		return -1;
	}
	return 0;
}

LRESULT CPortScannerDlg::OnRecv(WPARAM wParam, LPARAM lParam)
{
	CPortScannerDlg *p = (CPortScannerDlg *)wParam;
	CString ps;
	char dstip[16];
	Common::dword2char(p->m_dstip, dstip);
	ps.Format("%s:%d Connection accepted", dstip, ntohs(p->sp));
	m_CListCtrl.InsertItem(count++, ps);

	UpdateData(FALSE);
	GlobalUnlock((HGLOBAL)p);
	return 0;
}

LRESULT CPortScannerDlg::OnRecvFailed(WPARAM wParam, LPARAM lParam)
{
	CPortScannerDlg *p = (CPortScannerDlg *)wParam;
	CString result;
	char dstip[16];
	Common::dword2char(p->m_dstip, dstip);
	result.Format("%s:%d Connection Failed", dstip, ntohs(p->dp));
	m_CListCtrl.InsertItem(count++, result);

	UpdateData(FALSE);
	GlobalUnlock((HGLOBAL)p);
	return 0;
}

UINT CPortScannerDlg::send(LPVOID lpParam)
{
	CPortScannerDlg *p = (CPortScannerDlg *)lpParam;
	EthernetFrame ef;
	TCP tcp;
	PDU *pdu = &tcp;
	ef.SetMACDATA(pdu);

	BYTE buffer[54] = { 0 }; // 14 + 20 + 20
	for (WORD i = p->m_sport; i <= p->m_eport; ++i)
	{
		if (!p->started) {
			return 0;
		}
		p->m_curport = i;
		AfxGetMainWnd()->SendMessage(WM_UPDATE, 0, 0);

		//ethernetframe header
		ef.SetDestAddress(p->m_dstmac);
		ef.SetSourceAddress(p->m_srcmac);
		ef.SetEtherType(htons(ETHERTYPE_IPV4));

		//ip header
		tcp.setVersionIHL(0x45);
		tcp.setDifferentiatedServices(0);
		tcp.setTotallength(htons(40));
		tcp.setIdentification(htons(1));
		tcp.setUnusedDFMFFragmentoffset(0);
		tcp.setTimetolive(16);
		tcp.setProtocol(6);
		tcp.setHeaderchecksum(0);
		tcp.setSourceaddress(p->m_srcip);
		tcp.setDestinationaddress(p->m_dstip);

		//tcp header
		tcp.setSourceport(htons(MY_PORT));
		tcp.setDestinationport(htons(i));
		tcp.setSequencenumber(htonl(1));
		tcp.setAcknowledgementnumber(0);
		tcp.setTCPheaderlengthUnused(0x50);
		tcp.setUnusedFlags(0x02);
		tcp.setWindowsize(htons(0xffff));
		tcp.setChecksum(0);
		tcp.setUrgentpointer(0);

		//ip checksum recalculate
		ef.Write(buffer);
		WORD ipchecksum = Common::CalculateCheckSum(buffer + 14, 20);
		tcp.setHeaderchecksum(ipchecksum);
		ef.Write(buffer);

		// pdf P20
		//tcp pseudo-header
		BYTE temp[32];
		DWORD srcip = tcp.getSourceaddress();
		DWORD dstip = tcp.getDestinationaddress();
		memcpy(temp, &srcip, 4);
		memcpy(temp + 4, &dstip, 4);
		temp[8] = 0;
		temp[9] = 6;
		WORD TCPsegmentlength = htons(20);
		memcpy(temp + 10, &TCPsegmentlength, 2);
		memcpy(temp + 12, buffer + 34, 20);

		//tcp checksum recalculate
		WORD Checksum = Common::CalculateCheckSum(temp, 32);
		tcp.setChecksum(Checksum);
		ef.Write(buffer);

		//send tcp half open request
		p->device.sendPacket(buffer);
	}
	return 0;
}

LRESULT CPortScannerDlg::OnEnd(WPARAM wParam, LPARAM lParam)
{
	started = false;
	GetDlgItem(IDC_BUTTON_SCAN)->SetWindowText("&Scan");
	m_CListCtrl.InsertItem(count, "Finished scan");
	GetDlgItem(IDC_EDIT_STARTINGPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_ENDINGPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
	return 0;
}

LRESULT CPortScannerDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}
