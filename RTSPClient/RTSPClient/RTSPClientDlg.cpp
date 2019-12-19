
// RTSPClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "RTSPClient.h"
#include "RTSPClientDlg.h"
#include "afxdialogex.h"

#include <string>

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


// CRTSPUrlDlg
class CRTSPUrlDlg : public CDialogEx
{
public:
    CRTSPUrlDlg();
    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RTSP_URL };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();

// 实现
protected:
    DECLARE_MESSAGE_MAP()
protected:
    afx_msg LRESULT OnGetUrl(WPARAM wParam, LPARAM lParam);
public:
    CEdit m_rtsp_url_ctrl;
    CString m_rtsp_url;
    CString getRtspUrl();
    afx_msg void OnBnClickedOk();
};

CRTSPUrlDlg::CRTSPUrlDlg() : CDialogEx(IDD_RTSP_URL)
{
}

void CRTSPUrlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INPUT_URL, m_rtsp_url_ctrl);
}

BEGIN_MESSAGE_MAP(CRTSPUrlDlg, CDialogEx)
    ON_MESSAGE(WM_GET_URL, &CRTSPUrlDlg::OnGetUrl)
    ON_BN_CLICKED(IDOK, &CRTSPUrlDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CString CRTSPUrlDlg::getRtspUrl() {
    CString temp;
    m_rtsp_url_ctrl.GetWindowText(temp);
    return temp;
}

BOOL CRTSPUrlDlg::OnInitDialog() {
    CDialog::OnInitDialog();
    m_rtsp_url_ctrl.SetWindowTextW(L"rtsp://rtsp.yunyoujun.cn:554/test.mp3");
    return TRUE;
}


// CRTSPClientDlg 对话框
CRTSPClientDlg::CRTSPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RTSPCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
}

void CRTSPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_PROGRESS_SLIDER, m_progressBar);
    DDX_Control(pDX, IDC_VOLUME_SLIDER, m_volBar);
    DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
    DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
    DDX_Control(pDX, IDC_START_TIME, m_startTime);
    DDX_Control(pDX, IDC_END_TIME, m_endTime);
    DDX_Control(pDX, IDC_TXT_VOLUME, m_volText);
    DDX_Control(pDX, IDC_AUDIO_NAME, m_audioName);
    //DDX_Control(pDX, IDC_INPUT_URL, m_url);
}

BEGIN_MESSAGE_MAP(CRTSPClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_PLAY, &CRTSPClientDlg::OnBnClickedBtnPlay)
    ON_BN_CLICKED(IDC_BTN_STOP, &CRTSPClientDlg::OnBnClickedBtnStop)
    ON_WM_HSCROLL()
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_COMMAND(ID_OPEN_RTSP, &CRTSPClientDlg::OnOpenRtsp)
    ON_COMMAND(ID_OPEN_LOCAL, &CRTSPClientDlg::OnOpenLocal)
END_MESSAGE_MAP()

// CRTSPClientDlg 消息处理程序

BOOL CRTSPClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO:  在此添加额外的初始化代码
    g_pause = FALSE;
    g_play = FALSE;
    g_init = FALSE;
    g_local = FALSE;
    g_stop = FALSE;
    g_forward = FALSE;

    g_rtsp = false;

    m_curTime = 0;
    m_progressBar.SetRange(0, 100);
    m_volBar.SetRange(0, 100);
    m_volBar.SetPos(100);
    m_volText.SetWindowText(_T("100"));

    //m_url = "rtsp://computing.cuc.edu.cn:8554/Angel.mp3";
    //m_url = "rtsp://rtsp.yunyoujun.cn:554/test.mp3";
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRTSPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CRTSPClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}


HBRUSH CRTSPClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    CRect ctrlRect;
    pWnd->GetClientRect(&ctrlRect);
    //switch (pWnd->GetDlgCtrlID())
    //{
    //case IDC_PROGRESS_SLIDER:
    //case IDC_VOLUME_SLIDER:
        CBrush mybrush(RGB(255, 255, 255));
        pDC->FillRect(ctrlRect, &mybrush);
        mybrush.DeleteObject();
        pDC->SetBkMode(TRANSPARENT);
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        //break;
    //}
    return hBrush;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRTSPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRTSPClientDlg::OnBnClickedBtnPlay()
{
    if (!g_play)
    {
        m_fromTime = 0;
        if (g_local)
        {
            if (!g_init)
            {
                PLAYThread = AfxBeginThread(playThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
                g_init = TRUE;
            }
            else if (g_pause)
            {
                m_player.Play();
                g_pause = FALSE;
                g_play = TRUE;
                m_btnPlay.SetWindowText(_T("PAUSE"));
            }
        }

        if (g_rtsp)
        {
            if (!g_init)
            {
                RTSPThread = AfxBeginThread(rtspThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
                g_init = TRUE;
            }
            else if (g_pause)
            {
                m_player.Play();
                m_rtsp_request.RequestPlay(PLAY_AFTER_PAUSE);
                m_cs.Send(m_cs.RTSPSocket, m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
                m_rtsp_reply.response = m_cs.Recv(m_cs.RTSPSocket);
                if (!m_rtsp_reply.CheckRtspResponse(PLAY))
                {
                    AfxMessageBox(L"PLAY failed with error.");
                }
                g_play = TRUE;
                g_pause = FALSE;
                m_btnPlay.SetWindowText(_T("PAUSE"));
            }
        }
    }
    else {
        if (g_local)
        {
            m_player.Pause();
            g_pause = TRUE;
            g_play = FALSE;
            m_btnPlay.SetWindowText(_T("PLAY"));
        }
        if (g_rtsp)
        {
            m_player.Pause();
            m_rtsp_request.RequestPause();
            m_cs.Send(m_cs.RTSPSocket, m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
            m_rtsp_reply.response = m_cs.Recv(m_cs.RTSPSocket);
            if (!m_rtsp_reply.CheckRtspResponse(PAUSE))
            {
                AfxMessageBox(L"PAUSE failed with error.");
            }
            else {
                g_pause = TRUE;
                g_play = FALSE;
                m_btnPlay.SetWindowText(_T("PLAY"));
            }
        }
    }
}

void CRTSPClientDlg::OnBnClickedBtnStop()
{
    SOCKET rtspSocket = m_cs.RTSPSocket;
    SOCKET rtpSocket = m_cs.RTPSocket;
    CString volumeStr;
    m_curTime = 0;
    m_curPos = 0;
    m_player.Stop();
    if (g_local)
    {
        KillTimer(SETLOCALTIME);
        m_progressBar.SetPos(0);
        m_startTime.SetWindowText(L"00:00");
    }
    if (g_rtsp)
    {
        KillTimer(SETRTSPTIME);
        m_progressBar.SetPos(0);
        m_startTime.SetWindowText(L"00:00");
        //TEARDOWN
        m_rtsp_request.RequestTeardown();
        m_cs.Send(rtspSocket, m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
        m_rtsp_reply.response = m_cs.Recv(rtspSocket);
        if (!m_rtsp_reply.CheckRtspResponse(TEARDOWN))
        {
            AfxMessageBox(L"TEARDOWN failed with error.");
        }
        CloseHandle(hFile);
        m_cs.Close(rtspSocket);
        m_cs.Close(rtpSocket);
    }
    g_stop = TRUE;
    g_init = FALSE;
    g_pause = FALSE;
    g_play = FALSE;
    g_local = FALSE;
    g_rtsp = FALSE;
    g_forward = FALSE;

    m_btnPlay.SetWindowText(_T("PLAY"));
}

UINT CRTSPClientDlg::rtspThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;
    p->m_rtsp_request.url = p->c_url;

    if (!p->m_cs.Open(p->m_cs.RTSPSocket, p->cur_url.getIp().c_str(), p->cur_url.getPort().c_str(), TCP))
    {
        AfxMessageBox(L"Socket failed with error.");
    }

    if (!p->m_cs.Connect(p->m_cs.RTSPSocket, p->m_cs.RTSPServerInfo))
    {
        AfxMessageBox(L"Connect failed with error.");
    }

    //OPTIONS
    p->m_rtsp_request.RequestOptions();
    p->m_cs.Send(p->m_cs.RTSPSocket, p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    p->m_rtsp_reply.response = p->m_cs.Recv(p->m_cs.RTSPSocket);
    if (!p->m_rtsp_reply.CheckRtspResponse(OPTIONS) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"OPTIONS failed with error.");
    }

    //DESCRIBE
    p->m_rtsp_request.RequestDescribe();
    p->m_cs.Send(p->m_cs.RTSPSocket, p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    p->m_rtsp_reply.response = p->m_cs.Recv(p->m_cs.RTSPSocket);
    if (!p->m_rtsp_reply.CheckRtspResponse(DESCRIBE) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"DESCRIBE failed with error.");
    }
    p->m_rtsp_reply.GetAudioLen();
    p->c_fileLength = atoi(p->m_rtsp_reply.fileLength.c_str());

    //SETUP
    p->m_rtsp_request.RequestSetup();
    p->m_cs.Send(p->m_cs.RTSPSocket, p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    p->m_rtsp_reply.response = p->m_cs.Recv(p->m_cs.RTSPSocket);
    if (!p->m_rtsp_reply.CheckRtspResponse(SETUP) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"SETUP failed with error.");
    }

    //p->m_rtsp_reply.GetResponseRtcpPort();
    //p->m_rtsp_reply.GetResponseRtpPort();
    //p->c_clientRtpPort = p->m_rtsp_reply.ClientRtpPort;
    //p->c_serverRtpPort = p->m_rtsp_reply.ServerRtpPort;

    //PLAY
    p->m_rtsp_request.Session = p->m_rtsp_reply.Session;
    p->m_rtsp_request.RequestPlay(p->m_fromTime);
    p->m_cs.Send(p->m_cs.RTSPSocket, p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    Sleep(1000);
    p->m_rtsp_reply.response = p->m_cs.Recv(p->m_cs.RTSPSocket);
    if (!p->m_rtsp_reply.CheckRtspResponse(PLAY) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"PLAY failed with error.");
    }
    else {
        AfxBeginThread(rtpThread, p, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
    }

    //PARAMETER
    while (!p->g_stop)
    {
        p->m_rtsp_request.RequestGetparameter();
        p->m_cs.Send(p->m_cs.RTSPSocket, p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
        p->m_rtsp_reply.response = p->m_cs.Recv(p->m_cs.RTSPSocket);
        if (!p->m_rtsp_reply.CheckRtspResponse(GET_PARAMETER) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
        {
            AfxMessageBox(L"GET PARAMETER failed with error.");
        }
        // because timeout: 65s
        Sleep(60000);
    }
    return 0;
}

UINT CRTSPClientDlg::rtpThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;
    DWORD dwBytesWritten = 0;
    DWORD dwBytesCount = 0;
    int ClientBytesRecv = 0;

    p->hFile = CreateFile(
        L"./_cache.mp3",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        //FILE_FLAG_DELETE_ON_CLOSE,
        NULL);

    //string cacheName = "_cache.mp3";
    //p->c_filePath = cacheName;
    //fstream fs;
    //bool isFirstBuffer = true;
    //fs.open(cacheName, ios_base::binary | ios_base::out | ios_base::trunc);

    // ios_base::out: 打开文件进行写操作，即写入数据到文件。
    // ios_base::trunc: 打开文件，若文件已存在那么，清空文件内容.
    

    // WAIT_OBJECT_0：表示你等待的对象（比如线程、互斥体）已的正常执行完成或完成释放。

    if (p->hFile == INVALID_HANDLE_VALUE)
    {
        CloseHandle(p->hFile);
    }
    if (!p->m_cs.Open(p->m_cs.RTPSocket, p->c_serverIp.c_str(), p->c_serverRtpPort.c_str(), UDP))
    {
        AfxMessageBox(L"Socket failed with error.");
    }

    if (!p->m_cs.Bind(p->m_cs.RTPSocket, p->c_clientRtpPort.c_str()))
    {
        AfxMessageBox(L"Bind failed with error.");
    }
    p->c_filePath = ",/_cache.mp3";
    AfxBeginThread(playThread, p, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

    while (TRUE)
    {
        ZeroMemory(p->recvBuffer, sizeof(p->recvBuffer));
        //ClientBytesRecv = m_cs.RecvFrom(m_cs.RTPSocket, m_cs.RTPServerInfo, p->recvBuffer, sizeof(p->recvBuffer));
        //ClientBytesRecv = m_cs.Recv(m_cs.RTPSocket);
        ClientBytesRecv = p->m_cs.RecvRTP(p->m_cs.RTPSocket, p->recvBuffer);
            //if (!isFirstBuffer) {
            //    fs.write(p->recvBuffer + 16, ClientBytesRecv - 16);
            //}
            //else {
            //    fs.write(p->recvBuffer + 16 + 4 + 413, ClientBytesRecv - 16 - 4 - 413);
            //    isFirstBuffer = false;

            //    // play
            //    AfxBeginThread(playThread, p, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
            //}

        //ClientBytesRecv = m_cs.RecvFrom(m_cs.RTPSocket, m_cs.RTPServerInfo, p->recvBuffer, sizeof(p->recvBuffer));
        if (ClientBytesRecv > 0)
        {
            dwBytesWritten = ClientBytesRecv - 16;
            p->payloadBuffer = new char[ClientBytesRecv - 16];
            ZeroMemory(p->payloadBuffer, sizeof(p->payloadBuffer));
            memcpy(p->payloadBuffer, p->recvBuffer + 16, ClientBytesRecv - 16);
            WriteFile(p->hFile, p->payloadBuffer, dwBytesWritten, &dwBytesWritten, NULL);
        }
    }

    //fs.close();
    return 0;
}

UINT CRTSPClientDlg::playThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;
    Sleep(100);

    p->m_player.OpenMedia(p->c_filePath.c_str());
    p->m_player.Play();
    p->g_play = TRUE;
    p->m_btnPlay.SetWindowText(_T("PAUSE"));

    // set end time
    libvlc_time_t local_fileLength;
    local_fileLength = p->m_player.GetLength();
    CTimeSpan length(static_cast<time_t>(local_fileLength / 1000));
    CString endTimeStr;
    endTimeStr.Format(L"%02d:%02d", length.GetMinutes(), length.GetSeconds());
    p->m_endTime.SetWindowText(endTimeStr);

    if (p->g_local)
        UINT_PTR m_nWindowTimer = p->SetTimer(SETLOCALTIME, 100, NULL);
    else if (p->g_rtsp) {
        UINT_PTR m_nWindowTimer = p->SetTimer(SETRTSPTIME, 100, NULL);
    }
    return 0;
}

void CRTSPClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //reinterpret_cast <new_type> (expression)，从CScrollBar指针到CSliderCtrl指针
    //reinterpret_cast运算符是用来处理无关类型之间的转换；它会产生一个新的值，这个值会有与原始参数（expression）有完全相同的比特位。
    CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
    if (*pSlider == m_volBar)
    {
        int pos = m_volBar.GetPos();
        m_player.SetVolume(pos);
        CString volumeStr;
        volumeStr.Format(L"%d", pos);
        m_volText.SetWindowText(volumeStr);
        // todo set icon
    }

    if (*pSlider == m_progressBar)
    {
        if (g_local)
        {
            int pos = m_progressBar.GetPos();
            m_player.SetTime(c_fileLength / 100 * pos);
        }
        if (g_rtsp)
        {
            int pos = m_progressBar.GetPos();
            libvlc_time_t m_curPos = pos / 100 * c_fileLength;
            m_progressBar.SetPos(pos);
            Sleep(10);
            m_rtsp_request.RequestPlay(m_curPos);
            m_cs.Send(m_cs.RTSPSocket, m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
            m_rtsp_reply.response = m_cs.Recv(m_cs.RTSPSocket);
            if (!m_rtsp_reply.CheckRtspResponse(PLAY) || m_rtsp_reply.Cseq != m_rtsp_request.Cseq)
            {
                AfxMessageBox(L"PLAY failed with error.");
            }
            g_forward = TRUE;
        }
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRTSPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
    libvlc_time_t local_fileLength;
    libvlc_time_t rtsp_fileLength;
    if (nIDEvent == SETLOCALTIME)
    {
        local_fileLength = m_player.GetLength();
        c_fileLength = local_fileLength;
        m_curTime = m_player.GetPlayTime();
        //将int转换为time_t类型
        //time_t：秒数
        //CTimeSpan：存储时间跨度中的秒数
        CTimeSpan actualPosition(static_cast<time_t>(m_curTime / 1000));
        if (m_curTime % 1000 > 500) {
            actualPosition += 1;
        }
        CString currentTimeStr;

        currentTimeStr.Format(L"%02d:%02d", actualPosition.GetMinutes(), actualPosition.GetSeconds());
        m_startTime.SetWindowText(currentTimeStr);

        m_curPos = (int)(m_curTime * 100 / local_fileLength) + 1;
        m_progressBar.SetPos(m_curPos);
    }
    else if (nIDEvent == SETRTSPTIME)
    {
        rtsp_fileLength = c_fileLength * 1000;
        if (g_forward)
        {
            int pos = m_progressBar.GetPos();
            m_curTime = pos * c_fileLength * 10;
            g_forward = FALSE;
        }
        else if (m_curTime < rtsp_fileLength && !g_pause)
        {
            m_curTime = m_curTime + 100;
        }
        CTimeSpan length(static_cast<time_t>(rtsp_fileLength / 1000));
        CTimeSpan actualPosition(static_cast<time_t>(m_curTime / 1000));
        CString currentTimeStr, endTimeStr;
        currentTimeStr.Format(L"%02d:%02d", actualPosition.GetMinutes(), actualPosition.GetSeconds());
        endTimeStr.Format(L"%02d:%02d", length.GetMinutes(), length.GetSeconds());
        m_startTime.SetWindowText(currentTimeStr);
        m_endTime.SetWindowText(endTimeStr);
        m_curPos = (int)(m_curTime * 100 / rtsp_fileLength);
        m_progressBar.SetPos(m_curPos);
    }
    CDialog::OnTimer(nIDEvent);
}

void CRTSPClientDlg::OnOpenRtsp()
{
    // TODO: 在此添加命令处理程序代码
    CRTSPUrlDlg m_open_rtsp;
    if (m_open_rtsp.DoModal() == IDOK) {
        m_url = m_open_rtsp.m_rtsp_url;
        CString output;
        //CW2A将宽字符集（Unicode）转化为多字符集（ASCII）
        c_url = CT2CA(m_url);
        cur_url.read(c_url);
        if (cur_url.getProtocol() == "rtsp") {
            g_rtsp = TRUE;
            c_serverIp = cur_url.getIp();
            c_fileName = cur_url.getName();
            c_port = cur_url.getPort();
            output = c_fileName.c_str();
            m_audioName.SetWindowText(CString(cur_url.getName().c_str()));
            
            // play
            OnBnClickedBtnPlay();
        }
        else {
            MessageBox(L"The URL is invalid. Try again.");
            return;
        }
    }
}


void CRTSPClientDlg::OnOpenLocal()
{
    // TODO: 在此添加命令处理程序代码
    CString filePath;
    CFileDialog dlg(TRUE,
        NULL,
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("MPEG (*.mp3)|*.mp3|All Files (*.*)|*.*||"), //过滤，只能选择：mp3或all files
        NULL);
    if (dlg.DoModal() == IDOK)
    {
        filePath = dlg.GetPathName();
        c_filePath = CT2CA(filePath.GetBuffer(0));
        g_local = TRUE;
        int pos = c_filePath.find_last_of('\\', c_filePath.length());
        c_fileName = c_filePath.substr(pos + 1, c_filePath.length());
        CString output;
        output = c_fileName.c_str();
        m_audioName.SetWindowText(output);
    }

    if (!g_init)
    {
        AfxBeginThread(playThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
        g_init = TRUE;
    }
    else if (g_pause)
    {
        m_player.Play();
        g_pause = FALSE;
        g_play = TRUE;
    }
}

afx_msg LRESULT CRTSPUrlDlg::OnGetUrl(WPARAM wParam, LPARAM lParam)
{
    CString* url = (CString*)lParam;
    return 0;
}


void CRTSPUrlDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    m_rtsp_url = getRtspUrl();
    CDialogEx::OnOK();
}
