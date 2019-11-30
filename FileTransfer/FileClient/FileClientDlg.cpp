
// FileClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FileClient.h"
#include "FileClientDlg.h"
#include "afxdialogex.h"
#include <fstream>

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


// CFileClientDlg 对话框



CFileClientDlg::CFileClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IPADDRESS, m_ip_ctrl);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_filepath);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_savepath);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_content);
}

BEGIN_MESSAGE_MAP(CFileClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CANCEL, &CFileClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_REQUEST_BTN, &CFileClientDlg::OnBnClickedRequestBtn)
END_MESSAGE_MAP()


// CFileClientDlg 消息处理程序

BOOL CFileClientDlg::OnInitDialog()
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
	fileClient.Initialize();
	m_started = false;
	m_filepath = "";
	m_savepath = "";
	m_content = "";

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileClientDlg::OnPaint()
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
HCURSOR CFileClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileClientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


unsigned __stdcall downloadFile(LPVOID pArguments) {
	downloadInfo *curFile = (downloadInfo*)pArguments;
	downloadInfo cf = *curFile;

	string ip = inet_ntoa(cf.client.sin_addr);
	int client_tcp_port = ntohs(cf.client.sin_port);
	int client_udp_port = client_tcp_port + 1;

	// udp socket
	UdpSocket udpSock;
	udpSock.Socket();
	udpSock.Bind(ip, client_udp_port);

	// client get port
	cf.client.sin_port += 1;

	// init udpServer info
	struct sockaddr_in udpServer;
	// Set up the sockaddr structure
	udpServer.sin_family = AF_INET;
	udpServer.sin_addr.s_addr = inet_addr(ip.c_str());
	udpServer.sin_port = htons(UDP_SRV_PORT);

	int pos = cf.filename.find_last_of('/');
	string filename(cf.filename.substr(pos + 1));
	fstream fs;
	// C++ open 打开文件（含打开模式一览表）http://c.biancheng.net/view/294.html
	fs.open("./Download/" + filename, ios::app | ios::binary);
	int fileRecv = 0;

	char buffer[1500];
	while (fileRecv != -1) {
		fileRecv = udpSock.Recv(buffer, &udpServer);
		fs.write(buffer, fileRecv);
	}

	fs.close();
	//m_content = m_content + "\r\n" + "Download " + cf.filename + " successfully.";
	//UpdateData(FALSE);

	udpSock.Close();
	return 0;
}

void CFileClientDlg::OnBnClickedRequestBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_started) {
		m_started = true;

		GetDlgItem(ID_REQUEST_BTN)->SetWindowText("&Stop");

		// get data from interface
		UpdateData(true);

		BYTE field[4];
		m_ip_ctrl.GetAddress(field[0], field[1], field[2], field[3]);
		m_ip.Format("%d.%d.%d.%d", field[0], field[1], field[2], field[3]);

		// set disable
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(FALSE);

		fileClient(m_ip, m_port);
		fileClient.Open();
		fileClient.setSendRecvBuffer(MAX_BUF_SIZE);
		fileClient.Connect();
		fileClient.Send(m_filepath, strlen(m_filepath));

		
		if (m_savepath == "") {
			m_savepath = m_filepath;
		}
		
		// get local tcp socket port
		struct sockaddr_in c;
		socklen_t cLen = sizeof(c);
		getsockname(fileClient.m_socket, (struct sockaddr*) &c, &cLen);

		int fileRecv = 0;
		while (fileRecv != -1) {
			fileRecv = fileClient.Recv(buffer, sizeof(buffer));
			
			string cur_filename = buffer;
			struct downloadInfo curFile = { c, cur_filename };

			HANDLE hThread;
			unsigned threadID;
			hThread = (HANDLE)_beginthreadex(NULL, 0, &downloadFile, &curFile, 0, &threadID);

			m_content = m_content + "Begin download " + cur_filename.c_str() + " ..." + "\r\n";
			UpdateData(FALSE);
		}
	}
	//else {
		m_started = false;
		GetDlgItem(ID_REQUEST_BTN)->SetWindowText("&Request");
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
	//}

		fileClient.Close();
}
