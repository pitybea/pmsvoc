#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>
using namespace std;
#include <fstream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib") 
#pragma comment(lib,"ml.lib") 
#pragma comment(lib,"highgui.lib")  

char wndname[] = "Edge";

char tbarname[] = "Threshold";
int edge_thresh = 60;

vector<vector<double> > values;

IplImage *image = 0, *edge = 0, *sh=0;


vector<vector<int> > rects;

void on_trackbar(int h)
{
	cvZero(sh);
	cvSetImageROI(sh,cvRect(0,0,image->width,image->height));
	cvAdd(sh,image,sh,NULL);
	cvSetImageROI(sh,cvRect(image->width,0,image->width,image->height));
	for (int i=0;i<image->height;i++)
	{
		for (int j=0;j<image->width;j++)
		{
			if (values[i][j]>edge_thresh*0.001)
			{
				CvScalar s;
				s.val[0]=255;
				s.val[1]=255;
				s.val[2]=255;
			//	cout<<"("<<i<<","<<j<<")";
				cvSet2D(edge,i,j,s); 
			}
			else
			{
				CvScalar s;
				s.val[0]=0;
				s.val[1]=0;
				s.val[2]=0;
				cvSet2D(edge,i,j,s); 
			}
		}
	}
	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i][4]==1)
		{
			cvRectangle(sh,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,0,0),2,8,0);
		}
		else
		cvRectangle(sh,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,255,0),2,8,0);
	}
	
	cvAdd(sh,edge,sh,NULL);

	cvResetImageROI(sh);

	cvShowImage(wndname,sh);

}

int main(int argc,char* argv[])
{

	//D:\ethzshAllAngle\0
	_chdir("E:\\carData\\voc2007\\testing\\car");
	char tD[20];
	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		sprintf_s(tD,"000004");
	}

	

	char tDf[20];
	char tDi[20];
	char tDa[20];

//	sprintf_s

	sprintf_s(tDf,"%s_edge.txt",tD);
	sprintf_s(tDi,"%s.jpg",tD);
	sprintf_s(tDa,"%s_objs.txt",tD);

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);

	vector<vector<int> > v_;

//	values=new double*[heit];
	values.resize(heit,vector<double>(wid,0.0));
	
	v_.resize(heit,vector<int>(wid,0));

	for (int i=0;i<heit;i++)
	{
		//values[i]=new double[wid];
		for (int j=0;j<wid;j++)
		{
			double temt;
			fscanf_s(fp,"%lf ",&temt);
			values[i][j]=temt;

		}
		fscanf_s(fp,"\n");
	}
	fclose(fp);

	fp=fopen(tDa,"r");
	int objn;
	fscanf_s(fp,"%d\n",&objn);

	rects.resize(objn,vector<int>(5,0));
	for (int i = 0; i < objn; i++)
	{
	//	int ti;
		string ts;
		fscanf_s(fp,"%d %d %d %d %d %s\n",&rects[i][0],&rects[i][1],&rects[i][2],&rects[i][3],&rects[i][4],&ts);
	}

	fclose(fp);

	// (Xmin, Ymin) - (Xmax, Ymax) : (292, 115) - (381, 406)


	

	if( (image = cvLoadImage(tDi, 1)) == 0 )
		return -1;

	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i][4]==1)
		{
			cvRectangle(image,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,0,0),2,8,0);
		}
		else
		cvRectangle(image,cvPoint(rects[i][0],rects[i][1]),cvPoint(rects[i][2],rects[i][3]),cvScalar(255,255,0),2,8,0);
	}

	sh = cvCreateImage(cvSize(image->width*2,image->height), IPL_DEPTH_8U, 3);
	edge = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 3);
	cvNamedWindow(wndname, 1);
	cvCreateTrackbar(tbarname, wndname, &edge_thresh, 1000, on_trackbar);

	on_trackbar(60);
	// Create a window


	cvWaitKey(0);
	cvReleaseImage(&image);
	cvReleaseImage(&edge);
	cvReleaseImage(&sh);

	cvDestroyWindow(wndname);

	return 0;
}