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
		PEPROCESS process,
		HANDLE processId,
		PPS_CREATE_NOTIFY_INFO createInfo
	);

	bool checkIsVirus(PEPROCESS process);
};


