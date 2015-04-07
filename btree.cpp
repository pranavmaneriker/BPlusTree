#include"btree.hpp"
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<utility>

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
	if(freopen(toWrite.c_str(), "r", stdin))
	{
		fName = dataFile;
		cin>>leaf>>parent;
		cin>>cCount;
		for(int i=0;i < cCount ;++i)
		{
			cin>>keys[i]>>lChild[i];
		}
		if(leaf)cin>>nextLeaf;
		fclose(stdin);
	}
}


void 
BPTreeNode::writeToSelf()
{
	string toWrite = nodefileName(fName);
	freopen(fName.c_str(), "w", stdout);
	cout<<leaf<<" "<<parent<<endl;
	cout<<cCount<<endl;
	for(int i=0;i<cCount;++i)
	{
		cout<<keys[i]<<" "<<lChild[i]<<endl;
	}	
	if(leaf)cout<<nextLeaf;
	fclose(stdout);
}

void
BPTree::splitLeafNode(BPTreeNode left)
{
	string p = left.parent;
	string r = left.nextLeaf;
	BPTreeNode right(degree);

	right.fName = numToStr(intNodes++); leafNodes++;
	right.parent = p;
	right.nextLeaf = r;
	left.nextLeaf = right.fName;
	int lC = left.cCount/2 + 1 , rC = left.cCount - lC;

	float sepval = left.keys[lC-1];
	for(int i=lC;i<left.cCount;++i)
	{
		right.keys[i-lC] = left.keys[i];
		right.lChild[i-lC] =  left.lChild[i];
	}
	
	left.cCount = lC;
	right.cCount = rC;

	right.writeToSelf();
	left.writeToSelf();

	insertNode(sepval, right.fName, p);
}

void
BPTree::splitNode(BPTreeNode left)
{
	string p = left.parent;
	if(p == "none") //inserting in root
	{
		p = numToStr(intNodes++);
		root = p;
		left.parent = p;
	}

	string r = "";//TODO left.rmchild;
	BPTreeNode right(degree);

	right.fName = numToStr(intNodes++); leafNodes++;
	right.parent = p;
	right.nextLeaf = r;
	// TODO left.nextLeaf = right.fName;
	int lC = left.cCount/2 + 1 , rC = left.cCount - lC;

	float sepval = left.keys[lC-1];
	for(int i=lC;i<left.cCount;++i)
	{
		right.keys[i-lC] = left.keys[i];
		right.lChild[i-lC] =  left.lChild[i];
	}
	
	left.cCount = lC;
	right.cCount = rC;

	right.writeToSelf();
	left.writeToSelf();

	insertNode(sepval, right.fName, p);
}

void
BPTree::insertNode(float splitval, const string& newRchild, const string& origNode)
{
	BPTreeNode curNode(origNode, degree);

	vector<pair<float, string> > datavals;
	int i;
	for(i=0;i<curNode.cCount && splitval <= curNode.keys[i];++i)
	{
		datavals.push_back(make_pair(curNode.keys[i], curNode.lChild[i]));
	}
	datavals.push_back(make_pair(splitval, newRchild));
	for(;i<curNode.cCount; ++i)
	{
			datavals.push_back(make_pair(curNode.keys[i], curNode.lChild[i]));
	}

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
			if(key <= keys[i]) return lChild[i];	
		}
		//not <= any key
		return nextLeaf;
}

/** Tree functions begin **/
void
BPTree::writeMetaData()
{
	//output format: internalnodes leafnodes root datfiles
	freopen("meta", "w", stdout);
	cout<<intNodes<<" "<<leafNodes<<" "<<root<<" "<<datFiles<<endl;
	fclose(stdin);
}

void
BPTree::insert(float key, const string& value)
{
		//create the data file to hold this value
		string datName = numToStr(datFiles++);
		string df = datafileName(datName);
		freopen(df.c_str(), "w", stdout);
		cout<<value<<endl;
		fclose(stdin);

		datFiles++;	//above file added

		//TODO Handle first insertion
		BPTreeNode insertInto(degree);
		if(leafNodes > 0) insertInto = search(key);
		else { insertInto.fName = numToStr(intNodes++); leafNodes++; }

		vector<pair<float, string> > datavals;
		int i;
		for(i=0;i<insertInto.cCount && key <= insertInto.keys[i];++i)
		{
			datavals.push_back(make_pair(insertInto.keys[i], insertInto.lChild[i]));
		}
		datavals.push_back(make_pair(key, datName));
		for(;i<insertInto.cCount; ++i)
		{
				datavals.push_back(make_pair(insertInto.keys[i], insertInto.lChild[i]));
		}

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
}



BPTreeNode
BPTree::search(float key)
{
	freopen("meta", "r", stdin);
	cin>>intNodes>>leafNodes>>root>>datFiles;
	fclose(stdin);
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

