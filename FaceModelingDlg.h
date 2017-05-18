// FaceModelingDlg.h : header file
//
#include "cv.h"
#include "highgui.h"
#include "AAM_Basic.h"
#pragma once


// CFaceModelingDlg dialog
class CFaceModelingDlg : public CDialog
{
// Construction
public:
	CFaceModelingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FACEMODELING_DIALOG };

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
	CString WorkPath;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonModel();
	IplImage* TheImage;
	void ShowImg(IplImage* img);
	void ResizeImg(IplImage* img);
	AAM_Pyramid Fitmodel;
	afx_msg void OnBnClickedButtonShow();
	void AAMPisitioning(IplImage* TheImage);
	afx_msg void OnBnClickedButtonImg();
};
