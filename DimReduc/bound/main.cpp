#include <stdio.h>
#include <Windows.h>
#include <direct.h>

#include <dos.h>
#include <conio.h>

#include <iostream>
using namespace std;
#include <fstream>
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "..\..\common\VecDeal.h"
#include "..\..\common\detectCommon.h"



int main(int argc,char* argv[])
{
	//
	char tD[40];


	if (argc>1)
	{
		sprintf_s(tD,"%s",argv[1]);

	}
	else
	{
		_chdir("E:\\carData\\voc2007\\transfer");
		sprintf_s(tD,"000012_obj_0");
	
	}

	vector<vector<double> > pcamatrix;
	pcamatrix.clear();
	pcamatrix.resize(64,vector<double>(63,0.0));
	FILE* fp=fopen("pcaMatrix.txt","r");
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 63; j++)
		{
			fscanf(fp,"%lf ",&pcamatrix[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);

	string s(tD);

	int tnum;
	fp=fopen((s+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
	fclose(fp);

	vector<vector<double> >feas;
	feas.clear();
	feas.resize(tnum,vector<double> (featureDimension,0.0));

	fp=fopen((s+"_feas.txt").c_str(),"r");
	for (int i = 0; i < tnum; i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fscanf(fp,"%lf ",&feas[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);

	ZeroMnVec(feas);

	vector<vector<double> > opp=TransitMtx(feas,pcamatrix);

	fp=fopen((s+"_dimrd.txt").c_str(),"w");
	for (int i = 0; i < opp.size(); i++)
	{
		for (int j = 0; j < opp[i].size(); j++)
		{
			fprintf(fp,"%lf ",opp[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);


	return 0; 
}