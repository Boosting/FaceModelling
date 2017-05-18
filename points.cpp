#include "stdafx.h"
#include "points.h"

using namespace std;

void Points::readmeanshape(CvMat*meanshape)
{
	FILE *MS;
	char filename[200];
	strcpy(filename,filepath);
	strcat(filename,"\\3D_Matrix\\meanshape.txt");
	MS=fopen(filename,"r");
	for(int i=0; i<meanshape->height; i++)
	{
		float t1;
		fscanf(MS,"%f",&t1);
		cvmSet(meanshape, i, 0, t1);
	}
	fclose(MS);

}
void Points::readkp(char*filename,CvMat*m_kp)
{	
	FILE *fkp;
	fkp=fopen(filename,"r");
	if (model3d)
	{
		for (int i=0; i<m_kp->height; i++)
		{
			float t1, t2,t3;
			fscanf(fkp,"%f %f %f", &t1, &t2,&t3);
			cvmSet(m_kp, i, 0, t1);
			cvmSet(m_kp, i, 1, t2);
			cvmSet(m_kp, i, 2, t3);
		}
	} 
	else
	{
		for (int i=0; i<33; i++)
		{
			float t1, t2;
			fscanf(fkp,"%f %f", &t1, &t2);
			cvmSet(m_kp, i, 0, t1);
			cvmSet(m_kp, i, 1, t2);
		}
	}
	fclose(fkp);

}
void Points::readeigenvec(CvMat* eigenvector)
{
	FILE *feigV;
	char filename[200];
	strcpy(filename,filepath);
	strcat(filename,"\\3D_Matrix\\eigenvector.txt");
	feigV=fopen(filename,"r");
	for (int i=0; i<eigenvector->height; i++)
	{
		for (int j=0; j<eigenvector->width; j++)
		{
			float temp;
			fscanf(feigV, "%f", &temp);
			cvmSet(eigenvector, i, j, temp);
		}
	}
	fclose(feigV);

}

void Points::Centralize(CvMat *Shapes)
{
	int row=Shapes->rows;
	int col=Shapes->cols;
	CvMat *avg=cvCreateMat(1, col, CV_32FC1);
	cvZero(avg);

	for (int j=0; j<col; j++)
	{
		float temp=0.0;

		for (int i=0; i<row; i++)
		{
			temp+=cvmGet(Shapes, i, j);			
		}
		cvmSet(avg, 0, j, temp/row);
	}

	for (int i=0; i<row; i++)
	{
		for (int j=0; j<col; j++)
		{
			cvmSet(Shapes, i, j, cvmGet(Shapes, i, j)-cvmGet(avg, 0, j));
		}
	}

}

void Points::AlignTo(CvMat*ref,CvMat*mat)
{
	if (model3d)
	{
		CvMat *mat_t=cvCreateMat(mat->width, mat->height, CV_32FC1);

		CvMat*mat_rot=cvCreateMat(3,3,CV_32FC1);

		CvMat*U=cvCreateMat(3,3,CV_32FC1);
		CvMat*V=cvCreateMat(3,3,CV_32FC1);

		CvMat*mat_cross=cvCreateMat(3,3,CV_32FC1);
		cvTranspose(mat,mat_t);
		cvMatMul(mat_t,ref,mat_cross);

		CvMat*S=cvCreateMat(3,3,CV_32FC1);
		cvSVD(mat_cross,S,U,V);
		double det=cvDet(mat_cross);
		CvMat*mat_I=cvCreateMat(3,3,CV_32FC1);
		cvZero(mat_I);

		for (int j=0; j<3; j++)
		{
			cvmSet(mat_I, j, j, 1);
		}
		if (det<=0)
		{
			cvmSet(mat_I, 2, 2, -1);
		}

		cvMatMul(U,mat_I,mat_rot);
		cvTranspose(V,V);
		cvMatMul(mat_rot, V, mat_rot);

		cvMatMul(mat, mat_rot, mat);
	} 
	else
	{
		double a, b, tx, ty;

		AlignTransformation(ref, mat, a, b, tx, ty);

		//旋转
		Transform(mat, a, -b, b, a);
		//平移
		Translate(mat, tx, ty);
	}

}

void Points::AlignTransformation(CvMat* ref_mat, CvMat*mat,double &a, double &b, double &tx, double &tz)
{
	int row=mat->rows;
	int col=mat->cols;

	double X1 = 0, Y1 = 0, X2 = 0, Y2 = 0,  Z = 0, C1 = 0, C2 = 0;
	double W = row;
	double x1, y1, x2, y2;

	for(int i=0; i < row; i++)
	{
		x1 = cvGetReal2D(ref_mat, i, 0);
		y1 = cvGetReal2D(ref_mat, i, 1);

		x2 = cvGetReal2D(mat, i, 0);
		y2 = cvGetReal2D(mat, i, 1);

		X1 += x1;
		Y1 += y1;
		X2 += x2;
		Y2 += y2;
		Z  += x2 * x2 + y2 * y2;

		C1 += x1 * x2 + y1 * y2;
		C2 += y1 * x2 - x1 * y2;

	}

	double SolnA[] = {X2, -Y2, W, 0, Y2, X2, 0, W, Z, 0, X2, Y2, 0, Z, -Y2, X2};
	CvMat A = cvMat(4, 4, CV_64FC1, SolnA);
	double SolnB[] = {X1, Y1, C1, C2};
	CvMat B = cvMat(4, 1, CV_64FC1, SolnB);

	static CvMat* Soln = cvCreateMat(4, 1, CV_64FC1);
	cvSolve(&A, &B, Soln, CV_SVD);

	a	= CV_MAT_ELEM(*Soln, double, 0, 0);  b	= CV_MAT_ELEM(*Soln, double, 1, 0);
	tx	= CV_MAT_ELEM(*Soln, double, 2, 0);	 tz	= CV_MAT_ELEM(*Soln, double, 3, 0);
}


void Points::Translate(CvMat* mat, double tx, double ty)
{
	for(int i=0; i<mat->height; i++)
	{
		float x=cvmGet(mat, i, 0);
		float y=cvmGet(mat, i, 1);
		x=x+tx;
		y=y+ty;
		cvmSet(mat, i, 0, x);
		cvmSet(mat, i, 1, y);
	}
}

void Points::Transform(CvMat* mat, double c00, double c01, double c10, double c11)
{
	for (int i=0; i<mat->height; i++)
	{
		float x=cvmGet(mat, i, 0);
		float y=cvmGet(mat, i, 1);
		cvmSet(mat, i, 0, c00*x+c01*y);
		cvmSet(mat, i, 1, c10*x+c11*y);
	}

}


void Points::calcaplha(CvMat*eigenvec, CvMat *shape,CvMat*meanshape,CvMat*alpha)
{
	int height_eig=eigenvec->height;
	int width_eig=eigenvec->width;
	int height_sh=shape->height;
	int width_sh=shape->width;
	CvMat *tran_eig=cvCreateMat(width_eig, height_eig,CV_32FC1);
	cvTranspose(eigenvec, tran_eig);//trans(p)

	CvMat* multran_eig=cvCreateMat(width_eig, width_eig, CV_32FC1);
	cvMulTransposed(eigenvec, multran_eig, 1);//trans(p)*p

	CvMat *eta=cvCreateMat(multran_eig->height,multran_eig->width, CV_32FC1);
	cvZero(eta);
	CvMat *S=cvCreateMat(multran_eig->height, multran_eig->width, CV_32FC1);
	cvSVD(multran_eig, S);
	for (int i=0; i<multran_eig->width; i++)
	{
		float s=cvmGet(S, i, i);
		float temp=0.0001/cvmGet(S, i, i);
		cvmSet(eta, i, i, temp);
	}

	cvAdd(multran_eig, eta, multran_eig);
	cvInvert(multran_eig, multran_eig);

	CvMat* res1=cvCreateMat(width_eig, height_eig, CV_32FC1);
	cvMatMul(multran_eig, tran_eig, res1);

	CvMat*delS=cvCreateMat(width_sh*height_sh, 1, CV_32FC1);

	for (int i=0; i<height_sh; i++)
	{
		for (int j=0; j<width_sh; j++)
		{
			float temp=cvmGet(shape, i, j)-cvmGet(meanshape, i, j);
			cvmSet(delS, i*width_sh+j, 0, temp);
		}
	}

	cvMatMul(res1, delS, alpha);

}



void Points::readIndex(CvMat *Index)
{
	/************读取索引矩阵******************************/
	FILE *f_index;
	char filename[200];
	strcpy(filename,filepath);
	strcat(filename,"\\3D_Matrix\\index.txt");
	f_index=fopen(filename,"r");
	for(int i=0;i<51; i++)
	{
		for (int j=0; j<3; j++)
		{
			int t;
			fscanf(f_index, "%d", &t);
			cvmSet(Index, i, j, t);
		}
	}
	fclose(f_index);

}
void Points::readRand(CvMat* Rand)
{
	FILE *f_rand;
	char filename[200];
	strcpy(filename,filepath);
	strcat(filename,"\\3D_Matrix\\cor.txt");
	f_rand=fopen(filename,"r");
	for (int i=0; i<51; i++)
	{
		for (int j=0; j<100;j++)
		{
			for (int s=0; s<3; s++)
			{
				float temp;
				fscanf(f_rand, "%f", &temp);
				cvmSet(Rand, i*100+j, s, temp);
			}
		}
	}
	fclose(f_rand);
}
//依据2D图片的33点的特征点坐标，对3D模型的(x,y)部分进行修正，（0至32的点直接拿过来）,插值出其余的点(33至5132的点)
void Points::GetsubKP(CvMat* mat_kp, CvMat*index, CvMat*Rand, CvMat* res_kp)
{

	for (int i=0; i<33; i++)//对3D模型的(x,y)部分进行修正,（0至32的点直接拿过来）
	{
		cvmSet(res_kp, i, 0, cvmGet(mat_kp, i, 0) );
		cvmSet(res_kp, i, 1, cvmGet(mat_kp, i, 1) );
	}

	for (int i=0; i<index->height; i++)//每三个元素插值出100个点,插值出其余的点(33至5132的点)
	{	
		for (int s=0;s<100;s++)
		{		
			for(int j=0; j<2; j++)
			{
				float temp=0;
				for (int t=0; t<3; t++)
				{
					int i_temp=cvmGet(index, i, t);//取出index的每行元素序号
					temp+=cvmGet(mat_kp, i_temp, j)*cvmGet(Rand, i*100+s, t);//根据权值系数确定新点的坐标值
				}
				cvmSet(res_kp, 33+i*100+s, j, temp );
			}
		}		
	}
}
