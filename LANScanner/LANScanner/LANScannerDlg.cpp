
// LANScannerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LANScanner.h"
#include "LANScannerDlg.h"
#include "afxdialogex.h"

#include "afxdhtml.h"

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
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
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
	DDX_DHtml_SelectIndex(pDX, _T("nic-select"), m_nic_index);
	DDX_DHtml_SelectString(pDX, _T("nic-select"), m_nic_select_string);

	//DDX_DHtml_SelectString(pDX, _T("NIC"), m_nic_name);
	//m_nic_select.QueryInterface
	//DDX_DHtml_ElementText(pDX, _T("NIC"), m_nic_select);
	//DDX_DHtml_ElementInnerHtml(pDX, _T("NIC"), m_nic_select);
	//DDX_DHtml_Element(pDX, _T("NIC"), m_nic_select);
	//DDX_DHtml_ElementInnerText(pDX, _T("nic-select"), m_nic_select);
	DDX_DHtml_ElementValue(pDX, _T("ipAddress"), m_nic_ipaddress);
	DDX_DHtml_ElementValue(pDX, _T("subnetMask"), m_nic_subnetmask);
	DDX_DHtml_ElementValue(pDX, _T("macAddress"), m_nic_macaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayIPAddress"), m_nic_gatewayipaddress);
	DDX_DHtml_ElementValue(pDX, _T("gatewayMac"), m_nic_gatewaymac);
}

// Map the message to the handling function
BEGIN_MESSAGE_MAP(CLANScannerDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_CBN_SELCHANGE(IDC_COMBO_NIC, &CLANScannerDlg::OnSelectChangeNic)
END_MESSAGE_MAP()


// CLANScannerDlg 消息处理程序

BOOL CLANScannerDlg::OnInitDialog()
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
	//GetElementInterface(_T("nic-select"), IID_IHTMLElement, (void**)&m_nic_select);
	//if (m_nic_select == NULL)
		//return -1;
	
	for (vector<char*>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	{
		m_CComboBox.InsertString(row, *iter);
		++row;
	}
	//m_nic_select = "<select id=\"nic-select\" style=\"width:100 %;\">";
	//CComPtr<IHTMLElement> m_nic_select_option;
	//CString m_nic_select_option;
	//for (vector<char*>::const_iterator iter = al.begin(); iter != al.end(); ++iter)
	//{
	//	string name = *iter;
	//	m_nic_index = row;
	//	m_nic_select_string = name.c_str();
	//	//m_nic_select_option += ("<option value=\"\">" + name + "</option>").c_str();
	//	//m_nic_select_option->put_innerText(_com_util::ConvertStringToBSTR(*iter));
	//	//m_nic_select->add(m_nic_select_option, (_variant_t)(long)0);
	//	//.Insert(row, ("<option value=\"\">" + name + "</option>").c_str());
	//	m_nic_select += ("<option value=\"\">" + name + "</option>").c_str();
	//	//m_nic_select.Insert()
	//	++row;
	//}
	//m_nic_select->put_innerHTML((_bstr_t)m_nic_select_option);
	//m_nic_select = "<select id=\"nic-select\" style=\"width:100 %;\">" + m_nic_select_option + "</select>";
	m_nic_select += "</select>";


	//m_nic_ipaddress = "0.0.0.0";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//void  CLANScannerDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
//{
//	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);
//
//	// recreate the filelist table
//	CComPtr<IHTMLElement> spfileList;
//	GetElementInterface(_T("NIC"), IID_IHTMLElement, (void**)&spfileList);
//	if (spfileList == NULL)
//		return;
//
//	CComBSTR bstrTable = m_nic_select;
//	spfileList->put_innerHTML(bstrTable);
//
//}

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

HRESULT CLANScannerDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
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

//IHTMLElement* /*pElement*/
void CLANScannerDlg::OnSelectChangeNic()
{
	index = m_CComboBox.GetCurSel();
	nic.selectNIC(index);
	m_nic_ipaddress = CString(nic.getIPAddress());
	m_nic_subnetmask = CString(nic.getSubnetMask());
	m_nic_macaddress = CString(Common::mac2str(nic.getMacAddress()));
	m_nic_gatewayipaddress = CString(nic.getGatewayIPAddress());
	m_nic_gatewaymac = CString(Common::mac2str(nic.getGatewayMac()));

	//m_nic_select_string = "ok";
	UpdateData(FALSE);
}
