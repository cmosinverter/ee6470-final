#include "stdio.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"
#include "math.h"
int x = 5;
int y;

int f(int a, int b) {
	if (a > 0) {
		int ans = a + b;
		return ans;
	}
	a = -a;
	return b + a;
}

int main(int argc, char **argv) {
	char buf[256];
	int a = 5;
	int b = 3;
	b--;
	a = a / b;
	a = a * 2;
	a = f(x, y);
	printf("1 a=%d\n", a);	
	//Demo float print
	float c = 2.0;
	float d = 5.0;
	float e = d/c;
    printf("2 e=%f", e);
	return 0;
}
