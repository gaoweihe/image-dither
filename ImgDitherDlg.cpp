
// ImgDitherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgDither.h"
#include "ImgDitherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CImgDitherDlg dialog

CImgDitherDlg::CImgDitherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgDitherDlg::IDD, pParent)
	, m_VChkMarkF(FALSE)
{
	m_VIpRGB = _T("0,0,0,0");
	m_VSrcFile = _T("");
	m_VClrR = 0;
	m_VClrG = 0;
	m_VClrB = 0;
	currIndex = 159;
	currX = 0;
	currY = 0;
	ampF = 1;
	imgMoveF = FALSE;
	ipRed=0, ipGreen=0, ipBlue=0, ipDelta=0;
	markX0 = 10, markY0 = 10;
	markX1 = ImgWdDEF-10, markY1=ImgHtDEF-10;
	Clr24BmpInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
	Clr24BmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Clr24BmpInfo->bmiHeader.biWidth = SheetWdDEF;
	Clr24BmpInfo->bmiHeader.biHeight = SheetHtDEF;
	Clr24BmpInfo->bmiHeader.biPlanes = 1;
	Clr24BmpInfo->bmiHeader.biBitCount = 24;
	Clr24BmpInfo->bmiHeader.biCompression = BI_RGB;
	Clr24BmpInfo->bmiHeader.biSizeImage = 0;
	Clr24BmpInfo->bmiHeader.biXPelsPerMeter = 1000;
	Clr24BmpInfo->bmiHeader.biYPelsPerMeter = 1000;
	Clr24BmpInfo->bmiHeader.biClrUsed = 0;
	Clr24BmpInfo->bmiHeader.biClrImportant = 0;
	pOrgBuff = (BYTE*)malloc(SheetWdDEF*SheetHtDEF*sizeof(BYTE)*3);
	Clr8BmpInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	Clr8BmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Clr8BmpInfo->bmiHeader.biWidth = SheetWdDEF;
	Clr8BmpInfo->bmiHeader.biHeight = SheetHtDEF;
	Clr8BmpInfo->bmiHeader.biPlanes = 1;
	Clr8BmpInfo->bmiHeader.biBitCount = 8;
	Clr8BmpInfo->bmiHeader.biCompression = BI_RGB;
	Clr8BmpInfo->bmiHeader.biSizeImage = 0;
	Clr8BmpInfo->bmiHeader.biXPelsPerMeter = 1000;
	Clr8BmpInfo->bmiHeader.biYPelsPerMeter = 1000;
	Clr8BmpInfo->bmiHeader.biClrUsed = 0;
	Clr8BmpInfo->bmiHeader.biClrImportant = 0;
	SetColorSetting(5);
	pTgtBuff=(BYTE*)malloc(SheetWdDEF*SheetHtDEF*sizeof(BYTE));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgDitherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRCFILE, m_VSrcFile);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_SET, m_CClrBt);
	DDX_Text(pDX, IDC_EDIT_RED, m_VClrR);
	DDV_MinMaxInt(pDX, m_VClrR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_VClrG);
	DDV_MinMaxInt(pDX, m_VClrG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_VClrB);
	DDV_MinMaxInt(pDX, m_VClrB, 0, 255);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_CCmbColor);
	DDX_Control(pDX, IDC_COMBO_MODE, m_CCmbDMode);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, m_CCmbDisp);
	DDX_Control(pDX, IDC_COMBO_AMPF, m_CCmbAmp);
	DDX_Control(pDX, IDC_COMBO_SUB, m_CCmbSub);
	DDX_Text(pDX, IDC_EDIT_IPRGB, m_VIpRGB);
	DDX_Control(pDX, IDC_SPIN_RED, m_CSpinRed);
	DDX_Control(pDX, IDC_SPIN_GREEN, m_CSpinGreen);
	DDX_Control(pDX, IDC_SPIN_BLUE, m_CSpinBlue);
	DDX_Control(pDX, IDC_SPIN_DELTA, m_CSpinDelta);
	DDX_Check(pDX, IDC_CHECK_MARK, m_VChkMarkF);
	DDX_Control(pDX, IDC_COMBO_IPMODE, m_CCmbIpMode);
}

BEGIN_MESSAGE_MAP(CImgDitherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CImgDitherDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_SET, &CImgDitherDlg::OnBnClickedMfccolorbuttonSet)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CImgDitherDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CImgDitherDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DITHER, &CImgDitherDlg::OnBnClickedButtonDither)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, &CImgDitherDlg::OnCbnSelchangeComboDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_AMPF, &CImgDitherDlg::OnCbnSelchangeComboAmpf)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RED, &CImgDitherDlg::OnDeltaposSpinRed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GREEN, &CImgDitherDlg::OnDeltaposSpinGreen)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BLUE, &CImgDitherDlg::OnDeltaposSpinBlue)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELTA, &CImgDitherDlg::OnDeltaposSpinDelta)
	ON_BN_CLICKED(IDC_CHECK_MARK, &CImgDitherDlg::OnBnClickedCheckMark)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_HISTO, &CImgDitherDlg::OnBnClickedButtonHisto)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CImgDitherDlg::OnBnClickedButtonGo)
END_MESSAGE_MAP()

// CImgDitherDlg message handlers

BOOL CImgDitherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_CCmbColor.SetCurSel(3);
	m_CCmbDMode.SetCurSel(0);
	m_CCmbDisp.SetCurSel(0);
	m_CCmbAmp.SetCurSel(0);
	m_CCmbSub.SetCurSel(0);
	m_CCmbIpMode.SetCurSel(0);
	m_CSpinRed.SetRange(1, 19);
	m_CSpinRed.SetPos(10);
	m_CSpinGreen.SetRange(1, 19);
	m_CSpinGreen.SetPos(10);
	m_CSpinBlue.SetRange(1, 19);
	m_CSpinBlue.SetPos(10);
	m_CSpinDelta.SetRange(0, 9);
	m_CSpinDelta.SetPos(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImgDitherDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (pTgtBuff != NULL) delete(pTgtBuff);
	if (Clr8BmpInfo != NULL) delete(Clr8BmpInfo);
	if (pOrgBuff != NULL) delete(pOrgBuff);
	if (Clr24BmpInfo != NULL) delete(Clr24BmpInfo);
}

void CImgDitherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImgDitherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		RedrawColors();
		RedrawHistogram();
		RedrawImages();
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CImgDitherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Subroutines

void CImgDitherDlg::SetColorSetting(int factor)
{
	int lp;
	for (lp = 0; lp<0x100; lp++) {
		Clr8BmpInfo->bmiColors[lp].rgbBlue = 0xFF;
		Clr8BmpInfo->bmiColors[lp].rgbGreen = 0xFF;
		Clr8BmpInfo->bmiColors[lp].rgbRed = 0xFF;
	} //Set gray value
	if (factor < 2 || factor>5) return;
	for (lp = 0; lp<factor*factor*factor; lp++) {
		Clr8BmpInfo->bmiColors[lp].rgbBlue = lp%factor*255/(factor-1);
		Clr8BmpInfo->bmiColors[lp].rgbGreen = lp / factor%factor * 255 / (factor - 1);
		Clr8BmpInfo->bmiColors[lp].rgbRed = lp / factor/factor%factor * 255 / (factor - 1);
	} //Set gray value
} //End of SetColorSetting

void CImgDitherDlg::DrawAHistogram(int hIndex)
{
	CDC *pDC = GetDC();
	COLORREF color;
	int x0, y0, ht;
	x0 = HistoX0DEF + hIndex * HistoWdDEF;
	y0 = HistoY0DEF;
	color = RGB(0x00, 0x00, 0x00);
	pDC->FillSolidRect(x0, y0,
		HistoWdDEF - 1, HistoHtDEF+HMarkHtDEF, color);
	color = RGB(0xFF, 0xFF, 0xFF);
	pDC->FillSolidRect(x0 + 1, y0 + HistoHtDEF+1,
		HistoWdDEF - 3, HMarkHtDEF - 2, color);
	ht = 10;
	pDC->FillSolidRect(x0 + 1, y0+1,
		HistoWdDEF - 3, HistoHtDEF-ht-2, color);
	color = RGB(Clr8BmpInfo->bmiColors[hIndex+10].rgbRed,
			Clr8BmpInfo->bmiColors[hIndex+10].rgbGreen,
			Clr8BmpInfo->bmiColors[hIndex+10].rgbBlue);
	pDC->FillSolidRect(x0 + 1, y0 + HistoHtDEF - ht,
		HistoWdDEF - 3, ht, color);
	ReleaseDC(pDC);
} //End of DrawAHistogram

void CImgDitherDlg::RedrawHistogram()
{
	int lp;
	for (lp = 0; lp < 8; lp++) DrawAHistogram(lp);
} //End of RedrawHistogram

void CImgDitherDlg::DrawABlkOfColor(int cIndex)
{
	CDC *pDC = GetDC();
	COLORREF color;
	int x0, y0;
	x0 = ClrX0DEF + cIndex % 32 * ClrUWdDEF;
	y0 = ClrY0DEF + cIndex / 32 * ClrUHtDEF;
	if (cIndex == currIndex) {
		color = RGB(0x00,0x00,0x00);
		pDC->FillSolidRect(x0, y0, ClrUWdDEF - 1, ClrUHtDEF - 1, color);
		color = RGB(Clr8BmpInfo->bmiColors[cIndex].rgbRed,
			Clr8BmpInfo->bmiColors[cIndex].rgbGreen, Clr8BmpInfo->bmiColors[cIndex].rgbBlue);
		pDC->FillSolidRect(x0+1, y0+1, ClrUWdDEF - 3, ClrUHtDEF - 3, color);
	} //if it is the current index
	else {
		color = RGB(Clr8BmpInfo->bmiColors[cIndex].rgbRed,
			Clr8BmpInfo->bmiColors[cIndex].rgbGreen, Clr8BmpInfo->bmiColors[cIndex].rgbBlue);
		pDC->FillSolidRect(x0, y0, ClrUWdDEF - 1, ClrUHtDEF - 1, color);
	}; //else, other color
	ReleaseDC(pDC);
} //End of DrawABlkOfColor

void CImgDitherDlg::RedrawColors()
{
	int lp;
	for (lp = 0; lp < MaxColorDEF; lp++)DrawABlkOfColor(lp);
} //End of RedrawColors

void CImgDitherDlg::RedrawImages()
{
	CDC *pDC = GetDC();
	HDC hdc = pDC->GetSafeHdc();
	if (m_CCmbDisp.GetCurSel() == 0) {
		StretchDIBits(hdc, ImgX0DEF, ImgY0DEF, ImgWdDEF, ImgHtDEF,
			currX, currY, ImgWdDEF/ampF, ImgHtDEF/ampF,
			pOrgBuff, Clr24BmpInfo, DIB_RGB_COLORS, SRCCOPY);
	} //if original
	else
	{
		StretchDIBits(hdc, ImgX0DEF, ImgY0DEF, ImgWdDEF, ImgHtDEF,
			currX, currY, ImgWdDEF / ampF, ImgHtDEF / ampF,
			pTgtBuff, Clr8BmpInfo, DIB_RGB_COLORS, SRCCOPY);
	} //else, dithered
	if (m_VChkMarkF) {
		pDC->MoveTo(ImgX0DEF + markX0, ImgY0DEF);
		pDC->LineTo(ImgX0DEF + markX0, ImgY0DEF + ImgHtDEF - 1);
		pDC->MoveTo(ImgX0DEF + markX1, ImgY0DEF);
		pDC->LineTo(ImgX0DEF + markX1, ImgY0DEF + ImgHtDEF - 1);
		pDC->MoveTo(ImgX0DEF, ImgY0DEF + markY0);
		pDC->LineTo(ImgX0DEF + ImgWdDEF - 1, ImgY0DEF + markY0);
		pDC->MoveTo(ImgX0DEF, ImgY0DEF + markY1);
		pDC->LineTo(ImgX0DEF + ImgWdDEF - 1, ImgY0DEF + markY1);
	}; //if markF
	ReleaseDC(pDC);
} //End of RedrawImages

int CImgDitherDlg::FindTheBestColor(int r, int g, int b)
{
	int lp, minV,index,mr,mg,mb,currV;
	minV = 0xFFFFFF;
	index = 0;
	for (lp = 0; lp < MaxColorDEF; lp++) {
		mr = Clr8BmpInfo->bmiColors[lp].rgbRed;
		mg = Clr8BmpInfo->bmiColors[lp].rgbGreen;
		mb = Clr8BmpInfo->bmiColors[lp].rgbBlue;
		currV = (mr - r)*(mr - r) + (mg - g)*(mg - g) + (mb - b)*(mb - b);
		if (currV < minV) {
			minV = currV;
			index = lp;
		}; //if get minimal value
	} //End of for lp
	return(index);
} //End of FindTheBestColor

void CImgDitherDlg::DitherRow()
{
	int lpx,lpy, rowSWd,rowTWd,offset,index;
	int lp,delta;
	int currLn[SheetWdDEF*3], nextLn[SheetWdDEF*3];
	rowSWd = (Clr24BmpInfo->bmiHeader.biWidth*3 + 3) / 4*4;
	rowTWd = (Clr8BmpInfo->bmiHeader.biWidth + 3) / 4 * 4;
	for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth * 3; lpx++)
		nextLn[lpx] = pOrgBuff[lpx]; //End for lpx, 1st line
	for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight - 1; lpy++){
		offset = (lpy + 1)*rowSWd;
		for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth * 3; lpx++) {
			currLn[lpx]=nextLn[lpx];
			nextLn[lpx] = pOrgBuff[offset+lpx];
		} //End for lpx, 1st line
		for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth; lpx++){
			index = FindTheBestColor(currLn[lpx * 3 + 2],
				currLn[lpx * 3 + 1], currLn[lpx * 3]);
			pTgtBuff[lpy*rowTWd + lpx] = index;
			for (lp = 0; lp < 3; lp++) {
				delta = currLn[lpx * 3 + lp];
				switch (lp){
				case 1:
					delta-=	Clr8BmpInfo->bmiColors[index].rgbGreen;
					break;
				case 2:
					delta -= Clr8BmpInfo->bmiColors[index].rgbRed;
					break;
				default:
					delta -= Clr8BmpInfo->bmiColors[index].rgbBlue;
					break;
				} //End of switch
				if (lpx<Clr24BmpInfo->bmiHeader.biWidth - 1)
					currLn[(lpx + 1) * 3 + lp] += delta / 2;
				nextLn[lpx * 3+lp] += delta - delta / 2;
			} //End of for lp
		} //End for lpx
	} //End of for lpy
	for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth; lpx++){
		index = FindTheBestColor(nextLn[lpx * 3 + 2],
			nextLn[lpx * 3 + 1], nextLn[lpx * 3]);
		pTgtBuff[lpy*rowTWd + lpx] = index;
	}; //End of for lpx
} //End of DitherRow

void CImgDitherDlg::DitherColumn()
{
	int lpx, lpy, rowSWd, rowTWd, offset, index;
	int lp, delta;
	int currLn[SheetHtDEF*3], nextLn[SheetHtDEF*3];
	rowSWd = (Clr24BmpInfo->bmiHeader.biWidth * 3 + 3) / 4 * 4;
	rowTWd = (Clr8BmpInfo->bmiHeader.biWidth + 3) / 4 * 4;
	for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight; lpy++)
		for (lp=0; lp<3; lp++) nextLn[lpy*3+lp] = pOrgBuff[lpy*rowSWd+lp];
	for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth - 1; lpx++){
		offset = lpx*3;
		for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight*3; lpy++)
			currLn[lpy] = nextLn[lpy];
		for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight; lpy++) {
			for (lp = 0; lp<3; lp++) nextLn[lpy * 3 + lp] =
				pOrgBuff[lpy*rowSWd+offset + lp];
		} //End for lpx, 1st line
		for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight; lpy++){
			index = FindTheBestColor(currLn[lpy * 3 + 2],
				currLn[lpy * 3 + 1], currLn[lpy * 3]);
			pTgtBuff[lpy*rowTWd + lpx] = index;
			for (lp = 0; lp < 3; lp++) {
				delta = currLn[lpy * 3 + lp];
				switch (lp){
				case 1:
					delta -= Clr8BmpInfo->bmiColors[index].rgbGreen;
					break;
				case 2:
					delta -= Clr8BmpInfo->bmiColors[index].rgbRed;
					break;
				default:
					delta -= Clr8BmpInfo->bmiColors[index].rgbBlue;
					break;
				} //End of switch
				if (lpy<Clr24BmpInfo->bmiHeader.biHeight - 1)
					currLn[(lpy + 1) * 3 + lp] += delta / 2;
				nextLn[lpy * 3 + lp] += delta - delta / 2;
			} //End of for lp
		} //End for lpy
	} //End of for lpx
	for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight; lpy++){
		index = FindTheBestColor(nextLn[lpy * 3 + 2],
			nextLn[lpy * 3 + 1], nextLn[lpy * 3]);
		pTgtBuff[lpy*rowTWd + lpx] = index;
	}; //End of for lpy
} //End of DitherColumn

void CImgDitherDlg::DitherAverage()
{
	int lnBuff[3][SheetWdDEF*3 + 6];
	int lp, lpx, lpy, offset, len;
	int r, g, b, rowSWd, rowTWd, index;
	rowSWd = (Clr24BmpInfo->bmiHeader.biWidth * 3 + 3) / 4 * 4;
	rowTWd = (Clr8BmpInfo->bmiHeader.biWidth + 3) / 4 * 4;
	len = Clr24BmpInfo->bmiHeader.biWidth * 3;
	for (lpx = 0; lpx < len; lpx++)	lnBuff[2][lpx+3] = pOrgBuff[lpx];
	for (lp = 0; lp < 3; lp++) lnBuff[2][lp] = lnBuff[2][lp + 3];
	offset = Clr24BmpInfo->bmiHeader.biWidth * 3 + 3;
	for (lp = 0; lp < 3; lp++) 
		lnBuff[2][lp+offset] = lnBuff[2][lp + offset-3];
	for (lpx = 0; lpx < len+6; lpx++) lnBuff[1][lpx] = lnBuff[2][lpx];
	for (lpy = 0; lpy < Clr24BmpInfo->bmiHeader.biHeight; lpy++){
		for (lpx = 0; lpx < len + 6; lpx++) lnBuff[0][lpx] = lnBuff[1][lpx];
		for (lpx = 0; lpx < len + 6; lpx++) lnBuff[1][lpx] = lnBuff[2][lpx];
		if (lpy < Clr24BmpInfo->bmiHeader.biHeight - 1) {
			offset = lpy*rowSWd;
			for (lpx = 0; lpx < len; lpx++)
				lnBuff[2][lpx + 3] = pOrgBuff[offset+lpx];
			for (lp = 0; lp < 3; lp++) lnBuff[2][lp] = lnBuff[2][lp + 3];
			offset = Clr24BmpInfo->bmiHeader.biWidth * 3 + 3;
			for (lp = 0; lp < 3; lp++)
				lnBuff[2][lp + offset] = lnBuff[2][lp + offset - 3];
		}; // if not last line
		for (lpx = 0; lpx < Clr24BmpInfo->bmiHeader.biWidth; lpx++) {
			offset = lpx * 3;
			r = 0, g = 0, b = 0;
			for (lp = 0; lp < 3; lp++) {
				b += lnBuff[lp][offset] + lnBuff[lp][offset + 3];
				b+=lnBuff[lp][offset + 6];
				g += lnBuff[lp][offset + 1] + lnBuff[lp][offset + 4];
				g+=lnBuff[lp][offset + 7];
				r += lnBuff[lp][offset + 2] + lnBuff[lp][offset + 5];
				r+=lnBuff[lp][offset + 8];
			}; //End of for lp
			index = FindTheBestColor(r / 9, g / 9, b / 9);
			pTgtBuff[lpy*rowTWd + lpx] = index;
		}// End of for lpx
	} //End of for lpy
} //End of DitherAverage

void CImgDitherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect imgRect, clrRect;
	imgRect.SetRect(ImgX0DEF, ImgY0DEF, ImgX0DEF + ImgWdDEF, ImgY0DEF + ImgHtDEF);
	clrRect.SetRect(ClrX0DEF, ClrY0DEF, ClrX0DEF + ClrUWdDEF * 32, ClrY0DEF + ClrUHtDEF * 5);
	if (clrRect.PtInRect(point)) {
		currIndex = (point.y - ClrY0DEF) / ClrUHtDEF * 32;
		currIndex += (point.x - ClrX0DEF) / ClrUWdDEF;
		RedrawColors();
	}; //point is in Color rect
	if (imgRect.PtInRect(point)) {
		oldX = point.x;
		oldY = point.y;
		imgMoveF = TRUE;
	}; //point is in Image rect
	CDialogEx::OnLButtonDown(nFlags, point);
} //End of OnLButtonDown

void CImgDitherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (imgMoveF) {
		imgMoveF = FALSE;
		currX -= (point.x - oldX) / ampF;
		currY += (point.y - oldY) / ampF;
		if (currX<0) currX = 0;
		if (currY<0) currY = 0;
		if (currX + ImgWdDEF / ampF > Clr8BmpInfo->bmiHeader.biWidth)
			currX = Clr8BmpInfo->bmiHeader.biWidth - ImgWdDEF / ampF;
		if (currY + ImgHtDEF / ampF > Clr8BmpInfo->bmiHeader.biHeight)
			currY = Clr8BmpInfo->bmiHeader.biHeight - ImgHtDEF / ampF;
		RedrawImages();
	}; //if image move flag
	CDialogEx::OnLButtonUp(nFlags, point);
} //End of OnLButtonUp

void CImgDitherDlg::OnBnClickedButtonBrowse()
{
	char szFilters[] =
		"Filter Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog fileDlg(TRUE, _T("Bitmap file"), _T("*.bmp"),
		OFN_FILEMUSTEXIST, LPCTSTR(szFilters));
	UpdateData(TRUE);
	if (fileDlg.DoModal() == IDOK) {
		m_VSrcFile = fileDlg.GetPathName();
		CFile workFile;
		CFileException e;
		int len, wd, ht;
		BITMAPFILEHEADER bmpHdr;
		if (workFile.Open(m_VSrcFile, CFile::modeRead, &e)) {
			workFile.Read(&bmpHdr, sizeof(BITMAPFILEHEADER));
			workFile.Read(Clr24BmpInfo, sizeof(BITMAPINFOHEADER));
			Clr8BmpInfo->bmiHeader.biWidth = 
				Clr24BmpInfo->bmiHeader.biWidth;
			wd = Clr24BmpInfo->bmiHeader.biWidth;
			wd = (wd*3 + 3) / 4 * 4;
			Clr8BmpInfo->bmiHeader.biHeight = 
				Clr24BmpInfo->bmiHeader.biHeight;
			ht = Clr24BmpInfo->bmiHeader.biHeight;
			len = wd*ht;
			workFile.Read(pOrgBuff, len);
			workFile.Close();
			RedrawImages();
		}; //if opened
	}; //if IDOK
	UpdateData(FALSE);
} //End of OnBnClickedButtonBrowse

void CImgDitherDlg::OnBnClickedMfccolorbuttonSet()
{
	COLORREF color = m_CClrBt.GetColor();
	UpdateData(TRUE);
	m_VClrR = GetRValue(color);
	m_VClrG = GetGValue(color);
	m_VClrB = GetBValue(color);
	UpdateData(FALSE);
} //End of OnBnClickedMfccolorbuttonSet

void CImgDitherDlg::OnBnClickedButtonSet()
{
	UpdateData(TRUE);
	Clr8BmpInfo->bmiColors[currIndex].rgbBlue = m_VClrB;
	Clr8BmpInfo->bmiColors[currIndex].rgbGreen = m_VClrG;
	Clr8BmpInfo->bmiColors[currIndex].rgbRed = m_VClrR;
	RedrawColors();
	UpdateData(FALSE);
} //End of OnBnClickedButtonSet

void CImgDitherDlg::OnBnClickedButtonNew()
{
	UpdateData(TRUE);
	SetColorSetting(m_CCmbColor.GetCurSel() + 2);
	RedrawColors();
	UpdateData(FALSE);
} //End of OnBnClickedButtonNew

void CImgDitherDlg::OnBnClickedButtonDither()
{
	UpdateData(TRUE);
	BeginWaitCursor();
	Clr8BmpInfo->bmiHeader.biWidth = Clr24BmpInfo->bmiHeader.biWidth;
	Clr8BmpInfo->bmiHeader.biHeight = Clr24BmpInfo->bmiHeader.biHeight;
	switch (m_CCmbDMode.GetCurSel()) {
	case 1:
		DitherColumn();
		break;
	case 2:
		DitherAverage();
		break;
	default:
		DitherRow();
		break;
	} //ENd of switch
	RedrawImages();
	RedrawColors();
	EndWaitCursor();
	UpdateData(FALSE);
} //End of OnBnClickedButtonDither

void CImgDitherDlg::OnCbnSelchangeComboDisplay()
{
	UpdateData(TRUE);
	RedrawImages();
	UpdateData(FALSE);
} //End of OnCbnSelchangeComboDisplay

void CImgDitherDlg::OnCbnSelchangeComboAmpf()
{
	UpdateData(TRUE);
	ampF = m_CCmbAmp.GetCurSel() + 1;
	if (ampF>2) ampF = 4;
	if (currX + ImgWdDEF / ampF > Clr8BmpInfo->bmiHeader.biWidth)
		currX = Clr8BmpInfo->bmiHeader.biWidth - ImgWdDEF / ampF;
	if (currY + ImgHtDEF / ampF > Clr8BmpInfo->bmiHeader.biHeight)
		currY = Clr8BmpInfo->bmiHeader.biHeight - ImgHtDEF / ampF;
	RedrawImages();
	UpdateData(FALSE);
} //End of OnCbnSelchangeComboAmpf

void CImgDitherDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CRect imgRect;
	imgRect.SetRect(ImgX0DEF, ImgY0DEF,
		ImgX0DEF + ImgWdDEF, ImgY0DEF + ImgHtDEF);
	if (m_CCmbDisp.GetCurSel() == 0 && imgRect.PtInRect(point)) {
		int posX, posY, offset, rowSWd;
		posX = currX + (point.x - ImgX0DEF) / ampF;
		posY = currY + (ImgHtDEF - point.y + ImgY0DEF) / ampF;
		rowSWd = (Clr24BmpInfo->bmiHeader.biWidth*3 + 3) / 4 * 4;
		offset = posY*rowSWd + posX*3;
		UpdateData(TRUE);
		m_VClrB = pOrgBuff[offset];
		m_VClrG = pOrgBuff[offset + 1];
		m_VClrR = pOrgBuff[offset + 2];
		UpdateData(FALSE);
	}; //if 24-bit color
	CDialogEx::OnRButtonDblClk(nFlags, point);
} //End of OnRButtonDblClk


void CImgDitherDlg::OnDeltaposSpinRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta != 1) ipRed-=(ipRed>-9)?1:0;
	else ipRed+=(ipRed<9)?1:0;
	UpdateData(TRUE);
	m_VIpRGB.Format(_T("%d,%d,%d,%d"), ipRed, ipGreen, ipBlue, ipDelta);
	UpdateData(FALSE);
	*pResult = 0;
} //End of OnDeltaposSpinRed

void CImgDitherDlg::OnDeltaposSpinGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta != 1) ipGreen -= (ipGreen>-9) ? 1 : 0;
	else ipGreen += (ipGreen<9) ? 1 : 0;
	UpdateData(TRUE);
	m_VIpRGB.Format(_T("%d,%d,%d,%d"), ipRed, ipGreen, ipBlue, ipDelta);
	UpdateData(FALSE);
	*pResult = 0;
} //End of OnDeltaposSpinGreen

void CImgDitherDlg::OnDeltaposSpinBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta != 1) ipBlue -= (ipBlue>-9) ? 1 : 0;
	else ipBlue += (ipBlue<9) ? 1 : 0;
	UpdateData(TRUE);
	m_VIpRGB.Format(_T("%d,%d,%d,%d"), ipRed, ipGreen, ipBlue, ipDelta);
	UpdateData(FALSE);
	*pResult = 0;
} //End of OnDeltaposSpinBlue

void CImgDitherDlg::OnDeltaposSpinDelta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta != 1) ipDelta -= (ipDelta>0) ? 1 : 0;
	else ipDelta += (ipDelta<9) ? 1 : 0;
	UpdateData(TRUE);
	m_VIpRGB.Format(_T("%d,%d,%d,%d"), ipRed, ipGreen, ipBlue, ipDelta);
	UpdateData(FALSE);
	*pResult = 0;
} //End of OnDeltaposSpinDelta

void CImgDitherDlg::OnBnClickedCheckMark()
{
	UpdateData(TRUE);
	RedrawImages();
	UpdateData(FALSE);
} //End of OnBnClickedCheckMark

void CImgDitherDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect imgRect, clrRect;
	imgRect.SetRect(ImgX0DEF, ImgY0DEF, ImgX0DEF + ImgWdDEF, ImgY0DEF + ImgHtDEF);
	if (imgRect.PtInRect(point)) {
		markX0 = point.x - ImgX0DEF;
		markY0 = point.y - ImgY0DEF;
		imgMoveF = TRUE;
	}; //point is in Image rect
	CDialogEx::OnRButtonDown(nFlags, point);
} //End of OnRButtonDown

void CImgDitherDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (imgMoveF) {
		int tempV;
		imgMoveF = FALSE;
		markX1 = point.x - ImgX0DEF;
		markY1 = point.y - ImgY0DEF;
		if (markX1 < markX0) {
			tempV = markX0;
			markX0 = markX1;
			markX1 = tempV;
		}; //if x1<x0
		if (markY1 < markY0) {
			tempV = markY0;
			markY0 = markY1;
			markY1 = tempV;
		}; //if y1<y0
		if (markX0 < 0) markX0 = 0;
		if (markX1 > ImgWdDEF - 1) markX1 = ImgWdDEF - 1;
		if (markY0 < 0) markY0 = 0;
		if (markY1 > ImgHtDEF - 1) markY1 = ImgHtDEF - 1;
		RedrawImages();
	}; //if image move flag
	CDialogEx::OnRButtonUp(nFlags, point);
} //End of OnRButtonUp

void CImgDitherDlg::OnBnClickedButtonHisto()
{
} //End of OnBnClickedButtonHisto

void CImgDitherDlg::OnBnClickedButtonGo()
{
	// TODO:  在此添加控件通知处理程序代码
} //End of OnBnClickedButtonGo
