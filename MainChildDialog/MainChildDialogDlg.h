// MainChildDialogDlg.h: 헤더 파일
//

#pragma once
#include <iostream>
#include <gltools.h>
#include <glframe.h>
#include "GL/glew.h"
#include "glm.h"
#include "Hand.h"
#include "MainChildDialogDlg_Child.h"

// CMainChildDialogDlg 대화 상자
class CMainChildDialogDlg : public CDialogEx {
// 생성입니다.
public:
	CMainChildDialogDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINCHILDDIALOG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

private:
	HGLRC	m_hRC;
	CDC* m_pDC;
	HDC		m_hDC;

	MatchDevice* MATCH_Dev;
	double* Euler_shoulder;
	bool isMATCHconnected = FALSE;

public:
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	MainChildDialogDlg_Child* p_ChildDlg;

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void SetupRC();
	void DrawGround();
	void RenderScene();
	void objectAnimate(int i);

	void myKeys(unsigned char key, int x, int y);
	void SpecialKeys(int key, int x, int y);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mouseScroll(int button, int dir, int x, int y);
	void ChangeSize(int w, int h);
};