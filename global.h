#ifndef _GLOBAL_
#define _GLOBAL_

#include<iostream>
#include<cmath>
using namespace std;

class Stat{
public:
	double min;
	double max;
	int no;
	double sum;
	double sumsq;
	int tvar;
	Stat()
	{
		max=no=sumsq=sum=0;
		min = 10e18;
	}
	void print()
	{
		cout<<"Min: "<<min<<endl;
		cout<<"Max: "<<max<<endl;
		double avg = sum/no;
		cout<<"Avg: "<<avg<<endl;
		cout<<"Std dev:"<<sqrt((sumsq/no)-(avg*avg))<<endl;
	}
	void upd(double val)
	{
		min = val<min?val:min;
		max = val>max?val:max;
		sum+=val;
		no++;
		sumsq+=val*val;
	}
	void startUpdT()
	{
		tvar = 0;
	}
	void updT()
	{
		tvar++;
	}
	void endUpdT()
	{
		upd(tvar);
		tvar = 0;
	}
};

extern Stat TimeIns;
extern Stat TimePQ;
extern Stat TimeRQ;

extern Stat DAIns;
extern Stat DARQ;
extern Stat DAPQ;
extern int typeOfQ;

static void updDASt()
{
	if(typeOfQ == 0) DAIns.startUpdT();
	else if(typeOfQ == 1)DAPQ.startUpdT();
	else if(typeOfQ == 2)DARQ.startUpdT();
}
static void updDAEnd()
{
	if(typeOfQ == 0) DAIns.endUpdT();
	else if(typeOfQ == 1)DAPQ.endUpdT();
	else if(typeOfQ == 2)DARQ.endUpdT();
}

static void updDA()
{
	if(typeOfQ == 0) DAIns.updT();
	else if(typeOfQ == 1)DAPQ.updT();
	else if(typeOfQ == 2)DARQ.updT();
}


#endif