#include "stdio.h"

int main(unsigned hart_id) {
	// behave differently based on the core (i.e. hart) id
	int a=0;
	for (unsigned i=0; i<100*hart_id; ++i){
		a=a+1;
	}
	printf("hard-id=%d a=%d\n", hart_id, a);

	return 0;
}
