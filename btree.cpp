#include"btree.hpp"
#include<iostream>

/* format of file - leaf parent  
		    cCount
		    keys values/lchilds
		    ... 
		    rchild/nextleaf
*/
BPTreeNode::BPTreeNode(const string& dataFile, int ch)
{
	freopen(dataFile.c_str(), "r", stdin);
	cin>>leaf>>parent;
	cin>>cCount;
	keys = new float[ch];
	lChild = new string[ch];
	for(int i=0;i < ch ;++i)
	{
		cin>>keys[i]>>lChild[i];
	}
	cin>>rmChild;
	fclose(stdin)
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
	
}

BPTreeNode
BPTree::search(float key)
{
	freopen("meta", "r", stdin);
	cin>>intNodes>>leafNodes>>root>>datFiles;
	fclose(stdin);
	BPTreeNode t = searctT(key, root);
	return t;
}

BPTreeNode 
BPTree::searchT(float key, const string& dataFile)
{
	BPTreeNode f(dataFile);
	if(f.isLeaf())
	{
		return f;			
	}
	else
	{
		for(int i=0; i<degree; ++i)
		{
			if(key <= f.keys[i]) return searchT(key, f.lChild[i]);	
		}
		//not <= any key
		return searchT(key, f.rmchild);
	}
}


