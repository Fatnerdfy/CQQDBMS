//
//  BplusTree.cpp
//  DBMSM2
//
//  Created by CQQ on 2017/9/20.
//  Copyright © 2017年 CQQ. All rights reserved.
//

#include "BplusTree.h"
#include <queue>
#include <iostream>

Node::Node(bool isLeaf) {
    KeyCnt = 0;
    Parent = NULL;
    isLeafNode = isLeaf;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

Node::~Node() {
    delete this;
}

void Node::setKeyCnt(int cnt) {
    this->KeyCnt = cnt;
}

bool Node::getLeaf() {
    return this->isLeafNode;
}

string Node::getKey(int i) {
    return Key[i];
}

Node* Node::getChild(int i) {
    return Child[i];
}

void Node::setParent(Node* p) {
    this->Parent = p;
}

void Node::setChild(int ch, Node* child) {
    this->Child[ch] = child;
}

void Node::clear() {
    KeyCnt = 0;
    Parent = NULL;
    isLeafNode = false;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

void Node::insertKey(string key) {
    this->Key[this->KeyCnt++] = key;
    sort(this->Key, this->Key+KeyCnt); // sort Key
    if(this->KeyCnt==Node::MaxKeyCnt) {
        this->split();
    }
}

void Node::split() {
    
    // create left child
    Node* lchild;
    if (this->isLeafNode) {
        lchild = new Node(true);
    } else {
        lchild = new Node(false);
    }
    lchild->setKeyCnt(Node::MinKeyCnt);
    for (int i = 0; i < Node::MinKeyCnt; i++) {
        lchild->Key[i] = this->Key[i];
    }
    if (!this->isLeafNode) {
        for (int i = 0; i < Node::MinChildCnt; i++) {
            lchild->Child[i] = this->Child[i];
        }
    }
    lchild->setParent(this);
    
    // create right child
    Node* rchild;
    if (this->isLeafNode) {
        rchild = new Node(true);
        rchild->setKeyCnt(this->KeyCnt - Node::MinKeyCnt);
        for (int i = 0; i < rchild->KeyCnt; i++) {
            rchild->Key[i] = this->Key[i+Node::MinKeyCnt];
        }
        // for (int i = 0; i < this->KeyCnt+1-Node::MinChildCnt; i++) {
        //     rchild->Child[i] = this->Child[i+Node::MinChildCnt];
        // }
    } else {
        rchild = new Node(false);
        rchild->setKeyCnt(this->KeyCnt - Node::MinKeyCnt - 1);
        for (int i = 0; i < rchild->KeyCnt; i++) {
            rchild->Key[i] = this->Key[i+Node::MinKeyCnt+1];
        }
        for (int i = 0; i < this->KeyCnt+1-Node::MinChildCnt; i++) {
            rchild->Child[i] = this->Child[i+Node::MinChildCnt];
        }
    }
    rchild->setParent(this);
    
    // BplusTree - link the data of leaf node
    if (this->isLeafNode) {
        lchild->Child[lchild->KeyCnt] = rchild;
        rchild->Child[0] = lchild;
        
        if (this->Child[0]!=NULL) {
            Node* leftLeaf = this->Child[0];
            leftLeaf->Child[leftLeaf->KeyCnt] = lchild;
        }
        
        if (this->Child[this->KeyCnt]!=NULL) {
            Node* rightLeaf = this->Child[this->KeyCnt];
            rightLeaf->Child[0] = rchild;
        }
    } else {
        // reset Node children's parent
        for (int i = 0; i < MinChildCnt; i++) {
            this->Child[i]->setParent(lchild);
        }
        for (int i = MinChildCnt; i < this->KeyCnt + 1; i++) {
            this->Child[i]->setParent(rchild);
        }
    }
    
    // rebuild parent
    Node* p = this->Parent;
    string strtmp = Key[Node::MinKeyCnt];
    
    int pos = 0;
    if (p!=NULL) {
        for (int i = 0; i < p->KeyCnt + 1; i++) {
            if(p->Child[i]==this) {
                pos = i;
                break;
            }
        }
        for (int i = Node::MaxChildCnt; i > pos; i--) {
            p->Child[i] = p->Child[i-1];
        }
        p->Child[pos] = lchild;
        p->Child[pos+1] = rchild;
        lchild->setParent(p);
        rchild->setParent(p);
        p->insertKey(strtmp);
    } else {
        this->clear();
        this->Child[0] = lchild;
        this->Child[1] = rchild;
        this->insertKey(strtmp);
    }
}

void Node::printInfo() {
    cout << "NodeInfo: ##########" << endl;
    cout << "isLeafNode: " << this->isLeafNode << endl;
    cout << "KeyCnt: " << this->KeyCnt << endl;
    cout << "Key: ";
    for (int i = 0; i < this->KeyCnt; i++) {
        cout << this->Key[i] <<" ";
    }
    cout << "\n" << endl;
}

BplusTree::BplusTree() {
    root = new Node(1);
}

void BplusTree::insert(string key) {
    if (root->getLeaf()) {
        root->insertKey(key);
    } else {
        Node* pCurNode = root; // a pointer to current node
        while (!pCurNode->getLeaf()) {
            for (int i = 0; i < pCurNode->KeyCnt; i++) {
                if (key <= pCurNode->getKey(i)) {
                    pCurNode = pCurNode->getChild(i);
                    break;
                } else if (i==pCurNode->KeyCnt - 1) {
                    pCurNode = pCurNode->getChild(pCurNode->KeyCnt);
                    break;
                }
            }
        }
        pCurNode->insertKey(key);
    }
}

void BplusTree::printTree() {
    Node* tmp = NULL;
    queue <Node*> nq;
    nq.push(root);
    while (!nq.empty()) {
        tmp = nq.front();
        nq.pop();
        tmp->printInfo();
        if (!tmp->getLeaf()) {
            for (int i = 0; i < tmp->KeyCnt + 1; i++) {
                nq.push(tmp->Child[i]);
            }
        }
    }
}











