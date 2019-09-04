#include <stdio.h>
typedef struct Ele {
	long val;
	struct	Ele *next;
} *list_ptr;

long rsum_list(list_ptr ls) {
	if (!ls)
		return 0;
	else {
		long val = ls -> val;
		long rest = rsum_list(ls->next);
		return val + rest;
	}
}

int main() {
	struct Ele e3 = {30, 0};
	struct Ele e2 = {20, &e3};
	struct Ele e1 = {10, &e2};
	long s = rsum_list(&e1);
	printf("%ld\n", s);
}
