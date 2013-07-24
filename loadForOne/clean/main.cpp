
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

	while( st!=daset.end() && st->first<ymax )
	{
		auto st1=st->second.lower_bound(xmin);

		while ( st1!=st->second.end() && st1->first<xmax  )
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

template<class T1,class T2, class T3>
struct triple
{
	T1 feas;
	T2 kptFea;
	T3 intmap;
};

triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > kptstoNumKpFea( int thr, int stp,string s)
{

	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > rslt;

	vector<pair<int,int> > temkpts;

	temkpts.clear();

	//vector<vector<double> > feas;
	rslt.feas.clear();


	
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

	rslt.feas.resize(tnum,vector<double>(featureDimension,0.0));

	fp=fopen((mys+"_feas.txt").c_str(),"r");
	for (int i = 0; i < rslt.feas.size(); i++)
	{
		for (int j = 0; j < featureDimension; j++)
		{
			fscanf(fp,"%lf ",&rslt.feas[i][j]);
		}
		fscanf(fp,"\n");
	}
	fclose(fp);


	//map<int,map<int, int > > kptFea;

	rslt.kptFea.clear();

	for (int i = 0; i < temkpts.size(); i++)
	{
		int a=temkpts[i].second;
		if (rslt.kptFea.count(a)>0)
		{
			int b=temkpts[i].first;
			rslt.kptFea[a].insert(pair<int,int>(b,i));
		}
		else
		{
			map<int, int > sth;
			int b=temkpts[i].first;
			sth.insert(pair<int,int>(b,i));
			rslt.kptFea.insert(pair<int,map<int, int > >(a,sth));
		}
	}

	

	auto sth=rangequery(10,0,200,300,rslt.kptFea);

	//vector<vector<int> > intmap;

	FILE* ouF;
	ouF=fopen((mys+"_kptintg.txt").c_str(),"r");
	
	int heit,wid;
	fscanf(ouF,"%d %d\n",&heit,&wid);
	rslt.intmap.resize(heit,vector<int>(wid,0));

	for (int i = 0; i < heit; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			fscanf(fp,"%d ",&rslt.intmap[i][j]);
		}
		fscanf(fp,"\n");
	}
		
	
	fclose(ouF);
	
	return rslt;
}

void main(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	//_chdir("E:\\car");
	
	s="000007";

	
	vector<int> thrs=infromstring("bndThreshld.txt");

	vector<int> kspt=infromstring("kptStep.txt");

	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > ms;
	vector<vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > > infos;
	infos.resize(kspt.size(),vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > (thrs.size(),ms));


	



	
	for (int j = 0; j < kspt.size(); j++)
	{
		for (int i = 0; i < thrs.size(); i++)
		{
			infos[j][i]=	kptstoNumKpFea(thrs[i],kspt[j],s);
		}
	}

	
}

