//
//  main.cpp
//  DBMSM2
//
//  Created by CQQ on 2017/9/11.
//  Copyright © 2017年 CQQ. All rights reserved.
//

#include <queue>
#include <iostream>
#include <algorithm>
#include "BplusTree.h"

using namespace std;

void tt();
void test();

int main(int argc, const char * argv[]) {   

    //tt();
    test();
    return 0;
}

void test() {
    BplusTree* bpt = new BplusTree;
    string tmp;
    int num = 3;
    while(num--) {
        cin >> tmp;
        bpt->insertData(tmp);
    }
    bpt->printTree();
    //while (bpt->doChoice()) {
    //    ;
    //}
    cin >> tmp;
    bpt->deleteData(tmp);
    cin >> tmp;
    bpt->deleteData(tmp);
    bpt->printTree();
}


