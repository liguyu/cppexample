#include <stdio.h>
#include "SingleLinkList.h"

int main(void)
{
	SLinkList sll[5];
	sll[0].add(1);
	sll[0].add(2);
	sll[0].print();
	return 0;
}