#include<stdio.h>
#include<time.h>
int main(){
        int a = 2;
        int b = 50;
	time_t time1 = time(NULL);
	
	time_t time2 = time(NULL);
        int index = (b>>4)&(~(1<<a));
       	//printf("%d\n",1<<4);
	if(difftime(time1,time2)<0){
		printf("1\n");
	}
	else{
		printf("2\n");
	}
        return 0;

}
