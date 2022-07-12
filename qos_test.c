#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "qos_interface.h"

static void test_qos_apply()
{
	int ret;
	unsigned int cur_ua_flag;
	unsigned int cur_status;

	ret = QosApply(3);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	printf("\033[32m/* -------------- TEST_QOS_APPLY SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_APPLY FAILED!! -------------- */\033[0m\n");
	return;
}

static void test_qos_leave()
{
	int ret;
	unsigned int cur_ua_flag;
	unsigned int cur_status;

	ret = QosLeave();
	if (ret) {
		printf("uid %d qos leave failed\n", getuid());
		goto err;
	}

	printf("\033[32m/* -------------- TEST_QOS_LEAVE SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_LEAVE FAILED!! -------------- */\033[0m\n");
	return;
}

static void basic_qos_test()
{
	printf("0X01: starut BASIC_QOS_TEST\n");
	test_qos_apply();
	test_qos_leave();
	printf("finish BASIC_QOS_TEST!\n");
	printf("\n");
}

void test_qos_manipulate()
{
	int ret;
	int cur_uid;

	ret = setuid(SYSTEM_UID);
	if (ret)
		printf("%s:set uid failed\n", __func__);

	basic_qos_test();
}
