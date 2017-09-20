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

bool Node::getLeaf() {
    return isLeafNode;
}

string Node::getKey(int i) {
    return Key[i];
}

Node* Node::getChild(int i) {
    return Child[i];
}

void Node::setLeaf(bool isleaf) {
    this->isLeafNode = isleaf;
}

void Node::setChild(int ch, Node* child) {
    this->Child[ch] = child;
}

void Node::clear() {
    KeyCnt = 0;
    isLeafNode = false;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

void Node::insertKey(string key) {
    Key[KeyCnt++] = key;
    sort(Key, Key+KeyCnt); // sort Key
    if(this->KeyCnt==Node::MaxKeyCnt) {
        this->split();
    }
}

void Node::split() {
    
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
    Node* lchild;
    if (isLeafNode) {
        lchild = new Node(1);
        lchild->setKeyCnt(this->KeyCnt - Node::MinKeyCnt);
        for (int i = 0; i < lchild->KeyCnt; i++) {
            lchild->Key[i] = this->Key[i+Node::MinKeyCnt];
        }
        for (int i = MinChildCnt; i < this->KeyCnt+1; i++) {
            lchild->Child[i] = this->Child[i];
        }
    } else {
        lchild = new Node(0);
        lchild->setKeyCnt(this->KeyCnt - Node::MinKeyCnt);
        for (int i = 0; i < lchild->KeyCnt; i++) {
            lchild->Key[i] = this->Key[i+Node::MinKeyCnt+1];
        }
        for (int i = MinChildCnt; i < this->KeyCnt+1; i++) {
            lchild->Child[i] = this->Child[i];
        }
    }
    
    // BplusTree - link the data of leaf node
    if (isLeafNode) {
        rchild->isLeafNode = true;
        rchild->Child[rchild->KeyCnt] = lchild;
        lchild->Child[0] = rchild;
        
        if (this->Child[0]!=NULL) {
            Node* leftLeaf = this->Child[0];
            leftLeaf->Child[leftLeaf->KeyCnt] = lchild;
        }
        
        if (this->Child[this->KeyCnt]!=NULL) {
            Node* rightLeaf = this->Child[this->KeyCnt];
            rightLeaf->Child[0] = rchild;
        }
    }
    
    // rebuild parent
    string strtmp = Key[Node::MinKeyCnt];
    this->clear();
    this->insertKey(strtmp);
    this->setChild(0, rchild);
    this->setChild(1, lchild);
}


BplusTree::BplusTree() {
    root = NULL;
}

void BplusTree::insert(string key) {
    if (root==NULL) {
        root = new Node(1);
        root->insertKey(key);
    } else if (root->getLeaf()) {
        root->insertKey(key);
    } else {
        Node* pCurNode = root; // a pointer to current node
        while (!pCurNode->getLeaf()) {
            for (int i = 0; i <= pCurNode->KeyCnt; i++) {
                if (key < pCurNode->getKey(i)) {
                    pCurNode = pCurNode->getChild(i);
                    break;
                }
            }
        }
        pCurNode->insertKey(key);
    }
}

void BplusTree::tt() {
    cout << root->Key[0] << endl;
    cout << root->Child[0]->Key[0] << endl;
    cout << root->Child[1]->Key[0] << endl;
}

