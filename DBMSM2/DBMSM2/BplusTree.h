//
//  BplusTree.h
//  DBMSM2
//
//  Created by CQQ on 2017/9/20.
//  Copyright © 2017年 CQQ. All rights reserved.
//

#ifndef BplusTree_h
#define BplusTree_h

#include <string>
#include <cstdlib>
#define M 5 // order of BplusTree

using namespace std;

class BplusTree;

class Node {
private:
    bool isLeafNode;
    int KeyCnt;
    string Key[M];
    Node* Child[M+1];
    //string Data[M+1]; // store the data of a record
    static const int MaxKeyCnt = M;
    static const int MinKeyCnt = M / 2;
    static const int MinChildCnt = MinKeyCnt + 1;
    static const int MaxChildCnt = MaxKeyCnt + 1;
public:
    friend BplusTree;
    Node(int isLeaf);
    bool getLeaf();
    string getKey(int i);
    Node* getChild(int i);
    void setLeaf(bool isleaf);
    void setKeyCnt(int cnt);
    void setChild(int ch, Node* child);
    void insertKey(string key);
    void split();
    void clear();
};

class BplusTree {
private:
    Node* root;
public:
    BplusTree();
    ~BplusTree();
    void insert(string key);
    
    void tt();
};



#endif /* BplusTree_h */






