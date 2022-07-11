#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "qos_interface.h"

extern void test_auth_manipulate();
extern void test_qos_manipulate();
extern void test_concurrency();

int main()
{
	int ret;

	ret = EnableRtg(true);
	if (ret < 0)
		printf("RTG enable failed\n");

	test_auth_manipulate();
	test_qos_manipulate();
	test_concurrency();
}

