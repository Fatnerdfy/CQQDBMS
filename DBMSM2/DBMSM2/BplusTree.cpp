//
//  BplusTree.cpp
//  DBMSM2
//
//  Created by CQQ on 2017/9/20.
//  Copyright © 2017年 CQQ. All rights reserved.
//

#include "BplusTree.h"
#include <iostream>

Node::Node(int isLeaf):isLeafNode(isLeaf) {
    KeyCnt = 0;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

void Node::setKeyCnt(int cnt) {
    this->KeyCnt = cnt;
}

void Node::setChild(int ch, Node* child) {
    this->Child[ch] = child;
}

void Node::clear() {
    KeyCnt = 0;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

void Node::insertKey(string key) {
    Key[KeyCnt++] = key;
    sort(Key, Key+KeyCnt); // sort Key
}

void Node::spilt() {
    // create right child
    Node* rchild = new Node(0);
    rchild->setKeyCnt(Node::MinKeyCnt);
    for (int i = 0; i < Node::MinKeyCnt; i++) {
        rchild->Key[i] = this->Key[i];
    }
    for (int i = 0; i < Node::MinChildCnt; i++) {
        rchild->Child[i] = this->Child[i];
    }
    
    // create left child
    Node* lchild = new Node(0);
    lchild->setKeyCnt(this->KeyCnt - Node::MinKeyCnt);
    for (int i = 0; i < lchild->KeyCnt; i++) {
        lchild->Key[i] = this->Key[i+Node::MinKeyCnt];
    }
    for (int i = MinChildCnt; i < this->KeyCnt+1; i++) {
        lchild->Child[i] = this->Child[i];
    }
    
    // rebuild parent
    string strtmp = Key[Node::MinKeyCnt];
    this->clear();
    this->insertKey(strtmp);
    this->setChild(0, rchild);
    this->setChild(1, lchild);
}




BplusTree::BplusTree() {
    depth = 0;
    root = NULL;
}

void BplusTree::insert(string key) {
    if (root==NULL) {
        root = new Node(0);
        root->insertKey(key);
    } else if (depth==0) {
        root->insertKey(key);
        cout << "key is" << root->KeyCnt << endl;
        if(root->KeyCnt==Node::MaxKeyCnt) {
            root->spilt();
            depth += 1;
        }
    } else {
        // Node* pCurNode = root; // a pointer to current node
        
        // for (int i = 0; i < pCurNode->KeyCnt; i++) {
        
        // }
    }
}

void BplusTree::tt() {
    cout << root->Key[0] << endl;
    cout << root->Child[0]->Key[0] << endl;
    cout << root->Child[1]->Key[0] << endl;
}

