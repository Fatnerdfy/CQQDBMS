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


///----------------------Node-----------------
Node::Node(bool isLeaf) {
    KeyCnt = 0;
    Parent = NULL;
    isLeafNode = isLeaf;
    memset(Key, NULL, sizeof(Key));
    memset(Child, NULL, sizeof(Child));
}

Node::~Node() {
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
        delete this;
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

void Node::deleteLeafKey(string key) {
    
    int curNode = 0;
    int leftNode = 0;
    int rightNode = 0;
    Node* pNode = this->Parent;
    // find key in node
    bool find = false;
    for (int i = 0; i < this->KeyCnt; ++i) {
        if (this->Key[i] == key) {
            find = true;
            break;
        }
    }
    if (find == false) {
        return ;
    } else {
        // delete key in leafNode
        for (int i = 0; i < this->KeyCnt; ++i) {
            if (this->Key[i] == key) {
                this->Key[i] = this->Key[this->KeyCnt - 1];
                this->KeyCnt--;
                sort(this->Key, this->Key + this->KeyCnt); // sort Key
                break;
            }
        }
    }
    
    // find curNode position and its brother
    if (pNode != nullptr) {
        for (int i = 0; i < pNode->KeyCnt + 1; ++i) {
            if (pNode->Child[i]==this) {
                curNode = i;
                leftNode = i - 1;
                rightNode = i + 1;
                break;
            }
        }
    }
    
    if (this->KeyCnt >= (M + 1) / 2 - 1) {
        // --------------------------------
        // this node is not underflow
        // --------------------------------
        // update the search key in the parent node
        if (pNode != nullptr && curNode != 0) {
            pNode->Key[curNode - 1] = this->Key[0];
        }
    } else {
        // --------------------------------------------------
        // this node is underflow
        // fix the size of this node with transfer or merge
        // --------------------------------------------------
        Node* left = nullptr;
        left = leftNode >= 0 ? pNode->Child[leftNode] : nullptr;
        Node* right = nullptr;
        right = rightNode <= pNode->KeyCnt + 1 ? pNode->Child[rightNode] : nullptr;
        if (left != nullptr && left->KeyCnt >= (M + 1) / 2) {
            // ---------------------------------------------------------
            // 1.transfer last key and ptr from left node to this node
            // 2.update the search key in the parent node
            // ---------------------------------------------------------
            //------------------remember to copy data......
            this->Key[this->KeyCnt++] = left->Key[left->KeyCnt - 1];
            sort(this->Key, this->Key + this->KeyCnt);
            left->KeyCnt--;
            // update the search key in the parent node
            pNode->Key[leftNode] = this->Key[0];
        } else if (right != nullptr && right->KeyCnt >= (M + 1) / 2) {
            // ----------------------------------------------------------
            // 1.transfer first key and ptr from right node to this node
            // 2.update the search key in the parent node
            // ----------------------------------------------------------
            //------------------remember to copy data......
            this->Key[this->KeyCnt++] = right->Key[0];
            for (int i = 0; i < right->KeyCnt - 1; ++i) {
                right->Key[i] = right->Key[i + 1];
            }
            right->KeyCnt--;
            // update the search key in the parent node
            pNode->Key[rightNode - 1] = this->Key[this->KeyCnt - 1];
        } else if (left != nullptr) {
            //------------------remember to copy data......
            for (int i = 0; i < this->KeyCnt; ++i) {
                left->Key[left->KeyCnt++] = this->Key[i];
            }
            this->deleteInteralKey(this->Key[0], pNode, this);
        } else {
            //------------------remember to copy data......
            for (int i = 0; i < right->KeyCnt; ++i) {
                this->Key[this->KeyCnt++] = right->Key[i];
            }
            this->deleteInteralKey(right->Key[0], pNode, right);
        }
    }
    
}

void Node::deleteInteralKey(string key, Node* curNode, Node* rightChild) {
    int curIndex = 0;
    int leftIndex = 0;
    int rightIndex = 0;
    Node* pNode = curNode->Parent;
    // delete key and right child in curNode
    for (int i = 0; i < curNode->KeyCnt; ++i) {
        if (curNode->Key[i] == key && curNode->Child[i + 1] == rightChild) {
            curNode->Key[i] = curNode->Key[curNode->KeyCnt - 1];
            delete curNode->Child[i + 1];
            for (int j = i + 1; j < curNode->KeyCnt; ++j) {
                curNode->Child[j] = curNode->Child[j + 1];
            }
            curNode->KeyCnt--;
            sort(curNode->Key, curNode->Key + curNode->KeyCnt);
            break;
        }
    }
    // find curNode position and its brother
    if (pNode != nullptr) {
        for (int i = 0; i < pNode->KeyCnt + 1; ++i) {
            if (pNode->Child[i]==curNode) {
                curIndex = i;
                leftIndex = i - 1;
                rightIndex = i + 1;
                break;
            }
        }
    }
    Node* left = nullptr;
    Node* right = nullptr;
    left = leftIndex >= 0 ? pNode->Child[leftIndex] : nullptr;
    right = rightIndex <= pNode->KeyCnt ? pNode->Child[rightIndex] : nullptr;
    // --------------------------------------
    // check for underflow condition
    // --------------------------------------
    if (curNode->KeyCnt >= (M + 1) / 2 - 1) {
        // update parent search key
        return ;
        // -------------------------------------------------
        // curNode is underflow
        // fix the size of curNode with transfer or merge
        // -------------------------------------------------
    } else if (left != nullptr && left->KeyCnt >= (M + 1) / 2) {
        // --------------------------------------------------------------------------------
        // 1.transfer last key from left node through parent into curNode as the first key
        // 2.transfer right child link into curNode as the first link
        // --------------------------------------------------------------------------------
        for (int i = curNode->KeyCnt; i > 0; --i) {
            curNode->Key[i] = curNode->Key[i - 1];
            curNode->Child[i + 1] = curNode->Child[i];
        }
        curNode->KeyCnt++;
        curNode->Child[1] = curNode->Child[0];
        
        curNode->Key[0] = pNode->Key[leftIndex];
        curNode->Child[0] = left->Child[left->KeyCnt];
        pNode->Key[leftIndex] = left->Key[left->KeyCnt - 1];
        left->KeyCnt--;
    } else if (right != nullptr && right->KeyCnt >= (M + 1) / 2) {
        // ----------------------------------------------------------------------------------
        // 1.transfer first key from right node through parent into curNode as the last key
        // 2.transfer left child link into curNode as the last link
        // ----------------------------------------------------------------------------------
        curNode->Key[curNode->KeyCnt++] = pNode->Key[curIndex];
        curNode->Child[curNode->KeyCnt] = right->Child[0];
        
        pNode->Key[curIndex] = right->Key[0];
        
        for (int i = 0; i < right->KeyCnt - 1; ++i) {
            right->Key[i] = right->Key[i + 1];
        }
        right->KeyCnt--;
    } else if (left != nullptr) {
        // --------------------------------------------------------------------------------
        // 1.merge left node and curNode into left node
        // 2.transfer the key from parent node between the 2 pieces
        // 3.delete (transfered key, parent, right child, link)
        // --------------------------------------------------------------------------------
        string transferedKey = pNode->Key[leftIndex];
        left->Key[left->KeyCnt++] = pNode->Key[leftIndex];
        
        for (int i = 0; i < curNode->KeyCnt + 1; ++i) {
            left->Child[left->KeyCnt + i] = curNode->Child[i];
        }
        for (int i = 0; i < curNode->KeyCnt; ++i) {
            left->Key[left->KeyCnt++] = curNode->Key[i];
        }
        
        deleteInteralKey(transferedKey, pNode, curNode);
    } else {
        string transferedKey = pNode->Key[curIndex];
        curNode->Key[curNode->KeyCnt++] = pNode->Key[curIndex];
        
        for (int i = 0; i < right->KeyCnt + 1; ++i) {
            curNode->Child[curNode->KeyCnt + i] = right->Child[i];
        }
        for (int i = 0; i < right->KeyCnt; ++i) {
            curNode->Key[curNode->KeyCnt++] = right->Key[i];
        }
        
        deleteInteralKey(transferedKey, pNode, right);
    }
}

///--------------------BplusTree------------------------------
BplusTree::BplusTree() {
    root = new Node(1);
}

bool BplusTree::find(string key) {
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
    for (int i = 0; i < pCurNode->KeyCnt; i++) {
        // if return the data of this key, return its child pointer
        if (key==pCurNode->Key[i]) return true;
    }
    return false;
}

void BplusTree::insertData(string key) {
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

void BplusTree::deleteData(string key) {
    Node* pCurNode = root;
    while (!pCurNode->getLeaf()) {
        for (int i = 0; i < pCurNode->KeyCnt; i++) {
            if (key < pCurNode->getKey(i)) {
                pCurNode = pCurNode->getChild(i);
                break;
            } else if (i==pCurNode->KeyCnt - 1) {
                pCurNode = pCurNode->getChild(pCurNode->KeyCnt);
                break;
            }
        }
    }
    pCurNode->deleteLeafKey(key);
}

bool BplusTree::doChoice() {
    cout << "insert : 1" << endl;
    cout << "delete : 2" << endl;
    cout << "find key : 3" << endl;
    cout << "print tree : 4" << endl;
    cout << "please do your chioce: ";
    int choice;
    cin >> choice;
    string tmp;
    switch (choice) {
        case 1:
            cout << "please input the key: ";
            cin >> tmp;
            insertData(tmp);
            break;
            
        case 2:
            cout << "please input the key: ";
            cin >> tmp;
            deleteData(tmp);
            break;
        
        case 3:
            cout << "please input the key: ";
            cin >> tmp;
            find(tmp);
            break;
        
        case 4:
            printTree();
        default:
            return false;
            break;
    }
    return true;
}








