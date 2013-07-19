#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>

#include "FileInOut.h"
using namespace std;
using namespace cv;
//#define debugMode

#define featureDimension 128

vector<int> infromstring(string s)
{
	vector<int> rslt;
	rslt.clear();
	FILE* fp=fopen(s.c_str(),"r");
	int tn;
	fscanf(fp,"%d\n",&tn);
	rslt.resize(tn,0);
	for (int i = 0; i < tn; i++)
	{
		fscanf(fp,"%d\n",&rslt[i]);
	}
	fclose(fp);
	return rslt;
}

class featype
{
	public:
	featype()
	{

	};
	Point pos;
	double feature[featureDimension];

	static void initOne(FILE* fp,featype &t)
	{
		fscanf(fp,"%d %d\n",&t.pos.x,&t.pos.y);
		for (int i=0;i<featureDimension;i++)
		{
			fscanf(fp,"%lf ",&t.feature[i]);
		}
	};
	static void printOne(FILE* fp, featype t)
	{
		fprintf(fp,"%d %d\n",t.pos.x,t.pos.y);
		for (int i=0;i<featureDimension;i++)
		{
			fprintf(fp,"%lf ",t.feature[i]);
		}
	};

};


void kptstoNumKpFea(Mat iptimg, int thr, int stp,string s)
{
	vector<KeyPoint> temkpts;
	char tm[40];
	sprintf(tm,"%s_bt%d_stp%d_kpts.txt",s.c_str(),thr,stp);

	FILE* ft;
	ft=fopen(tm,"r");
	int tnn;
	
	fscanf(ft,"%d\n",&tnn);
	KeyPoint tkpt;
	//tnn=10;
	temkpts.resize(tnn,tkpt);
	for (int i = 0; i < tnn; i++)
	{
		int tmx,tmy;
		fscanf(ft,"%d %d\t",&tmx,&tmy);
		temkpts[i].pt.x=tmx;
		temkpts[i].pt.y=tmy;
		temkpts[i].size=stp;
	}
	fclose(ft);

	
	SiftDescriptorExtractor extractor;

	Mat descriptor;


	extractor.compute(iptimg,temkpts,descriptor);

	vector<featype> feas;
	feas.clear();
	for (auto i=0;i<descriptor.rows;i++)
	{
		featype temfes;

		temfes.pos.x=(int)temkpts[i].pt.x;
		temfes.pos.y=(int)temkpts[i].pt.y;
		Scalar ss(255,0,0);
		
		//temfes.feature.resize(128,0.0);
		double temsum(0.000001);
		for (auto j=0;j<descriptor.cols;j++)
		{
			temfes.feature[j]=descriptor.at<float>(i,j);
			temsum+=temfes.feature[j];
		}
		for (auto j=0;j<descriptor.cols;j++)
		{
			temfes.feature[j]/=temsum;
	
		}
	
	
		feas.push_back(temfes);
	}
	char tD[40];
	sprintf(tD,"%s_bt%d_stp%d",s.c_str(),thr,stp);
	string mys(tD);
	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"w");
	fprintf(fp,"%d",temkpts.size());
	fclose(fp);
	fp=fopen((mys+"_fpts.txt").c_str(),"w");
	for (int i = 0; i < temkpts.size(); i++)
	{
		fprintf(fp,"%d %d\n",temkpts[i].pt.x,temkpts[i].pt.y);
	}
	fclose(fp);
	fp=fopen((mys+"_feas.txt").c_str(),"w");
	for (int i = 0; i < feas.size(); i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fprintf(fp,"%lf ",feas[i].feature[j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	
	

}

void main(int argc, char* argv[])
{
	string s;
	
//	s=argv[1];
	
	_chdir("E:\\CarData\\voc2007\\training\\car");
	
	s="001119";
	Mat iptimg=imread(s+".jpg");


	vector<int> thrs=infromstring("..\\..\\bndThreshld.txt");

	vector<int> kspt=infromstring("..\\..\\kptStep.txt");

	for (int i = 0; i < thrs.size(); i++)
	{
		for (int j = 0; j < kspt.size(); j++)
		{
			kptstoNumKpFea(iptimg,thrs[i],kspt[j],s);
		}
	}

	
}

