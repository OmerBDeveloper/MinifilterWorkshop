#pragma once
#include <ntifs.h>


typedef struct
{
	UINT32 processIndex;
	PEPROCESS processes[65000];
} ProcessArray;

extern ProcessArray g_processArray;

#define MAX_PATH 512

namespace ProcessNotification 
{
	void registerProcessNotify();
		

	void unregisterProcessNotify();

	void processNotification(
		PEPROCESS Process,
		HANDLE ProcessId,
		PPS_CREATE_NOTIFY_INFO CreateInfo
	);

	bool checkIsVirus(PEPROCESS Process);
};


