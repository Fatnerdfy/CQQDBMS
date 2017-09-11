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

int disk_size = 100; // changeable
int memory_size = 10000; // changeable

int block_no = 0; // No. of the memory block
int disk_head_address = 0; // when write data to disk

int counter = 0; // count the num of record(data)
const int counter_length = 20; // save block head infomation(counter)

int Block_Size = OS_Block_Size * 1;
int header_address = counter_length;
int current_address = Block_Size - 2;
const int header_info_length = 15;

char* allocate_memory(int size);
struct header_info get_header_info(char* p);
bool write_data(char* p, int block_no, char* data  = "test string");
bool write_disk(FILE* fd, char* p, int block_num);
FILE* allocate_disk();
void test();
void test1();
void test2();

int main(int argc, const char * argv[]) {
    test1();
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
        char count[counter_length+1];
        sprintf(count, "%d\t%d\t%d\t%d\t", counter, 1, header_address, current_address); // 1 is this block full
        strncpy(p+block_no*Block_Size, count, strlen(count));
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
        // 0 is this block not full
        sprintf(count, "%d\t%d\t%d\t%d\t", counter, 0, header_address, current_address);
        strncpy(p+block_no*Block_Size, count, strlen(count));
    }
    return 1;
}

bool write_disk(FILE* fd, char* p, int block_num) {
    // allocate disk space
    if(fd==NULL) {
        printf("open file fail\n");
        return 0;
    } else {
        if(disk_head_address+Block_Size*block_num < disk_size*1024) {
            fseek(fd, disk_head_address, SEEK_SET);
            fwrite(p, Block_Size, block_num, fd);
            disk_head_address += Block_Size*block_num;
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

void reload_block(FILE* fd, char* p) {
    fseek(fd, 0, SEEK_SET);
    fread(p, Block_Size, 2, fd);
    char num[10];
    int datanum = atoi(p+Block_Size);
    sprintf(num, "%d", datanum);
    header_address = atoi(p+Block_Size+strlen(num)+3);
    int header_address_len = strlen(num);
    memset(num, '\0', 10);
    sprintf(num, "%d", header_address);
    current_address = atoi(p+Block_Size+header_address_len+strlen(num)+4);
    
    disk_head_address -= Block_Size;
}

void test() {
    FILE* fd = allocate_disk();
    char* p = allocate_memory(memory_size);
    if(p==NULL) {
        return ;
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
        write_disk(fd, p, block_no+1);
    }
    fclose(fd);
}

// write half of the block and write this block to disk
// then reload this block from disk and add new data
void test1() {
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
        // write data to block 1 & write half of the block
        for (int i = 0; i < 50; i++) {
            write_data(p, block_no, "hello world");
        }
        puts(p);
        write_disk(fd, p, block_no+1);
        /* test if p is clear
        printf("\n\n==========================================\n\n");
        memset(p, '-', Block_Size*2);
        puts(p);        
        */
        printf("\n\n==========================================\n\n");
        reload_block(fd, p);
        //puts(p); // test reload success!
        //printf("\n\n%d %d\n\n", header_address, current_address);
        while (true) {
            if (!write_data(p, 1, "half block test")) {
                break;
            }
        }
        puts(p);
        write_disk(fd, p+Block_Size, 1);
    }
    fclose(fd);
}
