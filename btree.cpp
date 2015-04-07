#include"btree.hpp"
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>
#include<fstream>
#include<assert.h>
#include "global.h"

/* format of file - leaf parent  
		    cCount
		    keys values/lchilds
		    ... 
		    rchild/nextleaf
*/
BPTreeNode::BPTreeNode(const string& dataFile, int degree)
{

	keys = new float[degree];
	lChild = new string[degree];
	string toWrite = nodefileName(dataFile);
	ifstream file;
	file.open(toWrite.c_str());
	if(file.is_open())
	{
		updDA();
		fName = dataFile;
		file>>leaf>>parent;
		file>>cCount;
		if(!leaf)cCount--;
		for(int i=0;i < cCount ;++i)
		{
			file>>keys[i]>>lChild[i];
		}
		if(!leaf)cCount++;
		if(leaf)file>>nextLeaf;
		else file>>rmChild;
		file.close();
	}
}


void 
BPTreeNode::writeToSelf()
{
	string toWrite = nodefileName(fName);
	ofstream file;
	file.open(toWrite.c_str());
	assert(file.is_open());
	updDA();
	file<<leaf<<" "<<parent<<endl;
	file<<cCount<<endl;
	if(!leaf) cCount--;
	for(int i=0;i<cCount;++i)
	{
		file<<keys[i]<<" "<<lChild[i]<<endl;
	}	
	if(!leaf)cCount++;
	if(leaf) file<<nextLeaf;
	else file<<rmChild;
	file.close();
}

void
BPTree::splitLeafNode(BPTreeNode left)
{

	int lC = left.cCount/2 , rC = left.cCount - lC;
	float sepval = left.keys[lC];

	string p = left.parent;
	BPTreeNode right(degree);

	right.fName = left.fName;
	left.fName = numToStr(intNodes++); leafNodes++;
	right.parent = p;
	right.nextLeaf = left.nextLeaf;
	left.nextLeaf = right.fName;

	for(int i=lC;i<left.cCount;++i)
	{
		right.keys[i-lC] = left.keys[i];
		right.lChild[i-lC] =  left.lChild[i];
	}
	
	left.cCount = lC;
	right.cCount = rC;

	if(p == "none") //inserting in root, create new node, code can be put in func
	{
		p = numToStr(intNodes++);
		root = p;
		left.parent = p;
		right.parent = p;
		BPTreeNode parentNode(degree);
		parentNode.fName = p;
		parentNode.keys[0] = sepval;
		parentNode.lChild[0] = left.fName;
		parentNode.leaf = false;
		parentNode.cCount = 2;
		parentNode.rmChild = right.fName;
		parentNode.writeToSelf();
		right.writeToSelf();
		left.writeToSelf();
	}
	else{
		right.writeToSelf();
		left.writeToSelf();
		insertNode(sepval, left.fName, p);
	} 


}

void
BPTree::splitNode(BPTreeNode left)
{

	string p = left.parent;
	int lC = left.cCount/2 , rC = left.cCount - lC;
	float sepval = left.keys[lC];

	BPTreeNode right(degree);

	right.fName = left.fName;
	right.leaf = false;
	left.fName = numToStr(intNodes++); leafNodes++;
	right.parent = p;

	for(int i=lC;i<left.cCount-1;++i)
	{
		right.keys[i-lC] = left.keys[i];
		right.lChild[i-lC] = left.lChild[i];
	}
	right.rmChild = left.rmChild;	
	left.rmChild = left.lChild[lC-1];
	left.cCount = lC;
	right.cCount = rC;

	if(p == "none") //inserting in root
	{
		p = numToStr(intNodes++);
		root = p;
		left.parent = p;
		right.parent = p;
		//foll code can be put in func
		BPTreeNode parentNode(degree);	
		parentNode.fName = p;
		parentNode.keys[0] = sepval;
		parentNode.lChild[0] = left.fName;
		parentNode.leaf = false;
		parentNode.cCount = 2;
		parentNode.rmChild = right.fName;
		parentNode.writeToSelf();
		right.writeToSelf();
		left.writeToSelf();
	}
	else{
			right.writeToSelf();
			left.writeToSelf();
			insertNode(sepval, left.fName, p);
	} 

	writeMetaData();
}

void
BPTree::insertNode(float splitval,const string& newRchild, const string& origNode)
{

	BPTreeNode curNode(origNode, degree);

	vector<pair<float, string> > datavals;
	for(int i=0;i<curNode.cCount-1 ;++i)
	{
		datavals.push_back(make_pair(curNode.keys[i], curNode.lChild[i]));
	}
	if(splitval > curNode.keys[curNode.cCount-2])
	{
		datavals.push_back(make_pair(splitval, curNode.rmChild));
		curNode.rmChild = newRchild;
	}
	else{ datavals.push_back(make_pair(splitval, newRchild)); }
	sort(datavals.begin(), datavals.end());

	if(curNode.cCount < degree)
	{
		curNode.cCount++;

		for(int i=0;i<datavals.size();++i)
		{
			curNode.keys[i] = datavals[i].first;
			curNode.lChild[i] = datavals[i].second;
		}
		curNode.writeToSelf();
	}
	else
	{
		splitNode(curNode);
	}	
}


string
BPTreeNode::searchLoc(float key)
{

		for(int i=0; i<cCount; ++i)
		{
			if(key < keys[i]) return lChild[i];	
		}
		//not < any key
		return rmChild;
}

/** Tree functions begin **/
void
BPTree::writeMetaData()
{
	//output format: internalnodes leafnodes root datfiles

	ofstream file;
	file.open("meta");
	updDA();
	file<<intNodes<<" "<<leafNodes<<" "<<root<<" "<<datFiles<<endl;
	file.close();
}

void BPTree::loadMetaData()
{
	ifstream file;
	file.open("meta");
	updDA();	
	file>>intNodes>>leafNodes>>root>>datFiles;
	file.close();
}

void
BPTree::insert(float key, const string& value)
{

		//create the data file to hold this value
		string datName = numToStr(datFiles);
		string df = datafileName(datName);
		ofstream file;
		file.open(df.c_str());
		file<<value<<endl;
		file.close();

		datFiles++;	//above file added

		//TODO Handle first insertion
		BPTreeNode insertInto(degree);
		if(leafNodes > 0) { writeMetaData(); insertInto = search(key); }
		else { insertInto.fName = numToStr(intNodes++); leafNodes++; insertInto.nextLeaf = "none";}

		vector<pair<float, string> > datavals;
		int i;
		for(i=0;i<insertInto.cCount;++i)
		{
			datavals.push_back(make_pair(insertInto.keys[i], insertInto.lChild[i]));
		}
		datavals.push_back(make_pair(key, datName));
		sort(datavals.begin(), datavals.end());

		insertInto.cCount++;

		for(int i=0;i<datavals.size();++i)
		{
			insertInto.keys[i] = datavals[i].first;
			insertInto.lChild[i] = datavals[i].second;
		}


		if(insertInto.cCount < degree)
		{
			insertInto.writeToSelf();
		}
		else
		{
			//need to split
			splitLeafNode(insertInto);
		}
		writeMetaData();
}



BPTreeNode
BPTree::search(float key)
{

	ifstream file;
	file.open("meta");
	updDA();
	file>>intNodes>>leafNodes>>root>>datFiles;
	file.close();
	BPTreeNode t = searchT(key, root);
	return t;
}

BPTreeNode 
BPTree::searchT(float key, const string& dataFile)
{

	BPTreeNode f(dataFile, degree);
	if(f.isLeaf())
	{
		return f;			
	}
	else
	{
		return searchT(key, f.searchLoc(key));
	}
}

