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

static struct qos_policy_datas default_qos_policy = {
	.policys = {
		{0, 0},
		{10, 10},
		{5, 5},
		{0, 0},
		{-5, -5},
		{-10, -10},
	}
};

static void test_qos_policy(struct qos_policy_datas *policy_datas)
{
	int ret;

	ret = QosPolicy(policy_datas);
	if (ret) {
		printf("uid %d qos leave failed\n", getuid());
		goto err;
	}

	printf("\033[32m/* -------------- TEST_QOS_POLICY SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_POLICY FAILED!! -------------- */\033[0m\n");
	return;
}

static void qos_policy_test()
{
	printf("0X02: starut QOS_POLICY_TEST\n");
	test_qos_policy(&default_qos_policy);
	printf("finish QOS_POLICY_TEST!\n");
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
	qos_policy_test();
}
