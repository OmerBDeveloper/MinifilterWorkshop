#pragma once
#include <ntifs.h>
#include "UnicodeString.h"

typedef struct
{
	UINT32 processIndex;
	PEPROCESS processes[65000];
} ProcessArray;

extern ProcessArray g_processArray;

#define MAX_PATH 512

namespace ProcessNotification 
{
	bool registerProcessNotify();
		

	void unregisterProcessNotify();

	void processNotification(
		PEPROCESS process,
		HANDLE processId,
		PPS_CREATE_NOTIFY_INFO createInfo
	);

	bool checkIsVirus(UnicodeString& processPath);

	bool searchInBuffer(const void* buffer, UINT32 bufferSize);
};


