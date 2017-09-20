//
//  main.cpp
//  DBMSM2
//
//  Created by CQQ on 2017/9/11.
//  Copyright © 2017年 CQQ. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "BplusTree.h"

using namespace std;

void test();

int main(int argc, const char * argv[]) {   

    //test();
    string a = "a";
    string b = "b";
    cout << (a < b ? "y":"n");
    return 0;
}

void test() {
    BplusTree* bpt = new BplusTree;
    bpt->insert("1.I");
    bpt->insert("2.ll");
    bpt->insert("3.pp");
    bpt->insert("4.");
    bpt->insert("5.");
    bpt->tt();
}

