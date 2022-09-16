#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/sched.h>
#include <pthread.h>
#include <stdint.h>

struct sched_attr {
	uint32_t size;

	uint32_t sched_policy;
	uint64_t sched_flags;

	int32_t sched_nice;
	uint32_t sched_priority;

	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;

	uint32_t sched_util_min;
	uint32_t sched_util_max;

	int32_t sched_latency_nice;
};

int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
	return syscall(274, pid, attr, flags);
}

void attr_init(struct sched_attr *attr)
{
	memset(attr, 0, sizeof(struct sched_attr));
}

static int change_to_rt()
{
	struct sched_attr attr;
	attr_init(&attr);

	attr.size = sizeof(attr);
	attr.sched_policy = SCHED_FIFO;
	attr.sched_priority = 3;
	attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;

	return sched_setattr(0, &attr, 0);
}

static int change_back_to_cfs()
{
	struct sched_attr attr;
	attr_init(&attr);

	attr.size = sizeof(attr);
	attr.sched_policy = SCHED_NORMAL;
	attr.sched_nice = 0;

	return sched_setattr(0, &attr, 0);
}

#define LOOP_NR 100

void test_sched_setattr()
{
	int i;
	int ret;

	for (i = 0; i < LOOP_NR; ++i) {
		ret = change_to_rt();
		if (ret) {
			printf("change to rt failed\n");
		}

		usleep(2000);

		ret = change_back_to_cfs();
		if (ret) {
			printf("change to rt failed\n");
		}
	}
}
