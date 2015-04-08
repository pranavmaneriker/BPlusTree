#ifndef __BTREE__
#define __BTREE__
#include<string>
#include<sstream>
#include "global.h"
/**
Classes for the tree and nodes
**/
using namespace std;


class BPTreeNode {
	public:
	bool leaf;
	string parent;
	int cCount; //children
	double * keys;
	string * lChild;	//left child file / dataFile for this key
	string nextLeaf;		//rightmost child/ next leaf
	string fName; 		//own filename
	string rmChild;		//rightmost child
	string prevLeaf;	//prev leaf

	BPTreeNode(int ch)
	{
		lChild = new string[ch];
		keys = new double[ch];
		parent = "none";
		rmChild = "none";
		prevLeaf = "none";
		leaf = true;
		cCount = 0;
	}
	bool isLeaf(){ return leaf; }
	BPTreeNode(const string& dataFile, int degree); //load node from datafile
	string searchLoc(double key);	//search in this node
	void writeToSelf();
	
};

class BPTree {
	string root;
	int degree;
	int intNodes, leafNodes, datFiles;

	public:
	BPTree(int d): degree(d)
	{
		intNodes = 0;
		leafNodes = 0;
		datFiles = 0;
	}
	void writeMetaData();
	BPTreeNode searchT(double key, const string& dataFile);
	void insert(double key, const string& value); //insert node in this	
	BPTreeNode search(double key);
	void splitNode();
	void insertNode(double splitval, const string& newRchild,const string& origNode);
	void splitNode(BPTreeNode t);
	void splitLeafNode(BPTreeNode t);
	void loadMetaData();
};

#endif
