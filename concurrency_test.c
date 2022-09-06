#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include "qos_interface.h"

#define TEST_UID 10486

#define FRAME_TIME_US 16666
#define FRAME_RUN_TIME_US 10000
#define FRAME_SLEEP_TIME_US (FRAME_TIME_US - FRAME_RUN_TIME_US)

#define FG_BG_SWAP_TIME 500000

#define USEC_PER_MSEC 1000

static pthread_t th[10];
bool success = true;

pid_t child_pid = -1;

static void do_frame(int runtime)
{
	int ret;
	clock_t before = clock();
	clock_t delta;
	int cum_run_time;

	ret = QosApply(5);
	if (ret) {
		printf("uid %d qos apply failed\n", getuid());
		success = false;
	}

	do {
		delta = clock() - before;
		cum_run_time = delta * 1000 / CLOCKS_PER_SEC;
	} while (cum_run_time < runtime / USEC_PER_MSEC);

	ret = QosLeave();
	if (ret) {
		printf("uid %d qos leave failed\n", getuid());
		success = false;
	}
}

static void* frameTask(void* arg) {
	int ret;

	sleep(1);

	int index = *(int*)arg;

	while(1) {
		do_frame(FRAME_RUN_TIME_US);
		usleep(FRAME_SLEEP_TIME_US);
	}

	free(arg);
}

static void add_auth_for_start_app(unsigned int uid)
{
	int ret;
	unsigned int ua_flag = AF_RTG_ALL;
	unsigned int status = AUTH_STATUS_FOREGROUND;

	ret = AuthEnable(uid, AF_RTG_ALL, status);
	if (ret) {
		printf("add auth for uid %d failed, cur_uid=%d\n", uid, getuid());
		success = false;
	}
}

static void do_app_start()
{
	int i;
	setuid(TEST_UID);

	for (int i = 0; i < 10; ++i) {
		int* a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &frameTask, a) != 0) {
			perror("Failed to created thread");
			success = false;
		}
	}

	for (i = 0; i < 10; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join thread");
		}
	}

	perror("all thread joined\n");

	exit(0);
}

static void on_app_start()
{
	int i;

	add_auth_for_start_app(TEST_UID);

	child_pid = fork();
	if (child_pid < 0)
		printf("fork fail\n");

	/* child process, simulate a mutl-thread do frame app */
	if (child_pid == 0)
		do_app_start();

	/* father process, the SYSTEM */
	if (child_pid)
		return;
}

static void on_app_dead()
{
	kill(child_pid, SIGTERM);

	AuthDelete(TEST_UID);
}

void app_swap_to_backgroud(unsigned int uid)
{
	int ret;

	ret = AuthPause(uid);
	if (ret) {
		printf("auth pause failed\n");
		success = false;
	}
}

void app_swap_to_front(unsigned int uid)
{
	int ret;

	//ret = AuthEnable(uid, AF_RTG_ALL, AUTH_STATUS_FOREGROUND);
	ret = AuthSwitch(uid, AF_RTG_ALL, AF_QOS_ALL, AUTH_STATUS_FOREGROUND);
	if (ret) {
		printf("auth switch resume failed\n");
		success = false;
	}
}

static void test_multithread_app() {
	int times = 10;
	setuid(SYSTEM_UID);

	on_app_start();

	usleep(5000000);

	while (times--) {
		app_swap_to_backgroud(TEST_UID);
		usleep(FG_BG_SWAP_TIME);
		app_swap_to_front(TEST_UID);
	}

	on_app_dead();

	if (success == false) {
		printf("\033[31m/* --------------  MULTI-THREAD-APP FAILED!! -------------- */\033[0m\n");
		return;
	}

	printf("\033[32m/* --------------  MULTI-THREAD-APP SUCCEED!! -------------- */\033[0m\n");
}

void test_concurrency()
{
	printf("0X03: start CONCURRENCY_TEST\n");
	test_multithread_app();
	printf("finish CONCURRENCY_TEST!\n");
	printf("\n");
}
