// FaceModelingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FaceModeling.h"
#include "FaceModelingDlg.h"
#include "cv.h"
#include "highgui.h"
#include "points.h"
#include <fstream>
#include <Windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "limits.h"
#include <direct.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HDC hDC;
CRect rect;
BOOL AAMReadData = FALSE;
int readmodel = 0;
char featurefile[200];
char featurefile1[200];
char featurefile2[200];
char featurefile3[200];
char dyfile[200];
char bmpfile[200];
char imgname[200];
//char imgname1[200]="C:\\Users\\Administrator\\Desktop\\南京抢劫案\\早年.jpg";
//char imgname2[200]="C:\\Users\\Administrator\\Desktop\\南京抢劫案\\正面图1.jpg";
//char imgname3[200]="C:\\Users\\Administrator\\Desktop\\南京抢劫案\\早年照片.jpg";
char imgname1[200];
char imgname2[200];
char imgname3[200];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

									
// CFaceModelingDlg dialog




CFaceModelingDlg::CFaceModelingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFaceModelingDlg::IDD, pParent)
	, WorkPath(_T(""))
	, TheImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceModelingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFaceModelingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFaceModelingDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_MODEL, &CFaceModelingDlg::OnBnClickedButtonModel)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CFaceModelingDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_IMG, &CFaceModelingDlg::OnBnClickedButtonImg)
END_MESSAGE_MAP()


// CFaceModelingDlg message handlers

BOOL CFaceModelingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CStatic *pStc =(CStatic *)GetDlgItem(IDC_STATIC_FRAME);
	pStc->GetClientRect(&rect);
	CDC *pDC = pStc->GetDC();
	hDC = pDC->GetSafeHdc();

	//当前路径获取
	GetModuleFileName(NULL,WorkPath.GetBuffer(MAX_PATH),MAX_PATH);   
	WorkPath.ReleaseBuffer();
	int nPos = WorkPath.ReverseFind('\\');
	WorkPath = WorkPath.Left(nPos);
	if(strstr(WorkPath, "Debug") != NULL||strstr(WorkPath, "Release") != NULL)
	{
		int nPos1 = WorkPath.ReverseFind('\\');
		WorkPath = WorkPath.Left(nPos1);
	}
	
	strcpy(imgname1,WorkPath);
	strcat(imgname1,"\\南京抢劫案\\早年.jpg");
	//imgname1[0] = imgname1[0] - 32;
	strcpy(imgname2,WorkPath);
	strcat(imgname2,"\\南京抢劫案\\正面图1.jpg");
	//imgname2[0] = imgname2[0] - 32;
	strcpy(imgname3,WorkPath);
	strcat(imgname3,"\\南京抢劫案\\早年照片.jpg");
	//imgname3[0] = imgname3[0] - 32;

	
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CFaceModelingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFaceModelingDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFaceModelingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFaceModelingDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	CString filter = "设备无关位图(*.bmp)|*.bmp|图片文件(*.jpg)|*.jpg||";
	CFileDialog aDlg(true,NULL,NULL,OFN_HIDEREADONLY,filter);
	if( aDlg.DoModal() == IDOK )
	{
		CString File = aDlg.GetPathName();
		strcpy(bmpfile,File);
		strcpy(imgname,bmpfile);
		TheImage = cvLoadImage(File);
		//ResizeImg(TheImage);
		//ShowImg(TheImage);
		CvvImage cImag;
		cImag.CopyOf(TheImage);
		cImag.DrawToHDC(hDC,&rect);
		UpdateData(false);
	}

}



//  generate   3D reconstructed face 
void CFaceModelingDlg::OnBnClickedButtonModel()
{
	// TODO: Add your control notification handler code here
	/**********************************************/
	/*********读取AAM训练数据**********************/
	/**********************************************/
	char filename[200];
	strcpy(filename, WorkPath);
	strcat(filename, "\\matrix\\aam.amf");
	if (AAMReadData==FALSE)
	{
		Fitmodel.ReadModel(filename);
		readmodel = 1;
		//MessageBox("AAM done!");
		AAMReadData=TRUE;
	} 

	//**特征点检测***//
	//**FitModel用来做人脸对齐
	AAMPisitioning(TheImage);

	/**********************************************/
	/*********生成三维模型**********************/
	/**********************************************/
	Points points_cloud;
	points_cloud.model3d = false; 
	strcpy(points_cloud.filepath, WorkPath);

	CvMat* meanshape=cvCreateMat(5133*3, 1 , CV_32FC1);
	CvMat* mat_mkp=cvCreateMat(33,2, CV_32FC1);
	CvMat *mat_kp=cvCreateMat(33, 2, CV_32FC1);
	CvMat *eigenvec=cvCreateMat(15399, 33, CV_32FC1);
	CvMat* mat_eig=cvCreateMat(66,33,CV_32FC1);
	CvMat *alpha=cvCreateMat(mat_eig->width, 1, CV_32FC1);
	CvMat* newshape=cvCreateMat(eigenvec->height, 1, CV_32FC1);
	CvMat *Index=cvCreateMat(51, 3, CV_32FC1);
	CvMat *Rand=cvCreateMat(5100, 3, CV_32FC1);
	CvMat *res_kp=cvCreateMat(5133, 2, CV_32FC1);

	/********读取平均人脸****************************/
	points_cloud.readmeanshape(meanshape);

	/*********抽取前33个点的xy坐标*****************/
	for(int i=0; i<33; i++)
	{
		cvmSet(mat_mkp, i, 0, cvmGet(meanshape, i*3, 0) );
		cvmSet(mat_mkp, i, 1, cvmGet(meanshape, i*3+1, 0) );
	}
	/********读取特征向量矩阵********************/

	points_cloud.readeigenvec(eigenvec);
	for (int i=0; i<33; i++)
	{
		for (int j=0; j<33; j++)
		{
			cvmSet(mat_eig, i*2, j, cvmGet(eigenvec, i*3, j) );
			cvmSet(mat_eig, i*2+1, j, cvmGet(eigenvec, i*3+1, j) );
		}
	}

	/********读取索引和随机系数矩阵*******************/

	points_cloud.readIndex(Index);
	points_cloud.readRand(Rand);

	/****读取图片上的人脸特征点坐标*******************/
	// 	char filename[200];
	// 	strcpy(filename, sPath);
	// 	strcat(filename, "\\iter_result.txt");

	if (strcmp(imgname,imgname1)==0)
	{
		points_cloud.readkp(featurefile1, mat_kp);
	}
	else if (strcmp(imgname,imgname2)==0)
	{
		points_cloud.readkp(featurefile2, mat_kp);
	}
	else if (strcmp(imgname,imgname3)==0)
	{
		points_cloud.readkp(featurefile3, mat_kp);
	}
	else
	{
		points_cloud.readkp(featurefile, mat_kp);
	}
	

	/*********计算重心**************/		
	points_cloud.Centralize(mat_kp);

	/********对齐****************/
	points_cloud.AlignTo(mat_mkp, mat_kp);

	/*******采样*****************/
	//依据人脸对齐得到的2D特征点修正3D坐标的(x,y)
	points_cloud.GetsubKP(mat_kp, Index, Rand, res_kp);

	/*******计算系数alpha************************/

	points_cloud.calcaplha(mat_eig, mat_kp, mat_mkp, alpha);

	/***********得到新人脸************************/
	cvMatMul(eigenvec, alpha, newshape);
	cvAdd(newshape, meanshape, newshape);

	//输出
	ofstream file;
	strcpy(dyfile, WorkPath);
	strcat(dyfile, "\\camera1_resized_points.txt");
	file.open(dyfile, ios::trunc);

	for (int i=0; i<newshape->height/3; i++)
	{
		file<<cvmGet(res_kp, i, 0)<<" "<<cvmGet(res_kp, i, 1)<<" ";//x,y point
		file<<cvmGet(newshape, i*3+2, 0);//z point
		file<<endl;
	}

	file.close();


	MessageBox("Model has been done!");

}

void CFaceModelingDlg::ShowImg(IplImage* img)
{
	CDC* pDC = GetDlgItem( IDC_STATIC_FRAME ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	hDC = pDC ->GetSafeHdc();				

	//CRect rect;
	GetDlgItem(IDC_STATIC_FRAME) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// 读取图片的宽和高
	int iw = img->width;	
	int ih = img->height;
	// 使图片的显示位置正好在控件的正中
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// 复制图片
	CvvImage cimg;
	cimg.CopyOf( img );
	// 将图片绘制到显示控件的指定区域内	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );
}

void CFaceModelingDlg::ResizeImg(IplImage* img)
{
	int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float) ( (float) max / 256.0f );

	// 缩放后图片的宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( TheImage, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize( img, TheImage );

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( TheImage );
}


//*********显示三维模型**********************//
void CFaceModelingDlg::OnBnClickedButtonShow()
{
	// TODO: Add your control notification handler code here
	/**********************************************/
	//*********显示三维模型**********************//
	/**********************************************/
	CString input = WorkPath+"\\TextureMap.exe";
	if (strcmp(imgname,imgname1)==0)
	{
		input = input + " " + bmpfile+'$'+featurefile1+'$'+dyfile;
	}
	else if (strcmp(imgname,imgname2)==0)
	{
		input = input + " " + bmpfile+'$'+featurefile2+'$'+dyfile;
	}
	else if (strcmp(imgname,imgname3)==0)
	{
		input = input + " " + bmpfile+'$'+featurefile3+'$'+dyfile;
	}
	else
	{
		input = input + " " + bmpfile+'$'+featurefile+'$'+dyfile;
	}
	
	WinExec( input, SW_SHOW);
	
	// Invalidate(TRUE);
	CvvImage cImag;
	cImag.CopyOf(TheImage);
	cImag.DrawToHDC(hDC,&rect);

	UpdateData(false); 
	Invalidate(TRUE);
	UpdateWindow();
}


void CFaceModelingDlg::AAMPisitioning(IplImage* TheImage)
{
	if (readmodel)
	{
		readmodel=0;
		AAMReadData = FALSE;
		AAM_Shape Shape;
		VJfacedetect facedet;
		char filename1[200];
		strcpy(filename1, WorkPath);
		strcat(filename1, "\\matrix\\haarcascade_frontalface_alt2.xml");
		facedet.LoadCascade(filename1);
		IplImage *image = cvCreateImage(cvSize(TheImage->width, TheImage->height), TheImage->depth, TheImage->nChannels);
		cvCopy(TheImage,image);

		std::vector<AAM_Shape> FaceShape;
		bool flag = Fitmodel.InitShapeFromDetBox(Shape, facedet, image, FaceShape);
		if (!flag)
		{
			MessageBox("No Face! Please choose another image!");
		}
		CvRect FaceRect = cvRect(FaceShape[0][0].x, FaceShape[0][0].y, FaceShape[0][1].x-FaceShape[0][0].x,FaceShape[0][1].y-FaceShape[0][0].y+20);
		char filename[200];
		strcpy(filename,WorkPath);

		Fitmodel.Fit(image, Shape, filename, 30, false);
		Fitmodel.Draw(image, Shape, 0);

		CvvImage cImag;
		cvSetImageROI(image,FaceRect);
		//cvSaveImage("tempface.bmp",image);
		cImag.CopyOf(image);
		cvResetImageROI(image);
		//cImag.DrawToHDC(hDC,&rect);
		//UpdateData(false);

		//output 33 points
		strcpy(featurefile,WorkPath);
		strcat(featurefile,"\\iter_result.txt");
		strcpy(featurefile1,WorkPath);
		strcat(featurefile1,"\\iter_result1.txt");
		strcpy(featurefile2,WorkPath);
		strcat(featurefile2,"\\iter_result2.txt");
		strcpy(featurefile3,WorkPath);
		strcat(featurefile3,"\\iter_result3.txt");
		std::ofstream of1;
		of1.open(featurefile,ios::trunc);
		Shape.MyWrite(of1);
		of1.close();

		// 	FILE* texturefile = fopen(texture_file,"a+");
		// 	fprintf(texturefile,featurefile);
		// 	fprintf(texturefile,"\n");
		// 	fclose(texturefile);
		cvReleaseImage(&image);
	}
}

void CFaceModelingDlg::OnBnClickedButtonImg()
{
	// TODO: Add your control notification handler code here
	CvvImage cImag;
	cImag.CopyOf(TheImage);
	cImag.DrawToHDC(hDC,&rect);
	UpdateData(false); 
}
