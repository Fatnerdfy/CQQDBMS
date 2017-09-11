//
//  main.cpp
//  DBMSM1
//
//  Created by CQQ on 2017/9/9.
//  Copyright © 2017年 CQQ. All rights reserved.
//

// disk size must larger than memory size
// do not consider LRU algorithm

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define OS_Block_Size 4096 // OS Allocation Block Size
#define Empty_Space_ratio 0.3 // Empty Space Ratio
#define FilePath "test.txt"

using namespace std;

struct header_info {
    int address;
    int size;
};

// when the block isn't full write back to disk then reload to add data

int disk_size = 100; // changeable
int memory_size = 10000; // changeable

int block_no = 0; // No. of the memory block
int disk_head_address = 0; // when write data to disk

int counter = 0; // count the num of record(data)
const int counter_length = 5; // save block head infomation(counter)

int Block_Size = OS_Block_Size * 1;
int header_address = counter_length;
int current_address = Block_Size - 2;
const int header_info_length = 15;

char* allocate_memory(int size);
struct header_info get_header_info(char* p);
bool write_data(char* p, int block_no, char* data  = "test string");
bool write_disk(FILE* fd, char* p);
FILE* allocate_disk();
void test();

int main(int argc, const char * argv[]) {
    test();
    return 0;
}

// diskutil info / | grep "Block Size"
// Device Block Size 512Bytes
// Allocation Block Size 4096Bytes
char* allocate_memory(int size) {
    char* p;
    int block_num = size / Block_Size;
    if(!block_num) {
        printf("Not enough memory\n");
        return NULL;
    } else {
        p = (char*)malloc(block_num * Block_Size);
        memset(p, '-', Block_Size*block_num);
        printf(p == NULL ? "Allocate memory failed\n" : "Allocate memory success\n");
        *(p+Block_Size-1) = '#'; // waiting for delete
    }
    return p;
}

struct header_info get_header_info(char* p) {
    header_info hi;
    hi.size = (int)strlen(p);
    hi.address = current_address - hi.size + 1;
    return hi;
}

bool write_data(char* p, int block_no, char* data) {
    if(current_address - header_address < Block_Size * Empty_Space_ratio) {
        printf("Achieve the limitation of empty space\n");
        return 0;
    } else {
        header_info hi = get_header_info(data);
        strncpy(p+block_no*Block_Size+hi.address, data, strlen(data)); // write data
        current_address = hi.address - 1; // update current_address
        // write header info
        char tmp[header_info_length];
        sprintf(tmp,"%d\t%d\t", hi.address, hi.size);
        strncpy(p+block_no*Block_Size+header_address, tmp, strlen(tmp));
        header_address += header_info_length;
        
        counter += 1;
        
        char count[counter_length+1];
        sprintf(count, "%d", counter);
        strncpy(p+block_no*Block_Size, count, strlen(count));
    }
    return 1;
}

bool write_disk(FILE* fd, char* p) {
    // allocate disk space
    if(fd==NULL) {
        printf("open file fail\n");
        return 0;
    } else {
        if(disk_head_address+Block_Size*block_no+1 < disk_size*1024) {
            fseek(fd, disk_head_address, SEEK_SET);
            fwrite(p, Block_Size, block_no+1, fd);
            disk_head_address += Block_Size;
            fseek(fd, disk_size*1024, SEEK_SET);
            fprintf(fd,"\t");
        } else {
            printf("Disk is full\n");
            return 0;
        }
    }
    printf("write data to disk\n");
    return 1;
}

// size kbytes
FILE* allocate_disk() {
    FILE* fd = fopen(FilePath, "w+");
    if(fd==NULL) {
        return NULL;
    } else {
        fseek(fd, disk_size*1024, SEEK_SET);
        fprintf(fd,"\t");
        return fd;
    }
}

void set_param() {
    counter = 0;
    block_no += 1;
    header_address = counter_length;
    current_address = Block_Size - 2;
}

void write_block_head(char* p, int block_no) {
    
}

void test() {
    FILE* fd = allocate_disk();
    char* p = allocate_memory(memory_size);
    if(p==NULL) {
        printf("allocate memory failed\n");
    } else {
        // write data to block 0
        while(true) {
            if(!write_data(p, block_no)) {
                break;
            }
        }
        set_param();
        *(p+Block_Size*block_no-1) = '#';
        // write data to block 1
        while(true) {
            if(!write_data(p, block_no, "hello world")) {
                break;
            }
        }
        puts(p);
        write_disk(fd, p);
    }
    fclose(fd);
}
