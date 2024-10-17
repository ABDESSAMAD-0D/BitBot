#include <stdio.h>

int main(void){

int a = 10;
int b = 20;
int c;

int *p1 = &a;
int *p2 = &b;

c = *p1;
*p1 = *p2;
*p2 = c;

printf("%d\n", a);
	return (0);
}
