#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct block{
	int y; 
}BLOCK;
BLOCK B[4];

int main(){
	
	int width=17;
	int block_n=4; 
	int k=0;	//只是一个临时的变量 
	
	srand((unsigned)time(NULL));
	while(k<block_n){
		int i=0;
		B[k].y=rand()%(width-1);
		for(i=0;i<k;i++){
			if(B[i].y==B[k].y)	break;
		}
		if(i<k)	continue;	//直接跳过k++ 
		
		k++;
	}
	
	for(int i=0;i<block_n;i++){
		printf("%d\n",B[i].y);
	}
}

