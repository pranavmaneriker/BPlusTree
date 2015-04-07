#include<string>
#include<iostream>
#include<iomanip>
#include<cstring>
#include<cstdio>
#include<ctime>
#include<sys/time.h>
#include<fstream>

#include "btree.hpp"
#include "global.h"

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

int main(int argc, char ** argv)
{
	long start;
	freopen("bplustree.config", "r", stdin); 
	int degree;
	cin>>degree;
	fclose(stdin);
	int count = 0;
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
			fclose(stdin);
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
					for(int i=0;i<tnode.cCount;++i)
					{
						if (tnode.keys[i] == l)
						{
							cout<<tnode.lChild[i]<<endl;
						}
					}
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
					BPTreeNode *temp = new BPTreeNode(lnode.nextLeaf, degree);
					//extra iteration for stats
					while(temp->fName!=rnode.fName)
					{
						temp = new BPTreeNode(temp->nextLeaf, degree);
						for(int i=0;i<temp->cCount;++i)
						{
							cout<<temp->lChild[i]<<" ";
						}
					}
					TimeRQ.upd(getTimeMs()-start);
					updDAEnd();
					BPTreeNode *n = new BPTreeNode(lnode.nextLeaf, degree);
					//print from lval to rval only
					int i;
					for(i=0;i<lnode.cCount && lval >=lnode.keys[i]; ++i);
					for(;i<lnode.cCount;++i)
					{cout<<lnode.lChild[i]<<" ";}
					while(n->fName!=rnode.fName)
					{
						n = new BPTreeNode(n->nextLeaf, degree);
						for(int i=0;i<n->cCount;++i)
						{
							cout<<n->lChild[i]<<" ";
						}
					}
					for(int i=0;i<rnode.cCount && rnode.keys[i]<=rval;++i)
					{
						cout<<rnode.lChild[i]<<" ";
					}
				}
				count++;
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
