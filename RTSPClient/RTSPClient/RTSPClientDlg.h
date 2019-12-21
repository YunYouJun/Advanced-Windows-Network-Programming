
// RTSPClientDlg.h: 头文件
//

#pragma once
#include "URL.h"
#include "ClientSocket.h"
#include "RTSP.h"
#include "VLCPlayer.h"

#define SETLOCALTIME 1
#define SETRTSPTIME 2
#define PLAY_AFTER_PAUSE -1

#define WM_GET_URL (WM_USER + 1001)

// CRTSPClientDlg 对话框
class CRTSPClientDlg : public CDialogEx
{
// 构造
public:
	CRTSPClientDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CRTSPClientDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RTSPCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    // 匈牙利命名法
    // https ://blog.csdn.net/zpch666/article/details/85641278
    VLCPlayer m_player;
    ClientSocket m_cs_tcp;
	ClientSocket m_cs_udp;
	fstream m_fs;

	string c_url;
	// parse
    URL cur_url;
	char m_buf[MAX_BUF_LEN];
	char m_rtp_buf[MAX_BUF_LEN];

    RTSP m_rtsp_request;
    RTSP m_rtsp_reply;

    CSliderCtrl m_progressBar;
    CSliderCtrl m_volBar;
    CButton m_btnPlay;
    CButton m_btnStop;
    CStatic m_startTime;
    CStatic m_endTime;
    CStatic m_volText;
    CStatic m_audioName;
    CString m_url;

    // global var
    BOOL g_init;
    BOOL g_rtsp;
    BOOL g_play;
    BOOL g_pause;
    BOOL g_local;
    BOOL g_adjust;
	BOOL g_mute;

    string c_filePath;
    string c_fileName;
    string c_clientRtpPort;
    string c_serverRtpPort;
    libvlc_time_t c_fileLength;

    libvlc_time_t m_curTime;
    // you drag to
    int m_curPos;

    static UINT playThread(LPVOID lpParam);
    static UINT rtspThread(LPVOID lpParam);
    static UINT rtpThread(LPVOID lpParam);

    afx_msg void OnBnClickedBtnPlay();
    afx_msg void OnBnClickedBtnStop();

    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnOpenRtsp();
    afx_msg void OnOpenLocal();
	CButton m_btnVol;
	afx_msg void OnBnClickedVol();
};
