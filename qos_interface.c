#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <stdbool.h>

#include "qos_interface.h"

const char RTG_SCHED_IPC_MAGIC = 0xAB;

#define CMD_ID_SET_ENABLE \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_ENABLE, struct rtg_enable_data)
#define CMD_ID_AUTH_MANIPULATE \
    _IOWR(RTG_SCHED_IPC_MAGIC, AUTH_MANIPULATE, struct rtg_auth_data)
#define CMD_ID_QOS_MANIPULATE \
    _IOWR(RTG_SCHED_IPC_MAGIC, QOS_MANIPULATE, struct rtg_qos_data)

static int trival_open_rtg_node()
{
	char fileName[] = "/proc/self/sched_rtg_ctrl";
	int fd = open(fileName, O_RDWR);
	if (fd < 0)
		printf("task %d belong to user %d open rtg node failed\n", getpid(), getuid());

	return fd;
}

int EnableRtg(bool flag)
{
	struct rtg_enable_data enableData;
	char configStr[] = "load_freq_switch:1;sched_cycle:1";

	enableData.enable = flag;
	enableData.len = sizeof(configStr);
	enableData.data = configStr;
	int fd = trival_open_rtg_node();
	if (fd < 0)
		return fd;

	if (ioctl(fd, CMD_ID_SET_ENABLE, &enableData))
		printf("set rtg config enable failed.\n");

	close(fd);

	return 0;
};

int AuthEnable(unsigned int uid, unsigned int ua_flag, unsigned int status)
{
	struct rtg_auth_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0)
		return fd;

	data.uid = uid;
	data.ua_flag = ua_flag;
	data.status = status;
	data.type = AUTH_ENABLE;

	ret = ioctl(fd, CMD_ID_AUTH_MANIPULATE, &data);
	if (ret < 0)
		printf("auth enable failed for uid %d with status %d\n", uid, status);

	close(fd);
	return ret;
}

int AuthDelete(unsigned int uid)
{
	struct rtg_auth_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0) {
		return fd;
	}

	data.uid = uid;
	data.type = AUTH_DELETE;

	ret = ioctl(fd, CMD_ID_AUTH_MANIPULATE, &data);
	if (ret < 0)
		printf("auth delete failed for uid %d\n", uid);

	close(fd);
	return ret;
}

int AuthPause(unsigned int uid)
{
	struct rtg_auth_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0) {
		return fd;
	}

	data.uid = uid;
	data.type = AUTH_PAUSE;

	ret = ioctl(fd, CMD_ID_AUTH_MANIPULATE, &data);
	if (ret < 0)
		printf("auth pause failed for uid %d\n", uid);

	close(fd);
	return ret;
}

int AuthGet(unsigned int uid, unsigned int *ua_flag, unsigned int *status)
{
	struct rtg_auth_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0) {
		return fd;
	}

	data.uid = uid;
	data.type = AUTH_GET;

	ret = ioctl(fd, CMD_ID_AUTH_MANIPULATE, &data);
	if (ret < 0)
		printf("auth get failed for uid %d\n", uid);

	close(fd);

	*ua_flag = data.ua_flag;
	*status = data.status;

	return ret;
}

int QosApply(unsigned int level)
{
	struct rtg_qos_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0) {
		return fd;
	}

	data.level = level;
	data.type = 1;

	ret = ioctl(fd, CMD_ID_QOS_MANIPULATE, &data);
	if (ret < 0)
		printf("qos apply failed for task %d\n", getpid());

	close(fd);
	return ret;
}

int QosLeave()
{
	struct rtg_qos_data data;
	int fd;
	int ret;

	fd = trival_open_rtg_node();
	if (fd < 0) {
		return fd;
	}

	data.type = 2;

	ret = ioctl(fd, CMD_ID_QOS_MANIPULATE, &data);
	if (ret < 0)
		printf("qos leave failed for task %d\n", getpid());

	close(fd);
	return ret;
}
