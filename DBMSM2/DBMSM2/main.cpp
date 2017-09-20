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

void p();
void test();

int main(int argc, const char * argv[]) {   
    // insert code here...
    std::cout << "Hello, Worlddd!\n";
    test();
    return 0;
}

void p() {
    string l[3];
    l[0] = "ad";
    l[1] = "io";
    cout << l << endl;
}
void test() {
    BplusTree* bpt = new BplusTree;
    bpt->insert("1.I");
    bpt->insert("2.love");
    bpt->insert("3.you");
    bpt->tt();
}

