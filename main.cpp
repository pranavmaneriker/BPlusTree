#include<string>
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<ctime>
#include<cmath>
#include<sys/time.h>
#include<fstream>

#include "btree.hpp"
#include "global.h"

#define EPSILON 2e-10 

using namespace std;

Stat TimeIns;
Stat TimePQ;
Stat TimeRQ;

Stat DAIns;
Stat DARQ;
Stat DAPQ;

int typeOfQ;

long getTimeMs()
{
 struct timeval tv;
 gettimeofday(&tv, NULL);
 long ret = tv.tv_usec;
 ret /= 1000;
 ret += (tv.tv_sec * 1000);
 return ret;
}

bool AreSame(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

int main(int argc, char ** argv)
{
	long start;
	freopen("bplustree.config", "r", stdin); 
	int degree;
	cin>>degree;
	fclose(stdin);
	int count = 1;
	bool didSomething = false;
	if(argc != 3)
	{
		cerr<<"Incorrect usage. Use -c for initialisation and -x for queries."<<endl;
		cerr<<"Eg ./main -c input.txt"<<endl;
		cerr<<"./main -x queries.txt"<<endl;
		return 1;
	}
	else if(strcmp(argv[1], "-c") == 0) //only inserts to create the tree
	{
		double key;
		string value;
		didSomething = true;
		ifstream file;
		file.open(argv[2]);
		if(file.is_open())
		{
			//STDIN contains the datafile, create the tree
			BPTree * b = new BPTree(degree);

			typeOfQ = 0;
			while(file>>key>>value)
			{
				updDASt();
				start = getTimeMs();
				b->insert(key, value);
				TimeIns.upd(getTimeMs()-start);
				updDAEnd();
			}
			file.close();
			cout<<"Tree created successfully"<<endl;
		}
		else
		{
			cerr<<"Data file "<<argv[2]<<" not found"<<endl;
			return 2;
		}
	}
	else if(strcmp(argv[1], "-x") == 0)
	{
		didSomething = true;
		if(freopen(argv[2], "r", stdin))
		{
			//STDIN contains the datafile, create the tree
			BPTree * b = new BPTree(degree);
			b->loadMetaData();
			int type;
			double l,diff;
			string s;
			string fName;
			string filetext;
			while(cin>>type>>l)
			{
				if(type == 0){
					cin>>s;	
					typeOfQ = 0;
					start = getTimeMs();
					updDASt();
					b->insert(l, s);
					TimeIns.upd(getTimeMs() - start);
					updDAEnd();
				} 
				else if(type == 1)
				{
					typeOfQ = 1;
					cout<<count<<": ";
					updDASt();
					start = getTimeMs();
					BPTreeNode tnode= b->search(l);
					TimePQ.upd(getTimeMs() - start);
					updDAEnd();
					//search in tnode
					ifstream file;
					for(int i=0;i<tnode.cCount;++i)
					{
						if (AreSame(tnode.keys[i], l))
						{
							fName = datafileName(tnode.lChild[i]);
							file.open(fName.c_str());
							file>>filetext;
							cout<<filetext;
							file.close();
							break;
						}
					}
					cout<<endl;
				}
				else if(type == 2)
				{
					typeOfQ = 2;
					cout<<count<<": ";
					cin>>diff;
					double lval = l-diff;
					double rval = l+diff;
					start = getTimeMs();
					updDASt();
					BPTreeNode lnode = b->search(lval);
					BPTreeNode rnode = b->search(rval);
					BPTreeNode temp = lnode;
					//extra iteration for stats
					while(temp.fName!=rnode.fName)
					{
						temp = BPTreeNode(temp.nextLeaf, degree);
					}
					TimeRQ.upd(getTimeMs()-start);
					updDAEnd();
					//print from lval to rval only
					int i;
					ifstream file;
					if(lnode.fName == rnode.fName)
					{
						for(i=0;i<lnode.cCount;++i)
						{
							if(lnode.keys[i]<= rval && lnode.keys[i]>=lval )
							{
								fName = datafileName(lnode.lChild[i]);
								file.open(fName.c_str());
								file>>filetext;
								file.close();
								cout<<filetext<<" ";
							}
						}
					}
					else
					{
						BPTreeNode n = BPTreeNode(lnode.nextLeaf, degree);
						for(i=0;i<lnode.cCount && lval >=lnode.keys[i]; ++i);
						for(;i<lnode.cCount;++i)
						{
							fName = datafileName(lnode.lChild[i]);
							file.open(fName.c_str());
							file>>filetext;
							file.close();
							cout<<filetext<<" ";
						}
						while(n.fName!=rnode.fName)
						{
							n = BPTreeNode(n.nextLeaf, degree);
							for(int i=0;i<n.cCount;++i)
							{
								fName = datafileName(n.lChild[i]);
								file.open(fName.c_str());
								file>>filetext;
								file.close();
								cout<<filetext<<" ";
							}
						}
						for(int i=0;i<rnode.cCount && rnode.keys[i]<=rval;++i)
						{
							fName = datafileName(rnode.lChild[i]);
							file.open(fName.c_str());
							file>>filetext;
							file.close();
							cout<<filetext<<" ";
						}
					}
				}
				count++;
				cout<<endl;	
			}


		}
		else
		{
			cerr<<"Data file "<<argv[2]<<" not found"<<endl;
			return 2;
		}
	}
	if(didSomething && freopen("stats.txt","w",stdout))
	{
		cout<<"Time Ins"<<endl;
		TimeIns.print();
		cout<<endl<<endl;
		
		cout<<"Time Point Q"<<endl;
		TimePQ.print();
		cout<<endl<<endl;

		cout<<"Time Range Q"<<endl;
		TimeRQ.print();
		cout<<endl<<endl;

		cout<<"Disk Access Ins"<<endl;
		DAIns.print();
		cout<<endl<<endl;

		cout<<"Disk Access PQ"<<endl;
		DAPQ.print();
		cout<<endl<<endl;

		cout<<"Disk Access Range Q"<<endl;
		DARQ.print();
		cout<<endl<<endl;		
		fclose(stdout);
	}
	else
	{
		cout<<"Failed to generate stats";
	}
	return 0;
}
