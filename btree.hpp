#ifndef __BTREE__
#define __BTREE__
#include<string>
#include<sstream>
/**
Classes for the tree and nodes
**/
using namespace std;

class Helpers{
public:
	string numToStr(int num)
	{
		stringstream ss;
		ss << num;
		return ss.str();	
	}
	string nodefileName(const string& dataFile)
	{
		string prefix = "node/";
		return prefix + dataFile;
	}

	string datafileName(const string& dataFile)
	{
		string prefix = "data/";
		return prefix + dataFile;
	}

};

class BPTreeNode : Helpers{
	public:
	bool leaf;
	string parent;
	int cCount; //children
	float * keys;
	string * lChild;	//left child file / dataFile for this key
	string nextLeaf;		//rightmost child/ next leaf
	string fName; 		//own filename

	BPTreeNode(int ch)
	{
		lChild = new string[ch];
		keys = new float[ch];
		parent = "none";
		leaf = true;
	}
	bool isLeaf(){ return leaf; }
	BPTreeNode(const string& dataFile, int degree); //load node from datafile
	string searchLoc(float key);	//search in this node
	void writeToSelf();
	
};

class BPTree : Helpers{
	string root;
	int degree;
	int intNodes, leafNodes, datFiles;

	public:
	BPTree(int d): degree(d)
	{
		intNodes = 0;
		leafNodes = 0;
		root = "1";
	}
	void writeMetaData();
	BPTreeNode searchT(float key, const string& dataFile);
	void insert(float key, const string& value); //insert node in this	
	BPTreeNode search(float key);
	void splitNode();
	void insertNode(float splitval, const string& newRchild,const string& origNode);
	void splitNode(BPTreeNode t);
	void splitLeafNode(BPTreeNode t);

};

#endif
