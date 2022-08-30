#define _GNU_SOURCE
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
	int i;

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


#define APPLY_ITER_NUM 10
static void test_qos_apply()
{
	int ret;
	int i;

	for (i = 0; i < APPLY_ITER_NUM; ++i) {
		ret = QosApply(5);
		if (ret) {
			printf("tid %d qos apply failed\n", gettid());
			goto err;
		}

		ret = QosApply(4);
		if (ret) {
			printf("tid %d qos apply failed\n", gettid());
			goto err;
		}

		ret = QosApply(3);
		if (ret) {
			printf("tid %d qos apply failed\n", gettid());
			goto err;
		}

		ret = QosApply(2);
		if (ret) {
			printf("tid %d qos apply failed\n", gettid());
			goto err;
		}

		ret = QosApply(1);
		if (ret) {
			printf("tid %d qos apply failed\n", gettid());
			goto err;
		}
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

static void test_qos_apply_leave_for_same_process()
{
	int ret;

	pthread_t child;

	ret = pthread_create(&child, NULL, &long_last_child, NULL);
	if (ret) {
		printf("child thread create failed\n");
		goto err;
	}

	usleep(500000);


	ret = QosApplyForOther(4, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	ret = QosApplyForOther(4, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	ret = QosApplyForOther(5, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	ret = QosApplyForOther(3, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	ret = QosApplyForOther(5, child_tid);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		goto err;
	}

	usleep(500000);

	ret = QosLeaveForOther(child_tid);
	if (ret) {
		printf("uid %d qos leave failed\n", getuid());
		goto err;
	}

	usleep(500000);

	ret = pthread_cancel(child);
	if (ret) {
		printf("cancel failed %d\n", child_tid);
		goto err;
	}

	printf("\033[32m/* -------------- TEST_QOS_APPLY_FOR_SAME_THREAD SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_APPLY_FOR_SAME_THREAD FAILED!! -------------- */\033[0m\n");
	return;
}

int other_pid = -1;

#define TEST_UID 12345

static void add_auth_for_child_process()
{
	int ret;

	ret = AuthEnable(TEST_UID, AF_RTG_ALL, AUTH_STATUS_FOREGROUND);
	if (ret) {
		printf("auth status\n");
	}
}

void process_do_loop()
{
	setuid(TEST_UID);
	while(1);
}

static void test_qos_apply_leave_for_other_process()
{
	int ret;

	add_auth_for_child_process();

	other_pid = fork();
	if (other_pid < 0)
		printf("fork fail\n");

	/* child process, do while */
	if (other_pid == 0)
		process_do_loop();

	usleep(500000);

	/* father process, the SYSTEM */
	if (other_pid)
		ret = QosApplyForOther(4, other_pid);

	if (ret) {
		printf("%d(uid %d)apply qos for pid %d (tid)failed\n",
		        gettid(), getuid(), other_pid);
		kill(other_pid, SIGTERM);
		goto err;
	}

	usleep(500000);

	ret = QosLeaveForOther(other_pid);

	usleep(500000);

	kill(other_pid, SIGTERM);

	printf("\033[32m/* -------------- TEST_QOS_APPLY_FOR_OTHER_PROCESS_THREAD SUCCED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_QOS_APPLY_FOR_OTHER_PROCESS_THREAD FAILED!! -------------- */\033[0m\n");
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
	int i;
	printf("0X01: starut BASIC_QOS_TEST\n");

	test_qos_apply_leave_for_same_process();
	test_qos_apply_leave_for_other_process();

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

	qos_policy_test();
	basic_qos_test();
}
