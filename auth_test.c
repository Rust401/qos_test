#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "qos_interface.h"

#define MAX_TEST_UID_NR 10
unsigned int uid_list[MAX_TEST_UID_NR] = {
	10086,
	10087,
	10088,
	10089,
	10090,
	10091,
	10092,
	10093,
	10094,
	10095,
};

static void clean_up_idr()
{
	for (int i = 0; i < MAX_TEST_UID_NR; ++i)
		AuthDelete(uid_list[i]);
}

static void test_auth_add()
{
	int ret;
	int i;
	unsigned int ua_flag = AF_RTG_ALL;
	unsigned int status = AUTH_STATUS_FOREGROUND;

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthEnable(uid_list[i], AF_RTG_ALL, status);
		if (ret) {
			printf("add auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	printf("\033[32m/* -------------- TEST_AUTH_ENABLE SUCCEED!! -------------- */\033[0m\n");
	return;

err:
	while(i >= 0) {
		ret = AuthDelete(uid_list[i]);
		i--;
		if (ret) {
			printf("delete auth for uid %d failed\n", uid_list[i]);
			continue;
		}
	}

	printf("\033[31m/* -------------- TEST_AUTH_ENABLE FAILED!! -------------- */\033[0m\n");
	return;
}

static void test_auth_delete()
{
	int ret;
	int i;

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthDelete(uid_list[i]);
		if (ret) {
			printf("delete auth for uid %d failed\n", uid_list[i]);
			break;
		}
	}
	if (ret)
		goto err;

	printf("\033[32m/* -------------- TEST_AUTH_DELETE SUCCEED!! -------------- */\033[0m\n");
	return;

err:
	printf("\033[31m/* -------------- TEST_AUTH_DELETE FAILED!! -------------- */\033[0m\n");
	return;
}

static void test_auth_pause()
{
	int ret;
	int i;
	unsigned int ua_flag = AF_RTG_ALL;
	unsigned int status = AUTH_STATUS_FOREGROUND;

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthEnable(uid_list[i], AF_RTG_ALL, status);
		if (ret) {
			printf("add auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthPause(uid_list[i]);
		if (ret) {
			printf("pause auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthEnable(uid_list[i], AF_RTG_ALL, status);
		if (ret) {
			printf("re-enable auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	printf("\033[32m/* -------------- TEST_AUTH_PAUSE SUCCEED!! -------------- */\033[0m\n");
	goto out;

err:
	printf("\033[31m/* -------------- TEST_AUTH_PAUSE FAILED!! -------------- */\033[0m\n");
out:
	clean_up_idr();
	return;
}

static inline bool value_changed(unsigned int cur_ua_flag, unsigned cur_status)
{
	return cur_ua_flag != AF_RTG_DELEGATED ||
	       cur_status != AUTH_STATUS_FOREGROUND;
}

static void test_auth_get()
{
	int ret;
	int i;
	unsigned int ua_flag = AF_RTG_ALL;
	unsigned int status = AUTH_STATUS_FOREGROUND;

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthEnable(uid_list[i], AF_RTG_ALL, status);
		if (ret) {
			printf("add auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		unsigned int cur_ua_flag;
		unsigned int cur_status;
		ret = AuthGet(uid_list[i], &cur_ua_flag, &cur_status);
		if (ret || value_changed(cur_ua_flag, cur_status)) {
			printf("get auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	printf("\033[32m/* -------------- TEST_AUTH_GET SUCCED!! -------------- */\033[0m\n");
	goto out;

err:
	printf("\033[31m/* -------------- TEST_AUTH_GET FAILED!! -------------- */\033[0m\n");
out:
	clean_up_idr();
	return;
}

static void test_auth_switch()
{
	int ret;
	int i;
	unsigned int ua_flag = AF_RTG_ALL;
	unsigned int qos_flag = AF_QOS_ALL;
	unsigned int status = AUTH_STATUS_FOREGROUND;

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthEnable(uid_list[i], AF_RTG_ALL, status);
		if (ret) {
			printf("add auth for uid %d failed\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthSwitch(uid_list[i], AF_RTG_ALL, AF_QOS_ALL, AUTH_STATUS_SYSTEM_SERVER);
		if (ret) {
			printf("switch auth for uid %d failed 0\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthSwitch(uid_list[i], 0, 0, AUTH_STATUS_BACKGROUND);
		if (ret) {
			printf("switch auth for uid %d failed 1\n", uid_list[i]);
			goto err;
		}
	}

	for (i = 0; i < MAX_TEST_UID_NR; ++i) {
		ret = AuthSwitch(uid_list[i], 2, 2, AUTH_STATUS_FOREGROUND);
		if (ret) {
			printf("switch auth for uid %d failed 2\n", uid_list[i]);
			goto err;
		}
	}

	printf("\033[32m/* -------------- TEST_AUTH_SWITCH SUCCED!! -------------- */\033[0m\n");
	goto out;

err:
	printf("\033[31m/* -------------- TEST_AUTH_SWTICH FAILED!! -------------- */\033[0m\n");
out:
	clean_up_idr();
	return;
}


static void basic_auth_test()
{
	printf("0X00: starut BASIC_AUTH_TEST\n");
	test_auth_add();
	test_auth_delete();
	test_auth_pause();
	test_auth_get();
	test_auth_switch();
	printf("finish BASIC_AUTH_TEST!\n");
	printf("\n");
}

void test_auth_manipulate()
{
	int ret;
	int cur_uid;

	ret = setuid(SYSTEM_UID);
	if (ret)
		printf("%s:set uid failed\n", __func__);

	basic_auth_test();
}
