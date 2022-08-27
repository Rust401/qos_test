#ifndef QOS_INTERFACE_H
#define QOS_INTERFACE_H
#include <stdbool.h>

/*
 * generic
 */
#define SYSTEM_UID 1000
#define ROOT_UID 0

/*
 * auth_ctrl
 */
struct auth_ctrl_data {
	unsigned int uid;
	unsigned int type;
	unsigned int rtg_ua_flag;
	unsigned int qos_ua_flag;
	unsigned int status;
};

enum auth_manipulate_type {
	AUTH_ENABLE = 1,
	AUTH_PAUSE,
	AUTH_DELETE,
	AUTH_GET,
	AUTH_MAX_NR,
};

enum auth_status {
	AUTH_STATUS_DISABLED = 1,
	AUTH_STATUS_FOREGROUND,
	AUTH_STATUS_BACKGROUND,
	AUTH_STATUS_SYSTEM_SERVER,
	AUTH_STATUS_DEAD,
};

enum auth_ctrl_cmdid {
	BASIC_AUTH_CTRL = 1,
	AUTH_CTRL_MAX_NR
};

#define AUTH_CTRL_IPC_MAGIG	0xCD

#define	BASIC_AUTH_CTRL_OPERATION \
	_IOWR(AUTH_CTRL_IPC_MAGIG, BASIC_AUTH_CTRL, struct auth_ctrl_data)


/*
 * qos ctrl
 */
#define NR_QOS 5
#define QOS_NUM_MAX 10

#define AF_QOS_ALL		0x0003
#define AF_QOS_DELEGATED	0x0001

enum qos_manipulate_type {
	QOS_APPLY = 1,
	QOS_LEAVE,
	QOS_MAX_NR,
};

struct qos_ctrl_data {
	int pid;
	unsigned int type;
	unsigned int level;
};

struct qos_policy_data {
	int nice;
	int latency_nice;
	int uclamp_min;
	int uclamp_max;
};

enum qos_policy_type {
	QOS_POLICY_DEFAULT = 1,
	QOS_POLICY_FRONT,
	QOS_POLICY_BACK,
	QOS_POLICY_SYSTEM_SERVER,
	QOS_POLICY_MAX_NR,
};

struct qos_policy_datas {
	int policy_type;
	struct qos_policy_data policys[NR_QOS + 1];
};

enum qos_ctrl_cmdid {
	QOS_CTRL = 1,
	QOS_POLICY,
	QOS_CTRL_MAX_NR
};

#define QOS_CTRL_IPC_MAGIG	0xCC

#define QOS_CTRL_BASIC_OPERATION \
	_IOWR(QOS_CTRL_IPC_MAGIG, QOS_CTRL, struct qos_ctrl_data)
#define QOS_CTRL_POLICY_OPERATION \
	_IOWR(QOS_CTRL_IPC_MAGIG, QOS_POLICY, struct qos_policy_datas)

/*
 * RTG
 */
#define AF_RTG_ALL		0x1fff
#define AF_RTG_DELEGATED	0x1fff

struct rtg_enable_data {
    int enable;
    int len;
    char *data;
};

enum rtg_sched_cmdid {
    SET_ENABLE = 1,
    SET_RTG,
    SET_CONFIG,
    SET_RTG_ATTR,
    BEGIN_FRAME_FREQ = 5,
    END_FRAME_FREQ,
    END_SCENE,
    SET_MIN_UTIL,
    SET_MARGIN,
    LIST_RTG = 10,
    LIST_RTG_THREAD,
    SEARCH_RTG,
    GET_ENABLE,
    RTG_CTRL_MAX_NR,
};

#define RTG_SCHED_IPC_MAGIC	0xAB

#define CMD_ID_SET_ENABLE \
    _IOWR(RTG_SCHED_IPC_MAGIC, SET_ENABLE, struct rtg_enable_data)

/*
 * interface
 */
int EnableRtg(bool flag);
int AuthEnable(unsigned int uid, unsigned int ua_flag, unsigned int status);
int AuthPause(unsigned int uid);
int AuthDelete(unsigned int uid);
int AuthGet(unsigned int uid, unsigned int *ua_flag, unsigned int *status);
int QosApply(unsigned int level);
int QosApplyForOther(unsigned int level, int tid);
int QosLeave();
int QosLeaveForOther(int tid);
int QosPolicy(struct qos_policy_datas *policy_datas);

#endif /* OQS_INTERFACE_H */
