
// ImgDitherDlg.h : header file
//

#pragma once
#include "afxcolorbutton.h"
#include "afxwin.h"
#include "afxcmn.h"

#define ImgX0DEF 24
#define ImgY0DEF 24
#define ImgWdDEF 512
#define ImgHtDEF 512
#define ClrX0DEF 580
#define ClrY0DEF 234
#define ClrUWdDEF 15
#define ClrUHtDEF 15
#define HistoX0DEF 580
#define HistoY0DEF 328
#define HistoWdDEF 14
#define HistoHtDEF 70
#define HMarkHtDEF 6
#define MaxColorDEF 160
#define SheetWdDEF 32000
#define SheetHtDEF 2048

// CImgDitherDlg dialog
class CImgDitherDlg : public CDialogEx
{
// Construction
public:
	CImgDitherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IMGDITHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int currIndex,currX,currY;
	int oldX, oldY,ampF;
	int markX0, markY0, markX1, markY1;
	BOOL imgMoveF;
	BITMAPINFO* Clr24BmpInfo;
	BYTE *pOrgBuff;
	BITMAPINFO* Clr8BmpInfo;
	BYTE *pTgtBuff;
	CString m_VSrcFile;
	CMFCColorButton m_CClrBt;
	int m_VClrR,m_VClrG,m_VClrB;
	CComboBox m_CCmbColor,m_CCmbDMode;
	CComboBox m_CCmbSub;
	CComboBox m_CCmbDisp,m_CCmbAmp;
	CString m_VIpRGB;
	CSpinButtonCtrl m_CSpinRed, m_CSpinGreen, m_CSpinBlue;
	CSpinButtonCtrl m_CSpinDelta;
	int ipRed, ipGreen, ipBlue, ipDelta;
	void SetColorSetting(int factor);
	void DrawAHistogram(int hIndex);
	void RedrawHistogram();
	void DrawABlkOfColor(int cIndex);
	void RedrawColors();
	void RedrawImages();
	int FindTheBestColor(int r, int g, int b);
	void DitherRow();
	void DitherColumn();
	void DitherAverage();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedMfccolorbuttonSet();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDither();
	afx_msg void OnCbnSelchangeComboDisplay();
	afx_msg void OnCbnSelchangeComboAmpf();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDeltaposSpinRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinBlue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDelta(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_VChkMarkF;
	CComboBox m_CCmbIpMode;
	afx_msg void OnBnClickedCheckMark();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonHisto();
	afx_msg void OnBnClickedButtonGo();
};
