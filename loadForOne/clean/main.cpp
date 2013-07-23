
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <map>

#include "FileInOut.h"
using namespace std;

//#define debugMode

#define featureDimension 64

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


template<class T>
pair<vector<vector<int> >, vector<T>> rangequery(int xmin,int ymin,int ymax,int xmax, map<int,map<int,T> > daset)
{
	vector<vector<int> > r1;

	vector<T> rslt;
	rslt.clear();

	auto st=daset.lower_bound (ymin);

	while(st->first<ymax && st!=daset.end() )
	{
		auto st1=st->second.lower_bound(xmin);

		while (st1->first<xmax && st1!=st->second.end() )
		{
			vector<int> tvi(2,0);
			tvi[0]=st1->first;
			tvi[1]=st->first;
			
			r1.push_back(tvi);
			rslt.push_back(st1->second);

			st1++;
		}
		st++;
	}

	return pair<vector<vector<int> >, vector<T>> (r1, rslt);
}


void kptstoNumKpFea( int thr, int stp,string s)
{
	vector<pair<int,int> > temkpts;

	temkpts.clear();

	vector<vector<double> > feas;
	feas.clear();


	
	char tD[40];
	sprintf(tD,"%s_bt%d_stp%d",s.c_str(),thr,stp);
	string mys(tD);

	int tnum;

	FILE *fp;
	fp=fopen((mys+"_num.txt").c_str(),"r");
	fscanf(fp,"%d",&tnum);
	fclose(fp);

	temkpts.resize(tnum,pair<int,int>(0,0));


	fp=fopen((mys+"_fpts.txt").c_str(),"r");
	for (int i = 0; i < temkpts.size(); i++)
	{
		fscanf(fp,"%d %d\n",&temkpts[i].first,&temkpts[i].second);
	}
	fclose(fp);

	feas.resize(tnum,vector<double>(featureDimension,0.0));

	fp=fopen((mys+"_feas.txt").c_str(),"r");
	for (int i = 0; i < feas.size(); i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fscanf(fp,"%lf ",&feas[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);


	map<int,map<int, int > > kptFea;

	kptFea.clear();

	for (int i = 0; i < temkpts.size(); i++)
	{
		int a=temkpts[i].second;
		if (kptFea.count(a)>0)
		{
			int b=temkpts[i].first;
			kptFea[b].insert(pair<int,int>(b,i));
		}
		else
		{
			map<int, int > sth;
			int b=temkpts[i].first;
			sth.insert(pair<int,int>(b,i));
			kptFea.insert(pair<int,map<int, int > >(a,sth));
		}
	}

	auto sth=rangequery(10,0,200,300,kptFea);

	vector<vector<int> > intmap;

	FILE* ouF;
	ouF=fopen((mys+"_kptintg.txt").c_str(),"r");
	
	int heit,wid;
	fscanf(ouF,"%d %d\n",&heit,&wid);
	intmap.resize(heit,vector<int>(wid,0));

	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fscanf(fp,"%d ",&intmap[i][j]);
		}
		fscanf(fp,"\n");
	}
		
	
	fclose(ouF);
	

}

void main(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	_chdir("E:\\CarData\\voc2007\\testing\\car");
	
	s="000014";



	vector<int> thrs=infromstring("..\\..\\bndThreshld.txt");

	vector<int> kspt=infromstring("..\\..\\kptStep.txt");


	for (int i = 0; i < thrs.size(); i++)
	{
		for (int j = 0; j < kspt.size(); j++)
		{
			kptstoNumKpFea(thrs[i],kspt[j],s);
		}
	}

	
}

