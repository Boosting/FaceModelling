#ifndef POINTS_H
#define POINTS_H

#include <stdio.h>
#include<cv.h>
#include <highgui.h>
#include <vector>
#include<fstream>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>

class Points  
{
public:
	bool model3d;
	char filepath[200];
	void readmeanshape(CvMat*meanshape);
	void readkp(char*filename,CvMat*m_kp);
	void readeigenvec(CvMat* eigenvector);

	void Centralize(CvMat *AllShapes);
	void AlignTo(CvMat*ref,CvMat*mat);
	void AlignTransformation(CvMat* ref_mat, CvMat*mat, double &a, double &b, double &tx, double &ty);
	void Translate(CvMat* mat, double x, double y);
	void Transform(CvMat* mat, double c00, double c01, double c10, double c11);

	void readIndex(CvMat* Index);
	void readRand(CvMat* omiga);
	void GetsubKP(CvMat* mat_kp, CvMat* index, CvMat*Rand, CvMat* res_kp);

	void calcaplha(CvMat *eigenvec, CvMat *shape,CvMat *meanshape,CvMat *alpha);

};

#endif //