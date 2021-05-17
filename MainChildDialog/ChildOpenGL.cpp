// ChildOpenGL.cpp: 구현 파일
//

#include "pch.h"
#include "MainChildDialog.h"
#include "ChildOpenGL.h"
#include "afxdialogex.h"


// ChildOpenGL 대화 상자

IMPLEMENT_DYNAMIC(ChildOpenGL, CDialogEx)

ChildOpenGL::ChildOpenGL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHILD_OPENGL, pParent)
{

}

ChildOpenGL::~ChildOpenGL()
{
}

void ChildOpenGL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChildOpenGL, CDialogEx)
END_MESSAGE_MAP()


// ChildOpenGL 메시지 처리기
