
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


BOOL multi_thread = false;

struct downloadParam {
    CBlockingSocket ConnectSocket;
    char *filename;
    char *savepath;
    CFileClientDlg *dlg;
};

unsigned __stdcall downloadFile(LPVOID pArguments) {
    downloadParam *dp = (downloadParam *)pArguments;
    downloadParam da = *dp;

    CBlockingSocket ConnectSocket = da.ConnectSocket;
    char *filename;
    char *savepath;
    filename = da.filename;
    savepath = da.savepath;

    struct sockaddr_in c;
    socklen_t cLen = sizeof(c);
    getsockname(ConnectSocket.m_socket, (struct sockaddr*) &c, &cLen);
    struct sockaddr_in s;
    socklen_t sLen = sizeof(s);
    getpeername(ConnectSocket.m_socket, (struct sockaddr*) &s, &sLen);

    // recv by udp
    UdpSocket udpSock;
    udpSock.Open();
    int client_tcp_port = htons(c.sin_port);
    int client_udp_port = client_tcp_port + 1;
    int server_tcp_port = htons(s.sin_port);
    int server_udp_port = server_tcp_port - 1;
    udpSock.Bind(NULL, client_udp_port);

    // set udp_server_addr
    sockaddr_in udp_server_addr;
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(server_udp_port);
    udp_server_addr.sin_addr.S_un.S_addr = s.sin_addr.S_un.S_addr;



    // send file name & get file size
    Message msg;
    char buffer[MAX_COMMAND_SIZE] = { 0 };
    msg.setType(COMMAND_DOWNLOAD);
    msg.setSize(strlen(filename));
    msg.setData(filename);
    msg.write(buffer);
    ConnectSocket.Send(buffer, MSG_HEADER_LENGTH + strlen(filename));

    Message size_msg;
    ConnectSocket.Recv(buffer, MSG_HEADER_LENGTH);
    size_msg.read(buffer);
    int filelen = -1;
    if (size_msg.getType() == FILE_DATA_SIZE) {
        filelen = size_msg.getSize();
    }
    if (filelen == -1) {
        da.dlg->m_content = "  File " + CString(filename) + " is not existed.";
        da.dlg->UpdateData(FALSE);
    }
    else {
        fstream fs;
        fs.open(savepath, ios::out | ios::binary);
        fs.close();
        fs.open(savepath, ios::app | ios::binary);

        int percent = 0;
        int block = filelen / MAX_DATA_SIZE + 1;
        char recvBuf[MAX_BUF_SIZE] = { 0 };
        unsigned long lastSeq = 0;
        int filerecv = 0;
        da.dlg->m_content = "  Receiving file: " + CString(filename) + CString(filename) + " B）";
        //da.dlg->UpdateData(FALSE);
        while (1)
        {
            int sLen = sizeof(sockaddr);
            filerecv = recvfrom(udpSock.m_socket, recvBuf, MAX_BUF_SIZE, 0, (sockaddr*)&udp_server_addr, &sLen);
            if (filerecv == -1) {
                break;
            }
            // check
            Message file_msg;
            file_msg.read(recvBuf);
            if (file_msg.getChecksum() != Common::CalculateCheckSum(file_msg.getData(), file_msg.getSize())) {
                da.dlg->m_content = "    Checksum is wrong, drop it.";
                //da.dlg->UpdateData(FALSE);
                continue;
            }

            // send ack
            Message data_ack_msg;
            data_ack_msg.setType(FILE_DATA_ACK);
            char bufAck[MSG_HEADER_LENGTH] = { 0 };
            data_ack_msg.write(bufAck);

            // get it before drop it
            if (lastSeq == file_msg.getSeq()) {
                //cout << "  Got it before, drop it." << endl;
                //cout << "    Send ack back to the server." << endl;
                sendto(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_server_addr, sizeof(sockaddr));
                continue;
            }
            fs.write(file_msg.getData(), file_msg.getSize());
            lastSeq = file_msg.getSeq();
            //cout << "    Send ack back to the server." << endl;
            sendto(udpSock.m_socket, bufAck, MSG_HEADER_LENGTH, 0, (sockaddr*)&udp_server_addr, sizeof(sockaddr));
            //cout << "    UDP: Received " << filerecv << " B, data: " << file_msg.getSize() << " B." << endl;


            if (!multi_thread) {
                // set console color
                HANDLE  hStdOutHandle;
                WORD    wOldColorAttrs;
                CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                hStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                GetConsoleScreenBufferInfo(hStdOutHandle, &csbiInfo);
                wOldColorAttrs = csbiInfo.wAttributes;

                // print progress bar
                percent = 100 * file_msg.getSeq() / block;
                if (percent == 100) {
                    SetConsoleTextAttribute(hStdOutHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    Common::progress_bar(percent);
                    SetConsoleTextAttribute(hStdOutHandle, wOldColorAttrs);
                }
                else {
                    Common::progress_bar(percent);
                }
            }

            // over
            if (file_msg.getType() == FILE_DATA_END) {
                da.dlg->m_content = "  Download file " + CString(filename) + " completed.";
                //da.dlg->UpdateData(FALSE);
                break;
            }
        }
        fs.close();
    }
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

        CBlockingSocket::Initialize();
        // init socket
        USES_CONVERSION;
        char *ip = T2A(m_ip.GetBuffer(0));
        m_ip.ReleaseBuffer();
        CBlockingSocket ConnectSocket(ip, m_port);
        ConnectSocket.Open();
        ConnectSocket.setSendRecvBuffer(MAX_BUF_SIZE);
        if (!ConnectSocket.Connect()) {
            cout << "Connect failed" << endl;
        }
		
		if (m_savepath == "") {
			m_savepath = "download_" + m_filepath;
		}

        char *filepath = T2A(m_filepath.GetBuffer(0));
        m_filepath.ReleaseBuffer();
        char *savepath = T2A(m_savepath.GetBuffer(0));
        m_savepath.ReleaseBuffer();
        downloadParam dp = {
            ConnectSocket,
            filepath,
            savepath,
            this
        };
        HANDLE hThread;
        unsigned threadID;
        hThread = (HANDLE)_beginthreadex(NULL, 0, &downloadFile, &dp, 0, &threadID);
	}
	//else {
		m_started = false;
		GetDlgItem(ID_REQUEST_BTN)->SetWindowText("&Request");
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
	//}
}
