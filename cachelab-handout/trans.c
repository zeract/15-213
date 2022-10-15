/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M==32&&N==32){
            //将A对角线的数据保存到变量中，防止与B对角线的cache冲突
        int t1[8][8],t2[8][8],t3[8][8],t4[8][8];
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                t1[i][j] = A[i][j];
            }
        }
        for(int i=8;i<16;i++){
            for(int j=8;j<16;j++){
                t2[i-8][j-8] = A[i][j];
            }
        }
        for(int i=16;i<24;i++){
            for(int j=16;j<24;j++){
                t3[i-16][j-16] = A[i][j];
            }
        }
        for(int i=24;i<32;i++){
            for(int j=24;j<32;j++){
                t4[i-24][j-24] = A[i][j];
            }
        }
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                B[j][i]=t1[i][j];
            }
        }
        for(int i=8;i<16;i++){
            for(int j=8;j<16;j++){
            B[j][i]=t2[i-8][j-8];
            }
        }
        for(int i=16;i<24;i++){
            for(int j=16;j<24;j++){
                B[j][i]=t3[i-16][j-16];
            }
        }
        for(int i=24;i<32;i++){
            for(int j=24;j<32;j++){
                B[j][i]=t4[i-24][j-24];
            }
        }
        for(int i=0;i<N;i+=8){
            for(int j=0;j<M;j+=8){
                for(int k=i;k<i+8;k++){
                    for(int p=j;p<j+8;p++){
                        if(i==j){
                            continue;
                        }
                        else{
                            B[k][p] = A[p][k];
                        }
                        
                    }
                }

            }
        }
    }
    else if(M==64&&N==64){
         int temp[4][4];
        //交换对角线上的区域，防止对角线冲突
        for(int i=0,j=0;i<M&&j<N;i+=4,j+=4){
            for(int k=i;k<i+4;k++){
                for(int p=j;p<j+4;p++){
                    if((i/4)%2==0){
                        temp[k-i][p-j] = A[k][p];
                        B[p+4][k] = A[k][p+4];
                    }
                    else{
                          B[p-4][k] = A[k][p-4];
                          temp[k-i][p-j] = A[k][p];

                    }
                }

            }
            for(int k=i;k<i+4;k++){
                for(int p=j;p<j+4;p++){
                    B[p][k] = temp[k-i][p-j];
                }
            }

        }
        
        for(int i=0;i<N;i+=8){
            for(int j=0;j<M;j+=8){
                //左上转置和右上转置
                for(int k=i;k<i+4;k++){
                    for(int p=j;p<j+4;p++){
                        if(i==j){
                            continue;
                        }
                        else{
                            B[p][k] = A[k][p];
                            B[p][k+4] = A[k][p+4]; //先将右上转置到右上
                        }
                        
                    }
                }
                
                    for(int p=j;p<j+4;p++){
                        if(i==j){
                            continue;
                        }
                        else{
                            int x1,x2,x3,x4;
                            //取B右上保存到变量中
                            x1 = B[p][i+4];x2 = B[p][i+5];x3 = B[p][i+6];x4 = B[p][i+7];
                            //将A左下移动到右上
                            B[p][i+4] = A[i+4][p];
                            B[p][i+5] = A[i+5][p];
                            B[p][i+6] = A[i+6][p];
                            B[p][i+7] = A[i+7][p];
                            //将变量移动到B左下
                            B[p+4][i] = x1;
                            B[p+4][i+1] = x2;
                            B[p+4][i+2] = x3;
                            B[p+4][i+3] = x4;
                        }
                        
                    }
                
                for(int k=i+4;k<i+8;k++){
                    for(int p=j+4;p<j+8;p++){
                        if(i==j){
                            continue;
                        }
                        else{
                            B[p][k] = A[k][p];
                        }
                    }
                }
            }
        }
    }
    else{
        int size = 16;
        
        for(int i=0;i<N;i+=size){
            for(int j=0;j<M;j+=size){
                for(int k=i;k<N&&k<i+size;k++){
                    for(int p=j;p<M&&p<j+size;p++){
                        B[p][k] = A[k][p];
                    }
                }
                 
            }
        }

         
          
    
    }
    
    

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

