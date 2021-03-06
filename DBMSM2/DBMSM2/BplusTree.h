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
#define M 3 // order of BplusTree

using namespace std;

class BplusTree;

class Node {
private:
    bool isLeafNode;
    int KeyCnt;
    string Key[M];
    Node* Parent;
    Node* Child[M+1];
    //string Data[M+1]; // store the data of a record
    static const int MaxKeyCnt = M;
    static const int MinKeyCnt = M / 2;
    static const int MinChildCnt = MinKeyCnt + 1;
    static const int MaxChildCnt = MaxKeyCnt + 1;
public:
    friend BplusTree;
    ~Node();
    Node(bool isLeaf);
    bool getLeaf();
    string getKey(int i);
    Node* getChild(int i);
    void setParent(Node* parent);
    void setKeyCnt(int cnt);
    void setChild(int ch, Node* child);
    void insertKey(string key);
    void split();
    void clear();
    void printInfo();
    void deleteLeafKey(string key, BplusTree* bpt);
    void deleteInteralKey(string key, Node* curNode, Node* rightChild, BplusTree* bpt);
};

class BplusTree {
private:
    Node* root;
public:
    BplusTree();
    ~BplusTree();
    void setRoot(Node* nr);
    bool find(string key); // return bool to test, later will return the data of the key
    void insertData(string key);
    void printTree();
    void deleteData(string key);
    bool doChoice();
};

#endif /* BplusTree_h */






