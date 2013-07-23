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


int main(int argc,char* argv[])
{
	//
	char tD[40];
	double thresh_edge;

	int bthresh;
	int kptstep;

	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);
		bthresh=atoi(argv[2]);
		kptstep=atoi(argv[3]);
	}
	else
	{
		_chdir("E:\\CarData\\voc2007\\training\\car");
		sprintf_s(tD,"003007");
		bthresh=50;
		kptstep=13;
	}
	char tDf[40];
	//char tDi[40];
	sprintf_s(tDf,"%s_edge.txt",tD);
	

	FILE* fp;
	fp=fopen(tDf,"r");
	int wid,heit;

	fscanf_s(fp,"%d %d\n",&heit,&wid);

	fclose(fp);
	

	vector<vector<int> > intmap;
	intmap.resize(heit,vector<int>(wid,0));
	


	vector<CvPoint> kpts;
	kpts.clear();


	


	char tDi[40];
	
	int kptn;

	FILE* ouF;
	sprintf_s(tDi,"%s_bt%d_stp%d_num.txt",tD,bthresh,kptstep);
	ouF=fopen(tDi,"r");
	fscanf(ouF,"%d\n",&kptn);
	fclose(ouF);


	sprintf_s(tDi,"%s_bt%d_stp%d_fpts.txt",tD,bthresh,kptstep);
	ouF=fopen(tDi,"r");
	
	
	
	CvPoint p;
	kpts.resize(kptn,p);

	for (int i = 0; i < kpts.size(); i++)
	{
		fscanf(ouF,"%d %d\t",&kpts[i].x,&kpts[i].y);
		intmap[kpts[i].y][kpts[i].x]=1;
	}
		
	
	fclose(ouF);

	for (int i = 1; i < heit; i++)
	{
		intmap[i][0]=intmap[i-1][0]+intmap[i][0];
	}
	for (int i = 1; i < wid; i++)
	{
		intmap[0][i]=intmap[0][i-1]+intmap[0][i];
	}
	for (int i = 1; i < heit; i++)
	{
		for (int j = 1; j < wid; j++)
		{
			intmap[i][j]=intmap[i][j]+intmap[i-1][j]+intmap[i][j-1]-intmap[i-1][j-1];
		}
	}



	sprintf_s(tDi,"%s_bt%d_stp%d_kptintg.txt",tD,bthresh,kptstep);
//	FILE* ouF;
	ouF=fopen(tDi,"w");
	
	fprintf(ouF,"%d %d\n",intmap.size(),intmap[0].size());
	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fprintf(fp,"%d ",intmap[i][j]);
		}
		fprintf(fp,"\n");
	}
		
	
	fclose(ouF);
	return 0; 
}