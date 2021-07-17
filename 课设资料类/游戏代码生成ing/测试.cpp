#include<stdio.h>
int a,b;
int panduan();
int main(){
	a=1;b=2;
	int c=panduan();
	printf("%d",c);
	return 0;
}
int panduan(){
	return b>a;
}
