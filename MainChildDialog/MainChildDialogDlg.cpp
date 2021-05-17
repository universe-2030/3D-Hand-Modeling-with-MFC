
// MainChildDialogDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MainChildDialog.h"
#include "MainChildDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
const double R2D = 180.0 / M_PI;
const double D2R = 1 / R2D;

int			screenWidth = 1600;
int			screenHeight = 900;
GLFrame		frameCamera;
float		delta = 0;
int			cursorPos[2];
//GLfloat		whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//GLfloat		sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//GLfloat		lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool		cameraControlMode = false;
GLFrame		hand[2];
Hand		rightHand(&hand[0]);
Hand		leftHand(&hand[1]);
int			jointIndex = 0;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CMainChildDialogDlg 대화 상자



CMainChildDialogDlg::CMainChildDialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINCHILDDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainChildDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainChildDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMainChildDialogDlg 메시지 처리기

BOOL CMainChildDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	int argc = 1;
	char* argv[] = {"Hello", "Hello" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(900, 700);
	glutCreateWindow("OpenGL 3D Model Demo");

	SetupRC();
	SetTimer(1000, 10, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMainChildDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMainChildDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();

		wglMakeCurrent(m_hDC, m_hRC);
		RenderScene();
		SwapBuffers(m_hDC);
		wglMakeCurrent(m_hDC, NULL);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMainChildDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainChildDialogDlg::SetupRC()
{
	int iSphere;

	// Bluish background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	// Draw everything as wire frame
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//MODEL = glmReadOBJ("hand.obj");


	//glEnable(GL_LIGHTING);
	// Setup and enable light 0
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Randomly place the sphere inhabitants

	//for (iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
	//{
	//	// Pick a random location between -20 and 20 at .1 increments
	//	float x = ((float)((rand() % 400) - 200) * 0.1f);
	//	float z = (float)((rand() % 400) - 200) * 0.1f;
	//	spheres[iSphere].SetOrigin(x, 0.0f, z);
	//}

	M3DVector3f up = { -0.614193, 0.690447, 0.382165 };
	M3DVector3f origin = { 5.12, 7.09, -5.57 };
	M3DVector3f look = { -0.51938, -0.71827, 0.4629628 };
	frameCamera.SetForwardVector(look);
	frameCamera.SetOrigin(origin);
	frameCamera.SetUpVector(up);
	frameCamera.Normalize();
}

///////////////////////////////////////////////////////////
// Draw a gridded ground
void CMainChildDialogDlg::DrawGround(void)
{
	const GLfloat fExtent = 100.0f;
	const GLfloat fStep = 0.5f;
	GLfloat y = -0.4f;
	GLfloat iLine;

	glLineWidth(1);
	glColor3ub(0, 150, 150);
	glBegin(GL_LINES);
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, y, fExtent);    // Draw Z lines
		glVertex3f(iLine, y, -fExtent);

		glVertex3f(fExtent, y, iLine); // Draw X lines
		glVertex3f(-fExtent, y, iLine);
	}
	glEnd();

	//gltDrawUnitAxes();
}

// Called to draw scene
void CMainChildDialogDlg::RenderScene(void)
{
	//frameCamera.Normalize();
	//frametorus.Normalize();
	//cout << "Camera Position: (" << frameCamera.GetOriginX() << ", " << frameCamera.GetOriginY() << ", " << frameCamera.GetOriginZ() << ")" << endl;
	//M3DVector3f tmp;
	//frameCamera.GetForwardVector(tmp);
	//cout << "Camera Lookat: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	frameCamera.ApplyCameraTransform();

	// Draw the ground
	DrawGround();
	glTranslatef(0, 1, 0);
	//??짫╁ㅦ1?ⓒΞglBegin() ... glEnd()⒡첩퍀ず큍?か톱
#pragma region PolygonObject
	glPushMatrix();
	const float vertices[8][3] = { { -1, -1, -1 },
									{ 1, -1, -1 },
									{ 1, 1, -1 },
									{ -1, 1, -1 },
									{ -1, -1, 1 },
									{ 1, -1, 1 },
									{ 1, 1, 1 },
									{ -1, 1, 1 } };
	glTranslatef(1, 0, 1);
	//glScalef(0.1, 0.1, 0.1);
	glColor3ub(255, 0, 255);

#pragma region PolygonObject_Solid

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(.5, .5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Back
	glColor3ub(0, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glEnd();

	//Front
	glColor3ub(255, 0, 255);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glEnd();

	//Top
	glColor3ub(0, 0, 255);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glEnd();

	//Bottom
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);
	glEnd();

	//Left
	glColor3ub(0, 255, 0);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[4]);
	glEnd();

	//Right
	glColor3ub(255, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[5]);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
#pragma endregion

#pragma region PolygonObject_Wire
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glEnd();

	//Front
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glEnd();

	//Top
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glEnd();

	//Bottom
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[5]);
	glEnd();

	//Left
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[4]);
	glEnd();

	//Right
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[5]);
	glEnd();
#pragma endregion

	glPopMatrix();
#pragma endregion


	//쬞Ξ셕?glutWire*()큍?か톱좥첩퍀쩧╁ㅦⓦ┳짽?츙?ずか톱
#pragma region JointObject
	glPushMatrix();
	//glRotatef(90, 0, 0, 1);
	rightHand.Render();
	glPopMatrix();
#pragma endregion

	glPopMatrix();

	// Do the buffer Swap
	glutSwapBuffers();
}

void CMainChildDialogDlg::objectAnimate(int i)
{
	if (i < 50)
	{
		for (int j = 0; j < 14; j++)
		{
			rightHand.setJointIndex(j);
			rightHand.fingerRotateDown();
		}
	}
	else if (i >= 50 && i < 60)
	{
		hand[0].MoveForward(-0.3);
	}
	else if (i >= 60 && i < 70)
	{
		hand[0].MoveForward(0.3);
	}
	else
	{
		for (int j = 0; j < 14; j++)
		{
			rightHand.setJointIndex(j);
			rightHand.fingerRotateUp();
		}
	}
}
//void bf(int i)
//{
//	/*
//	Your Implementation
//	*/
//
//	const double rad = 3.14159265359 / 180;
//	const float dist = sqrtf(pow(frameCamera.GetOriginX() - frametorus.GetOriginX(), 2) + pow(frameCamera.GetOriginZ() - frametorus.GetOriginZ(), 2));
//
//	frameCamera.RotateLocalY(1 * D2R);
//
//	frameCamera.MoveRight(-dist*sin(rad));
//
//	if ((i >= 0 && i < 90) || (i >= 270 && i < 360))
//		frameCamera.MoveForward(dist - dist*cos(rad));
//	else
//		frameCamera.MoveForward(-(dist - dist*cos(rad)));
//
//	if (i < 360)
//		glutTimerFunc(33, bf, ++i);
//
//	glutPostRedisplay();
//}

void CMainChildDialogDlg::myKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		/* Your Implementation */
		//Ex:  frametorus.????
		//hand[0].MoveForward(.1f);
		if (++jointIndex > 14)
			jointIndex = 0;
		rightHand.setJointIndex(jointIndex);

		break;
	case 's':
		/* Your Implementation */
		//hand[0].MoveForward(-.1f);
		if (--jointIndex < 0)
			jointIndex = 14;
		rightHand.setJointIndex(jointIndex);

		break;
	case 'a':
		/* Your Implementation */
		//hand[0].MoveRight(.1f);
		rightHand.fingerRotateUp();
		break;
	case 'd':
		/* Your Implementation */
		//hand[0].MoveRight(-.1f);
		rightHand.fingerRotateDown();
		break;

	case 'b':
		objectAnimate(0);
		break;
	case 'p':
		cout << "Camera Position: (" << frameCamera.GetOriginX() << ", " << frameCamera.GetOriginY() << ", " << frameCamera.GetOriginZ() << ")" << endl;
		M3DVector3f tmp;
		frameCamera.GetForwardVector(tmp);
		cout << "Camera Lookat: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		frameCamera.GetUpVector(tmp);
		cout << "Camera Up: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		break;

	case 'r':
		M3DVector3f up = { -0.614193, 0.690447, 0.382165 };
		M3DVector3f origin = { 5.12, 7.09, -5.57 };
		M3DVector3f look = { -0.51938, -0.71827, 0.4629628 };
		frameCamera.SetForwardVector(look);
		frameCamera.SetOrigin(origin);
		frameCamera.SetUpVector(up);
		frameCamera.Normalize();
		break;

	}
	glutPostRedisplay();
}

// Respond to arrow keys by moving the camera frame of reference
void CMainChildDialogDlg::SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		frameCamera.MoveForward(0.05f);
		hand[0].MoveForward(-0.05f);
	}
	if (key == GLUT_KEY_DOWN)
	{
		frameCamera.MoveForward(-0.05f);
		hand[0].MoveForward(0.05f);
	}
	if (key == GLUT_KEY_LEFT)
	{
		frameCamera.MoveRight(0.05f);
		hand[0].MoveRight(-0.05f);
		//frameCamera.RotateLocalY(0.1f);
	}
	if (key == GLUT_KEY_RIGHT)
	{
		frameCamera.MoveRight(-0.05f);
		hand[0].MoveRight(0.05f);
		//frameCamera.RotateLocalY(-0.1f);
	}
	// Refresh the Window
	glutPostRedisplay();
}

void CMainChildDialogDlg::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		cameraControlMode = true;
	}
	else
	{
		cameraControlMode = false;
	}
}

int rotateCount = 0;
void CMainChildDialogDlg::mouseMotion(int x, int y)
{
	y = screenHeight - y;
	if (cameraControlMode)
	{
		if (x - cursorPos[0] > 2)
			frameCamera.MoveRight(0.1f);
		else if (x - cursorPos[0] < -2)
			frameCamera.MoveRight(-0.1f);



		//cout << "Camera Up: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		if (y - cursorPos[1] > 2 && rotateCount > -30)
		{

			rotateCount--;
			frameCamera.RotateLocalX(-3 * D2R);
			const float dist = sqrtf(pow(frameCamera.GetOriginX() - hand[0].GetOriginX(), 2) +
				pow(frameCamera.GetOriginY() - hand[0].GetOriginY(), 2) +
				pow(frameCamera.GetOriginZ() - hand[0].GetOriginZ(), 2));
			frameCamera.MoveUp(-dist * sin(3 * D2R));
			frameCamera.MoveForward(-(dist - dist * cosf(3 * D2R)));

		}
		else if (y - cursorPos[1] < -2 && rotateCount < 30)
		{

			rotateCount++;
			frameCamera.RotateLocalX(3 * D2R);
			const float dist = sqrtf(pow(frameCamera.GetOriginX() - hand[0].GetOriginX(), 2) +
				pow(frameCamera.GetOriginY() - hand[0].GetOriginY(), 2) +
				pow(frameCamera.GetOriginZ() - hand[0].GetOriginZ(), 2));
			frameCamera.MoveUp(dist * sin(3 * D2R));
			frameCamera.MoveForward(dist - dist * cosf(3 * D2R));
		}

	}
	M3DVector3f camera;
	M3DVector3f object;
	frameCamera.GetOrigin(camera);
	hand[0].GetOrigin(object);
	M3DVector3f look = { object[0] - camera[0], object[1] - camera[1], object[2] - camera[2] };
	frameCamera.SetForwardVector(look);
	frameCamera.Normalize();

	cursorPos[0] = x;
	cursorPos[1] = y;

	//cout << "Cursor: " << cursorPos[0] << "," << cursorPos[1] << endl;
}

void CMainChildDialogDlg::mouseScroll(int button, int dir, int x, int y)
{
	if (dir > 0)
		frameCamera.MoveForward(0.2f);
	else
		frameCamera.MoveForward(-0.2f);
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)


void CMainChildDialogDlg::ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	screenWidth = w;
	screenHeight = h;

	glViewport(0, 0, screenWidth, screenHeight);

	fAspect = (GLfloat)screenWidth / (GLfloat)screenHeight;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(70.0f, fAspect, 0.1f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CMainChildDialogDlg::OnDestroy() {
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}


void CMainChildDialogDlg::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	if (++jointIndex > 14)
		jointIndex = 0;
	rightHand.setJointIndex(jointIndex);
	this->Invalidate(FALSE);
}


int CMainChildDialogDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	int nPixelFormat;
	m_hDC = ::GetDC(m_hWnd);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));
	m_hRC = wglCreateContext(m_hDC);
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	wglMakeCurrent(NULL, NULL);

	return 0;
}


void CMainChildDialogDlg::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	ChangeSize(cx, cy);
	VERIFY(wglMakeCurrent(NULL, NULL));
}


BOOL CMainChildDialogDlg::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}
