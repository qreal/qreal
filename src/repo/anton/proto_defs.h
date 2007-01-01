#ifndef STORAGE_PROTO
#define STORAGE_PROTO

#define TIME_LIMIT 2*5*180 // 3 minutes (1 second = 5)
#define MAX_PROJ_NAME_LEN 20
#define MAX_DESCRIPTION_LEN 50

struct __permissions {
	unsigned long userId;
	int userP;
	//unsigned long groupId;
	int groupP;
	int othersP;
};

typedef struct __permissions permissions;

#define PERMS_NONE -1
#define PERMS_READ 0
#define PERMS_RDWR 1

#define WANT_READ 0
#define WANT_WRITE 1

enum error {
	errorNoError, errorTimeOut, errorUndefined, errorWasInSync,
	errorForbidden, errorOwnerForbidden, 
	errorNoSuchUser, errorNoSuchObject, errorInterruptedByOwner,
	errorHasWriteLock, errorIOError, errorNotimplemented
};

struct __st_packet {
	int cmd;
	int errCode;
	int data_len;
	char data[4096];
};

struct __st_project {
	unsigned int id;
	char name[MAX_PROJ_NAME_LEN + 1];
	char descr[MAX_DESCRIPTION_LEN + 1];
	int perms;
};

typedef struct __st_packet st_packet;
typedef struct __st_project st_project;

#define CMD_LOGIN			1
#define CMD_LOGOUT			2
#define CMD_PING			3
#define CMD_LIST_PROJECTS	4
#define CMD_OPEN_PROJECT	5
#define CMD_CLOSE_PROJECT	6
#define CMD_CREATE_OBJECT	7
#define CMD_LOAD_OBJECT		8
#define CMD_STORE_OBJECT	9
#define CMD_CHECK_PERMS		10
#define CMD_LOCK_OBJECT		11
#define CMD_UNLOCK_OBJECT   12

#define RPL_FAILED			13
#define RPL_LOST_LOCK		14

#endif
