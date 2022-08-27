#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "qos_interface.h"

static struct qos_policy_datas default_qos_policy = {
	.policy_type = QOS_POLICY_DEFAULT,
	.policys = {
		{0, 0, 0, 1024},
		{0, 0, 0, 1024},
		{0, 0, 0, 1024},
		{0, 0, 0, 1024},
		{0, 0, 0, 1024},
		{0, 0, 0, 1024},
	}
};

static struct qos_policy_datas foreground_qos_policy = {
	.policy_type = QOS_POLICY_FRONT,
	.policys = {
		{0, 0, 0, 1024},
		{10, 10, 0, 200},
		{5, 5, 0, 250},
		{0, 0, 0, 1024},
		{-5, -5, 300, 1024},
		{-10, -10, 500, 1024},
	}
};

static struct qos_policy_datas background_qos_policy = {
	.policy_type = QOS_POLICY_BACK,
	.policys = {
		{0, 0, 0, 1024},
		{15, 15, 0, 150},
		{10, 10, 0, 200},
		{5, 5, 0, 250},
		{0, 0, 0, 300},
		{-5, -5, 0, 350},
	}
};

static struct qos_policy_datas system_server_qos_policy = {
	.policy_type = QOS_POLICY_SYSTEM_SERVER,
	.policys = {
		{0, 0, 0, 1024},
		{10, 10, 0, 200},
		{5, 5, 0, 250},
		{0, 0, 0, 1024},
		{-5, -5, 300, 1024},
		{-10, -10, 500, 1024},
	}
};

static int set_qos_policy(struct qos_policy_datas *policy_datas)
{
	int ret;

	ret = QosPolicy(policy_datas);
	if (ret)
		printf("%d set_qos_polilcy failed\n", getuid());

	return ret;
}

void test_qos_polilcy()
{
	int ret;
	ret = set_qos_policy(&default_qos_policy);
	if (ret) {
		printf("%d set_qos_polilcy failed 1\n", getuid());
		goto err;
	}

	ret = set_qos_policy(&foreground_qos_policy);
	if (ret) {
		printf("%d set_qos_polilcy failed 2\n", getuid());
		goto err;
	}

	ret = set_qos_policy(&background_qos_policy);
	if (ret) {
		printf("%d set_qos_polilcy failed 3\n", getuid());
		goto err;
	}

	ret = set_qos_policy(&system_server_qos_policy);
	if (ret) {
		printf("%d set_qos_polilcy failed 4\n", getuid());
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
	test_qos_polilcy();
	printf("finish QOS_POLICY_TEST!\n");
	printf("\n");
}



static void test_qos_apply()
{
	int ret;

	ret = QosApply(5);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}
	ret = QosApply(4);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}
	ret = QosApply(3);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}
	ret = QosApply(2);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}
	ret = QosApply(1);
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

int child_tid;

void* long_last_child() {
	child_tid = gettid();
	while(1);
}

static void test_qos_apply_for_others()
{
	int ret;

	pthread_t child;

	ret = pthread_create(&child, NULL, &long_last_child, NULL);
	if (ret) {
		printf("child thread create failed\n");
		goto err;
	}

	usleep(500000);

	printf("child %d created\n", child_tid);

	ret = QosApplyForOther(5, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	printf("qos applied for %d\n", child_tid);

	usleep(500000);


	//kill(child_tid, SIGTERM);

	printf("\033[32m/* -------------- TEST_QOS_APPLY_FOR_OTHER SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_APPLY_FOR_OTHER FAILED!! -------------- */\033[0m\n");
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
	//test_qos_apply();
	test_qos_apply_for_others();
	//test_qos_leave();
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

	qos_policy_test();
	basic_qos_test();
}
