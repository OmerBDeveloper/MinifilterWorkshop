#pragma once
#include <ntifs.h>
#include "UnicodeString.h"
#include "Buffer.h"

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

	bool searchInBuffer(const Buffer& buffer);
};


