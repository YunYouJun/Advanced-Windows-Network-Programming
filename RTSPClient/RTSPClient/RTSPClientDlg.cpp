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
    m_rtsp_url_ctrl.SetWindowTextW(L"rtsp://rtsp.yunyoujun.cn:554/adamas.mp3");
    return TRUE;
}


// CRTSPClientDlg 对话框
CRTSPClientDlg::CRTSPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RTSPCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
}

CRTSPClientDlg::~CRTSPClientDlg()
{
	//TEARDOWN
	m_rtsp_request.RequestTeardown();
	m_cs_tcp.Send(m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
	// close
	m_cs_tcp.Close();
	m_cs_udp.Close();
	m_fs.close();
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
	DDX_Control(pDX, IDC_VOL, m_btnVol);
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
	ON_BN_CLICKED(IDC_VOL, &CRTSPClientDlg::OnBnClickedVol)
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

	//icon
	HBITMAP hBmp_play = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY));
	HBITMAP hBmp_pause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE));
	HBITMAP hBmp_stop = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_STOP));
	HBITMAP hBmp_volUp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_UP));
	m_btnPlay.SetBitmap(hBmp_play);
	m_btnStop.SetBitmap(hBmp_stop);
	m_btnVol.SetBitmap(hBmp_volUp);

    // TODO:  在此添加额外的初始化代码
	g_init = FALSE;
	g_rtsp = FALSE;
	g_local = FALSE;
	g_play = FALSE;
	g_pause = FALSE;
    g_adjust = FALSE;
	g_mute = FALSE;
    
	m_curTime = 0;
    m_progressBar.SetRange(0, 100);
    m_volBar.SetRange(0, 100);
    m_volBar.SetPos(100);
    m_volText.SetWindowText(_T("100"));

	// bold
	//CFont pfont;
	//pfont.CreateFontW(10, 0, 0, 0, FW_BOLD, 0, 1, 0, 0, 0, 0, 0, 0, _T("Mircosoft YaHei"));
	//m_audioName.SetFont(&pfont);
	// whole y cannot be display

	m_btnPlay.EnableWindow(FALSE);
	m_progressBar.EnableWindow(FALSE);
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
        if (g_local)
        {
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

				HBITMAP hBmp_pause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE));
				m_btnPlay.SetBitmap(hBmp_pause);
            }
        }

        if (g_rtsp)
        {
            if (!g_init)
            {
                AfxBeginThread(rtspThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
                g_init = TRUE;
            }
            else if (g_pause)
            {
                m_rtsp_request.RequestPlay(PLAY_AFTER_PAUSE);
                m_cs_tcp.Send(m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
				m_cs_tcp.Recv(m_buf);
				m_rtsp_reply.response = m_buf;

				m_player.Play();

                if (!m_rtsp_reply.CheckRtspResponse(PLAY))
                {
                    TRACE(L"PLAY failed with error.");
				}
				else {
					g_play = TRUE;
					g_pause = FALSE;
					HBITMAP hBmp_pause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE));
					m_btnPlay.SetBitmap(hBmp_pause);
				}
            }
        }
    }
    else {
        if (g_local)
        {
            m_player.Pause();
            g_pause = TRUE;
            g_play = FALSE;

			HBITMAP hBmp_play = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY));
			m_btnPlay.SetBitmap(hBmp_play);
        }
        if (g_rtsp)
        {
            m_player.Pause();
            m_rtsp_request.RequestPause();
            m_cs_tcp.Send(m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));

			m_cs_tcp.Recv(m_buf);
			m_rtsp_reply.response = m_buf;

            if (!m_rtsp_reply.CheckRtspResponse(PAUSE))
            {
                AfxMessageBox(L"PAUSE failed with error.");
            }
            else {
                g_pause = TRUE;
                g_play = FALSE;

				HBITMAP hBmp_play = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY));
				m_btnPlay.SetBitmap(hBmp_play);
            }
        }
    }
}

void CRTSPClientDlg::OnBnClickedBtnStop()
{
    CString volumeStr;
    m_curTime = 0;
    m_curPos = 0;
	c_fileLength = 0;
    m_player.Stop();
    if (g_local)
    {
		g_local = FALSE;

        KillTimer(SETLOCALTIME);
        m_progressBar.SetPos(0);
        m_startTime.SetWindowText(L"00:00");
    }
    if (g_rtsp)
    {
		g_rtsp = FALSE;

        KillTimer(SETRTSPTIME);
        m_progressBar.SetPos(0);
        m_startTime.SetWindowText(L"00:00");

        //TEARDOWN
        m_rtsp_request.RequestTeardown();
        m_cs_tcp.Send(m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
        m_cs_tcp.Recv(m_buf);
		m_rtsp_reply.response = m_buf;
        if (!m_rtsp_reply.CheckRtspResponse(TEARDOWN) && m_rtsp_reply.Cseq != m_rtsp_request.Cseq)
        {
            TRACE(L"TEARDOWN failed with error.");
        }

        m_cs_tcp.Close();
		m_cs_udp.Close();
		m_fs.close();
    }
    g_init = FALSE;
    g_pause = FALSE;
    g_play = FALSE;
    g_adjust = FALSE;

	m_audioName.SetWindowText(L"YunYou - AudioPlayer");
	AfxGetMainWnd()->SetWindowTextW(L"RTSPClient");

	HBITMAP hBmp_play = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PLAY));
	m_btnPlay.SetBitmap(hBmp_play);
}

UINT CRTSPClientDlg::rtspThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;
    p->m_rtsp_request.url = p->c_url;

    if (!p->m_cs_tcp.Open(TCP))
    {
        AfxMessageBox(L"Socket failed with error.");
    }

    if (!p->m_cs_tcp.Connect(p->cur_url.getIp().c_str(), p->cur_url.getPort()))
    {
        AfxMessageBox(L"Connect failed with error.");
    }

    //OPTIONS
    p->m_rtsp_request.RequestOptions();
    p->m_cs_tcp.Send(p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
	p->m_cs_tcp.Recv(p->m_buf);
	p->m_rtsp_reply.response = p->m_buf;
    if (!p->m_rtsp_reply.CheckRtspResponse(OPTIONS) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"OPTIONS failed with error.");
		return -1;
    }

    //DESCRIBE
    p->m_rtsp_request.RequestDescribe();
    p->m_cs_tcp.Send(p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    p->m_cs_tcp.Recv(p->m_buf);
	p->m_rtsp_reply.response = p->m_buf;
    if (!p->m_rtsp_reply.CheckRtspResponse(DESCRIBE) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"DESCRIBE failed with error.");
		return -1;
    }
    p->m_rtsp_reply.GetAudioLen();
	// why? rtsp time more than local
    p->c_fileLength = p->m_rtsp_reply.file_length;

    //SETUP
    p->m_rtsp_request.RequestSetup();
    p->m_cs_tcp.Send(p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
    p->m_cs_tcp.Recv(p->m_buf);
	p->m_rtsp_reply.response = p->m_buf;
    if (!p->m_rtsp_reply.CheckRtspResponse(SETUP) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
    {
        AfxMessageBox(L"SETUP failed with error.");
		return -1;
	}
	else {
		p->m_rtsp_reply.GetResponseClientRtpPort();
		p->m_rtsp_reply.GetResponseServerRtpPort();
		p->c_clientRtpPort = p->m_rtsp_reply.ClientRtpPort;
		p->c_serverRtpPort = p->m_rtsp_reply.ServerRtpPort;
	}

    //PLAY
    p->m_rtsp_request.Session = p->m_rtsp_reply.Session;
    p->m_rtsp_request.RequestPlay(0); // from head
    p->m_cs_tcp.Send(p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
	Sleep(100);
	p->m_cs_tcp.Recv(p->m_buf);
	p->m_rtsp_reply.response = p->m_buf;
	if (!p->m_rtsp_reply.CheckRtspResponse(PLAY) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
	{
		AfxMessageBox(L"PLAY failed with error.");
		return -1;
	}
	else {
		AfxBeginThread(rtpThread, p, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		Sleep(1000);
		AfxBeginThread(playThread, p, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}

    //PARAMETER
	// wait p->play change
	Sleep(100);
    while (p->g_rtsp && p->g_play)
    {
        p->m_rtsp_request.RequestGetparameter();
        p->m_cs_tcp.Send(p->m_rtsp_request.request.c_str(), strlen(p->m_rtsp_request.request.c_str()));
		p->m_cs_tcp.Recv(p->m_buf);
		p->m_rtsp_reply.response = p->m_buf;
        if (!p->m_rtsp_reply.CheckRtspResponse(GET_PARAMETER) || p->m_rtsp_reply.Cseq != p->m_rtsp_request.Cseq)
        {
            AfxMessageBox(L"GET PARAMETER failed with error.");
        }
        // because timeout: 65s
        Sleep(60000);
    }

	p->m_cs_tcp.Close();
    return 0;
}

UINT CRTSPClientDlg::rtpThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;

    p->c_filePath = "./_cache.mp3";
	p->m_fs.open(p->c_filePath, ios::out | ios::binary);

	if (!p->m_cs_udp.Open(UDP))
	{
		AfxMessageBox(L"Socket failed with error.");
		return -1;
	}
	if ( !p->m_cs_udp.Bind(atoi(p->c_clientRtpPort.c_str())) )
	{
		AfxMessageBox(L"Bind failed with error.");
		return -1;
	}

	while (p->g_rtsp && p->m_cs_udp.m_socket != INVALID_SOCKET)
	{
		int recv_size = p->m_cs_udp.RecvFrom(p->m_rtp_buf, p->cur_url.getIp().c_str(), atoi(p->c_serverRtpPort.c_str()));
		if (recv_size > 0) {
			// header 16
			// https://datatracker.ietf.org/doc/rfc3550/?include_text=1
			// 5.1 RTP Fixed Header Fields
			p->m_fs.write(p->m_rtp_buf + 16, recv_size - 16);
		}
	}

	p->m_cs_udp.Close();
	p->m_fs.close();
    return 0;
}

UINT CRTSPClientDlg::playThread(LPVOID lpParam)
{
    CRTSPClientDlg *p = (CRTSPClientDlg *)lpParam;
	p->m_progressBar.EnableWindow(TRUE);

	p->g_play = TRUE;
    p->m_player.OpenMedia(p->c_filePath.c_str());
    p->m_player.Play();    
    //p->m_btnPlay.SetWindowText(_T("PAUSE"));

	HBITMAP hBmp_pause = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PAUSE));
	p->m_btnPlay.SetBitmap(hBmp_pause);

	// set end time
	libvlc_time_t local_fileLength;
	local_fileLength = p->m_player.GetLength();
	if (p->c_fileLength) {
		local_fileLength = p->c_fileLength;
	}
	CTimeSpan length(static_cast<time_t>(local_fileLength / 1000));
	CString endTimeStr;
	endTimeStr.Format(L"%02d:%02d", length.GetMinutes(), length.GetSeconds());
	p->m_endTime.SetWindowText(endTimeStr);

	if (p->g_local) {
		UINT_PTR m_nWindowTimer = p->SetTimer(SETLOCALTIME, 100, NULL);
	}
    else if (p->g_rtsp) {
        UINT_PTR m_nWindowTimer = p->SetTimer(SETRTSPTIME, 100, NULL);
    }

	p->m_btnPlay.EnableWindow(TRUE);
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
		g_mute = false;

        int pos = m_volBar.GetPos();
        m_player.SetVolume(pos);
        CString volumeStr;
        volumeStr.Format(L"%d", pos);
        m_volText.SetWindowText(volumeStr);
        // icon
		HBITMAP hBmp_volDown = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_DOWN));
		HBITMAP hBmp_volUp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_UP));
		HBITMAP hBmp_volMute = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_MUTE));
		if (pos > 50) {
			m_btnVol.SetBitmap(hBmp_volUp);
		}
		else if (pos > 0) {
			m_btnVol.SetBitmap(hBmp_volDown);
		}
		else {
			m_btnVol.SetBitmap(hBmp_volMute);
		}
    }

    if (*pSlider == m_progressBar)
    {
		Sleep(100);
        if (g_local)
        {
            int pos = m_progressBar.GetPos();
            m_player.SetTime(pos * c_fileLength / 100);
        }
        if (g_rtsp)
        {
            int pos = m_progressBar.GetPos();
			m_curTime = pos * c_fileLength / 100;
			int start_second = pos * c_fileLength / 100;
			m_rtsp_request.RequestPlay(start_second / 1000);
            m_cs_tcp.Send(m_rtsp_request.request.c_str(), strlen(m_rtsp_request.request.c_str()));
            m_cs_tcp.Recv(m_buf);
			m_rtsp_reply.response = m_buf;
            if (!m_rtsp_reply.CheckRtspResponse(PLAY) || m_rtsp_reply.Cseq != m_rtsp_request.Cseq)
            {
                TRACE(L"PLAY failed with error.");
            }
            g_adjust = TRUE;
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
        CString currentTimeStr;

        currentTimeStr.Format(L"%02d:%02d", actualPosition.GetMinutes(), actualPosition.GetSeconds());
        m_startTime.SetWindowText(currentTimeStr);

        m_curPos = (int)(m_curTime * 100 / local_fileLength) + 1;
        m_progressBar.SetPos(m_curPos);
    }
    else if (nIDEvent == SETRTSPTIME)
    {
        rtsp_fileLength = c_fileLength;
        if (g_adjust)
        {
            int pos = m_progressBar.GetPos();
            m_curTime = pos * c_fileLength / 100;
            g_adjust = FALSE;
        }
        else if (m_curTime < rtsp_fileLength && !g_pause)
        {
            m_curTime = m_curTime + 100;
        }

		// common
		CTimeSpan actualPosition(static_cast<time_t>(m_curTime / 1000));
		CString currentTimeStr;
		currentTimeStr.Format(L"%02d:%02d", actualPosition.GetMinutes(), actualPosition.GetSeconds());
		m_startTime.SetWindowText(currentTimeStr);

        m_curPos = (int)(m_curTime * 100 / rtsp_fileLength);
        m_progressBar.SetPos(m_curPos);
    }

	if (m_curPos == 100) {
		g_play = FALSE;
	}

    CDialog::OnTimer(nIDEvent);
}

void CRTSPClientDlg::OnOpenRtsp()
{
    // TODO: 在此添加命令处理程序代码
    CRTSPUrlDlg m_open_rtsp;
    if (m_open_rtsp.DoModal() == IDOK) {
		OnBnClickedBtnStop();

        m_url = m_open_rtsp.m_rtsp_url;
        CString output;
        //CW2A将宽字符集（Unicode）转化为多字符集（ASCII）
        c_url = CT2CA(m_url);
        cur_url.read(c_url);
        if (cur_url.getProtocol() == "rtsp") {
            g_rtsp = TRUE;

            c_fileName = cur_url.getName();
            output = c_fileName.c_str();
            m_audioName.SetWindowText(CString(cur_url.getName().c_str()));

			CString header = _T("RTSPClient - ") + output;
			AfxGetMainWnd()->SetWindowTextW(header);
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
		OnBnClickedBtnStop();

        filePath = dlg.GetPathName();
        c_filePath = CT2CA(filePath.GetBuffer(0));
        g_local = TRUE;
        int pos = c_filePath.find_last_of('\\', c_filePath.length());
        c_fileName = c_filePath.substr(pos + 1, c_filePath.length());
        CString output;
        output = c_fileName.c_str();
        m_audioName.SetWindowText(output);

		CString header = _T("RTSPClient - ") + output;
		AfxGetMainWnd()->SetWindowTextW(header);

		OnBnClickedBtnPlay();
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

void CRTSPClientDlg::OnBnClickedVol()
{
	HBITMAP hBmp_volDown = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_DOWN));
	HBITMAP hBmp_volUp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_UP));
	HBITMAP hBmp_volMute = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_VOL_MUTE));
	if (g_mute) {
		g_mute = false;
		int vol = m_volBar.GetPos();
		if (vol > 50) {
			m_btnVol.SetBitmap(hBmp_volUp);
		}
		else if (vol > 0){
			m_btnVol.SetBitmap(hBmp_volDown);
		}
		m_player.SetVolume(vol);
	}
	else {
		g_mute = true;
		m_btnVol.SetBitmap(hBmp_volMute);
		m_player.SetVolume(0);
	}
}
