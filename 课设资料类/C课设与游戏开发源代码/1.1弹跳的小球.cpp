#include<stdlib.h> 
#include<stdio.h>
int main(){
	int y=5,x=0;
	int left=0,right=20;
	int top=0,bottom=20;
	int v1=1,v2=1;
	
	while(1){
		x+=v1;y+=v2;
		system("cls");
		for(int i=0;i<x;i++){
			printf("\n");
		}
		for(int i=0;i<y;i++){
			printf(" ");
		}
		printf("O");
		printf("\n");
		if(x==top||x==bottom)	{v1=-v1;printf("\a");
		}                                          
		if(y==left||y==right)	{v2=-v2;printf("\a");
		}
	}
	return 0;
}
	
