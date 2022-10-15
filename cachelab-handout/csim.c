#include "cachelab.h"
#include<unistd.h>
#include<getopt.h>
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include<time.h>
#include <assert.h>

struct cache_line{
    int valid;
    int tag;
    int lru;
};





int main(int argc,char **argv)
{
    int hits=0,miss=0,evicts=0;
    int s,e,b,opt;
    char path[255];
    
    while(-1 != (opt = getopt(argc,argv,"s:E:b:t:"))){
        switch(opt){
            case 's':
                s = atoi(optarg);
                assert(s>0);
                break;
            case 'E':
                e = atoi(optarg);
                assert(e>0);
                break;
            case 'b':
                b = atoi(optarg);
                assert(b>0);
                break;
            case 't':
                strcpy(path,optarg);
                break;
            default:
                printf("wrong argument\n");
                break;
        }
    }
    struct cache_line cache[1<<s][e];
    memset(&cache,0,sizeof(cache));
    /*
    time_t tick = time(NULL);   //初始化timstamp
    for(int i=0;i< 1<<s;i++){
        for(int j=0;j<e;j++){
            cache[i][j].lru = tick;
        }
    }
    */
    FILE * pfile;
    pfile = fopen(path,"r");
    char identifier;
    unsigned address;
    int size;
    int tag;
    
    while(fscanf(pfile," %c %x,%d",&identifier,&address,&size)>0){
        if(identifier=='I'){
            continue;
        }
        int flag=0;
        if(address<0){
            continue;
        }
        int index = (address>>b)&((1<<s)-1);
        assert(index<(1<<s));
        int k=0;
        int min = 0; //选择0作为最小值
        switch(identifier){
            case 'L':
                tag = address >>(b+s);  //获取tag
                for(int i=0;i<e;i++){
                    if(cache[index][i].valid==1&&cache[index][i].tag==tag){
                        flag=1;
                        for(int k=0;k< (1<<s);k++){
                            for (int p=0;p<e;p++){
                                if(k==index&&p==i){
                                    continue;
                                }
                                else{
                                    cache[k][p].lru++;
                                    }
                                }
                        }
                        cache[index][i].lru = 0;
                        break;
                    }
                }
                if(flag){
                    hits++;
                }
                else{
                    miss++;
                    for(int i=0;i<e;i++){
                        if(cache[index][i].lru>min){
                            min = cache[index][i].lru;
                            k = i;
                        }
                    }
                    
                    if(cache[index][k].valid!=0){
                        evicts++;
                    }
                    cache[index][k].tag = tag;
                    cache[index][k].lru = 0;
                    cache[index][k].valid = 1;
                }
                break;
            case 'S':
                tag = address >>(b+s);
                for(int i=0;i<e;i++){
                    if(cache[index][i].valid==1&&cache[index][i].tag==tag){
                        flag=1;
                        for(int k=0;k< (1<<s);k++){
                            for (int p=0;p<e;p++){
                                if(k==index&&p==i){
                                    continue;
                                }
                                else{
                                    cache[k][p].lru++;
                                    }
                                }
                        }
                        cache[index][i].lru = 0;
                        break;
                    }
                }
                if(flag){
                    hits++;
                }
                else{
                    miss++;
                    for(int i=0;i<e;i++){
                        if(cache[index][i].lru>min){
                            min = cache[index][i].lru;
                            k = i;
                        }
                    }
                    if(cache[index][k].valid!=0){
                        evicts++;
                    }
                    cache[index][k].tag = tag;
                    cache[index][k].lru = 0;
                    cache[index][k].valid = 1;
                }
                break;
            case 'M':
                tag = address >>(b+s);
                for(int i=0;i<e;i++){
                    if(cache[index][i].valid==1&&cache[index][i].tag==tag){
                        flag=1;
                        for(int k=0;k< (1<<s);k++){
                            for (int p=0;p<e;p++){
                                if(k==index&&p==i){
                                    continue;
                                }
                                else{
                                    cache[k][p].lru++;
                                    }
                                }
                        }
                        cache[index][i].lru = 0;
                        break;
                    }
                }
                if(flag){
                    hits++;
                }
                else{
                    miss++;
                    for(int i=0;i<e;i++){
                        if(cache[index][i].lru>min){
                            min = cache[index][i].lru;
                            k = i;
                        }
                    }
                    if(cache[index][k].valid!=0){  //tag不为0，则进行了evict操作
                        evicts++;
                    }
                    cache[index][k].tag = tag;
                    cache[index][k].lru = 0;
                    cache[index][k].valid = 1;
                }
                for(int i=0;i<e;i++){
                    if(cache[index][i].valid==1&&cache[index][i].tag==tag){
                        flag=1;
                        for(int k=0;k< (1<<s);k++){
                            for (int p=0;p<e;p++){
                                if(k==index&&p==i){
                                    continue;
                                }
                                else{
                                    cache[k][p].lru++;
                                    }
                                }
                        }
                        cache[index][i].lru = 0;
                        break;
                    }
                }
                if(flag){
                    hits++;
                }
                else{
                    miss++;
                    for(int i=0;i<e;i++){
                        if(cache[index][i].lru>min){
                            min = cache[index][i].lru;
                            k = i;
                        }
                    }
                    if(cache[index][k].valid!=0){
                        evicts++;
                    }
                    cache[index][k].tag = tag;
                    cache[index][k].lru = 0;
                    cache[index][k].valid = 1;
                }
                break;
            default:
                break;
        }
    }
    fclose(pfile);
    
    printSummary(hits, miss, evicts);
    return 0;
}
