#ifndef RTG_INTERFACE_H
#define RTG_INTERFACE_H
#include <stdbool.h>

struct rtg_enable_data {
    int enable;
    int len;
    char *data;
};

enum grp_ctrl_cmd {
    CMD_CREATE_RTG_GRP,
    CMD_ADD_RTG_THREAD,
    CMD_REMOVE_RTG_THREAD,
    CMD_CLEAR_RTG_GRP,
    CMD_DESTROY_RTG_GRP
};

enum auth_manipulate_type {
	AUTH_ENABLE = 1,
	AUTH_PAUSE,
	AUTH_DELETE,
	AUTH_GET,
	AUTH_MAX_NR,
};

enum rtg_auth_status {
	AUTH_STATUS_CACHED = 0,
	AUTH_STATUS_ENABLE,
};

struct rtg_auth_data {
    unsigned int uid;
    unsigned int type;
    unsigned int ua_flag;
    unsigned int status;
};

struct rtg_qos_data {
    unsigned int type;
    unsigned int level;
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
    AUTH_MANIPULATE,
    QOS_MANIPULATE = 15,
    RTG_CTRL_MAX_NR,
};

#define SYSTEM_UID 1000
#define ROOT_UID 0

#define AF_RTG_ALL 0x7fff
#define AF_RTG_NORMAL_TASK_FULL 0x5fff

int EnableRtg(bool flag);
int AuthEnable(unsigned int uid, unsigned int ua_flag, unsigned int status);
int AuthPause(unsigned int uid);
int AuthDelete(unsigned int uid);
int AuthGet(unsigned int uid, unsigned int *ua_flag, unsigned int *status);
int QosApply(unsigned int level);
int QosLeave();

#endif /* RTG_INTERFACE_H */
