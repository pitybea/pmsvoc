
#include <iostream>
#include <fstream>


#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "FileInOut.h"
using namespace std;

#define PT_per_L 11.0
#define Stasdd_Num 217
#define stdd_X 256.0
#define stdd_Y 128.0


//#define debugMode

#define featureDimension 64


struct obj
{
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int tror;
	string pos;
	
};

vector<obj> infromastring(string s)
{
	vector<obj> rslt;
	
	int num;
	FILE* fp=fopen(s.c_str(),"r");
	fscanf(fp,"%d\n",&num);
	if (num>0)
	{
		obj to;
		rslt.resize(num,to);
		for (int i = 0; i < num; i++)
		{
			char ts[30];
			fscanf(fp,"%d %d %d %d %d %s\n",&rslt[i].xmin,&rslt[i].ymin,&rslt[i].xmax,&rslt[i].ymax,&rslt[i].tror,&ts);
			rslt[i].pos=ts;
		}
	}

	return rslt;
	fclose(fp);
}

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
pair<vector<vector<int> >, vector<T> > rangequery(obj o,const map<int,map<int,T> >& daset)
{

	
	int xmin(o.xmin),ymin(o.ymin),ymax(o.ymax),xmax(o.xmax);

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

void kptstoNumKpFea( int thr, int stp,string s,triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > >& rslt)
{

//	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > rslt;

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

	

	//auto sth=rangequery(10,0,200,300,rslt.kptFea);

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
	
//	return rslt;
}


int area(obj o)
{
	return (o.xmax-o.xmin)*(o.ymax-o.ymin);
}
int disp(obj o, const vector<int>& ref)
{
	double av=sqrt((double) area(o));
	double st=av/PT_per_L;

	double dis=abs(st-(double) ref[0]);
	int minx=0;
	for (int i = 1; i < ref.size(); i++)
	{
		double tdis=abs(st-(double) ref[i]);
		if (tdis<dis)
		{
			dis =tdis;
			minx=i;
		}
	}
	return minx;
}

int countkptnumber(obj o,const vector<vector<int> >& intmap)
{
	int sx,sy,ex,ey;

	int h=intmap.size();
	int w=intmap[0].size();

	sx=o.xmin<0?0:o.xmin;
	sy=o.ymin<0?0:o.ymin;

	sx=sx<w?sx:(w-1);
	sy=sy<h?sy:(h-1);

	ex=o.xmax<w?o.xmax:(w-1);
	ey=o.ymax<h?o.ymax:(h-1);

	ex=ex<0?0:ex;
	ey=ey<0?0:ey;

	return intmap[ey][ex]+intmap[sy][sx]-intmap[ey][sx]-intmap[sy][ex];
}

pair<int,int> odisp(obj o,const vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > >& ref)
{
	int feasnum=countkptnumber(o,ref[0].intmap);
	int propernum=abs(feasnum-Stasdd_Num);
	int properindex=0;
	
	for (int i = 1; i < ref.size(); i++)
	{
		int tfen=countkptnumber(o,ref[i].intmap);
		int tpropernum=abs(tfen-Stasdd_Num);
		if (tpropernum<propernum)
		{
			propernum=tpropernum;
			properindex=i;
			feasnum=tfen;
		}
	}

	return pair<int,int>(properindex,feasnum);
}


void writeOobj(obj o,const string& s,int wh, const vector<vector<int> > & cors, const vector<int>& inx,const vector<vector<double> > & feas)
{
	char td[40];
	sprintf(td,"%s_objneg_%d",s.c_str(),wh);
	string mys(td);
	FILE* fp;
	
	fp=fopen((mys+"_num.txt").c_str(),"w");
	fprintf(fp,"%d",inx.size());

	fclose(fp);
	

	fp=fopen((mys+"_pts.txt").c_str(),"w");

	double ratiox(stdd_X/(double)(o.xmax-o.xmin)),ratioy(stdd_Y/(double)(o.ymax-o.ymin));
	for(auto sth:cors)
	{
		fprintf(fp,"%lf %lf\n",(sth[0]-o.xmin)*ratiox,(sth[1]-o.ymin)*ratioy);
	}
	fclose(fp);
	
	fp=fopen((mys+"_feas.txt").c_str(),"w");
	for(auto sth:inx)
	{
		for (int i = 0; i < feas[sth].size(); i++)
		{
			fprintf(fp,"%lf ",feas[sth][i]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);


}

void chagobj(obj& o,int x,int y)
{
	o.xmin+=x;
	o.xmax+=x;
	o.ymin+=y;
	o.ymax+=y;
}

vector<obj> moveobj(obj o)
{
	vector<obj> os;
	os.resize(4,o);
	int w=o.xmax-o.xmin;
	int h=o.ymax-o.ymin;

	chagobj(os[0],w,h);
	chagobj(os[1],0-w,h);
	chagobj(os[2],w,0-h);
	chagobj(os[3],0-w,0-h);

	return os;
}

void main(int argc, char* argv[])
{
	string s;
	
	//s=argv[1];
	
	//_chdir("E:\\car");
	
	if (argc==1)
	{
		_chdir("E:\\CarData\\voc2007\\demo\\car");
		s="000007";
	}
	else
	{
		s=argv[1];
	}
	
	

	
	vector<int> thrs=infromstring("..\\..\\bndThreshld.txt");

	vector<int> kspt=infromstring("..\\..\\kptStep.txt");

	vector<obj> bjs=infromastring(s+"_objs.txt");
	vector<obj> objs;
	objs.clear();
	for (int i = 0; i < bjs.size(); i++)
	{
		vector<obj> o=moveobj( bjs[i]);
		objs.insert(objs.end(),o.begin(),o.end());
	}

	
	unordered_map<int,int> thrs_um;
	unordered_map<int,int> kspt_um;
	for (int i = 0; i < thrs.size(); i++)
	{
		thrs_um[thrs[i]]=i;
	}
	for (int i = 0; i < kspt.size(); i++)
	{
		kspt_um[kspt[i]]=i;
	}

	triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > ms;
	vector<vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > > infos;
	infos.resize(kspt.size(),vector< triple<vector<vector<double> >,map<int,map<int, int > >, vector<vector<int> > > > (thrs.size(),ms));


	


	vector<int> ks;
	ks.resize(objs.size(),0);

	unordered_set<int> ks_neededs;

	for (int i = 0; i < objs.size(); i++)
	{
		int ksi(disp(objs[i],kspt));
		ks[i]=ksi;
		ks_neededs.insert(ksi);
	}
	
	for(auto kki:ks_neededs)
	{
		int j=kki;
		for (int i = 0; i < thrs.size(); i++)
		{
			kptstoNumKpFea(thrs[i],kspt[j],s,infos[j][i]);
		}
	}

	int gi(0);
	for (int i = 0; i < objs.size(); i++)
	{
		
		auto gth=odisp(objs[i],infos[ks[i]]);

		if (gth.second>120)
		{
			int thi(gth.first) ;

			auto oneok=rangequery(objs[i],infos[ks[i]][thi].kptFea);

			writeOobj(objs[i],s,gi,oneok.first,oneok.second,infos[ks[i]][thi].feas);
			gi+=1;
		}
		
	}
	
}

