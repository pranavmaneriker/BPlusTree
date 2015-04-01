#ifndef __BTREE__
#define __BTREE__
#include<string>

/**
Classes for the tree and nodes
**/
using namespace std;

class BPTreeNode{
	private:
	bool leaf;
	string parent;
	float * keys;
	string * valueF;	//valueFile
	int cCount; //children
	public:
	string * lChild;	//left child file / dataFile for this key
	string rmChild;		//rightmost child/ next leaf
	BPTreeNode(int ch)
	{
		values = new string[ch];
		keys = new string[ch];
		leaf = true;
	}
	bool isLeaf(){ return leaf; }
	BPTreeNode(const string& dataFile); //load node from datafile
};

class BPTree{
	string root;
	int degree;
	int intNodes, leafNodes, datFiles;
	string root;

	public:
	BPTree(int d): degree(d)
	{
		intNodes = 0;
		leafNodes = 0;
		root = "1"
	}
	void writeMeta Data();
	void insert(int key, const string& value);	

};

#endif
