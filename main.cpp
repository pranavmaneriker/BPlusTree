#include<string>
#include<iostream>
#include<iomanip>
#include "btree.hpp"

using namespace std;


int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		cerr<<"Incorrect usage. Refer README for correct use."<<endl;
		return 1;
	}
	else if(strcmp(argv[1], "-c")) //only inserts to create the tree
	{
		float key;
		string value;
		freopen(stdin,"r", "bplustree.config"); 
		int degree;
		cin>>degree;
		fclose(stdin);
		if(freopen(argv[2], "r", stdin))
		{
			//STDIN contains the datafile, create the tree
			BTree b = new Btree(degree);
			while(cin>>key>>value)
			{
				b.insert(key, value);
			}
			b.writeMetaData();
			fclose(stdin);
			cout<<"Tree created successfully"<<endl;
		}
		else
		{
			cerr<<"Data file "<<argv[2]<<" not found"<<endl;
			return 2;
		}
	}
	return 0;
}