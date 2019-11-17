
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
	DDX_Control(pDX, IDC_CHECK_SAVE, m_check_save);
	DDX_Control(pDX, IDC_CHECK_PRINT, m_check_print);
}

BEGIN_MESSAGE_MAP(CFileClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CANCEL, &CFileClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_REQUEST_BTN, &CFileClientDlg::OnBnClickedRequestBtn)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &CFileClientDlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDC_CHECK_PRINT, &CFileClientDlg::OnBnClickedCheckPrint)
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
	m_save_checked = m_check_save.GetCheck();
	m_print_checked = m_check_print.GetCheck();

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
		CString ip;
		ip.Format("%d.%d.%d.%d", field[0], field[1], field[2], field[3]);

		// set disable
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PRINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(FALSE);

		fileClient.HintsAndResult(ip, m_port);
		fileClient.Open();
		fileClient.setSendRecvBuffer(BDP);
		fileClient.Connect();
		fileClient.Send(m_filepath, strlen(m_filepath));

		
		if (m_save_checked) {
			if (m_savepath == "") {
				m_savepath = m_filepath;
			}
			fstream fs;
			// C++ open 打开文件（含打开模式一览表） http://c.biancheng.net/view/294.html
			fs.open(m_savepath, ios::app|ios::binary);

			int fileRecv = 0;
			while (fileRecv != -1) {
				fileRecv = fileClient.Read(buffer, sizeof(buffer));
				fs.write(buffer, fileRecv);

				if (m_print_checked) {
					m_content += buffer;
					UpdateData(FALSE);
				}
			}

			fs.close();
		}
		else if (m_print_checked) {
			fileClient.Read(buffer, sizeof(buffer));
			m_content = buffer;
			UpdateData(FALSE);
		}
	}
	//else {
		m_started = false;
		GetDlgItem(ID_REQUEST_BTN)->SetWindowText("&Request");
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILEPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PRINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
	//}
}


void CFileClientDlg::OnBnClickedCheckSave()
{
	// TODO: 在此添加控件通知处理程序代码
	m_save_checked = m_check_save.GetCheck();
}


void CFileClientDlg::OnBnClickedCheckPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	m_print_checked = m_check_print.GetCheck();
}
