#include "ProcessNotification.h"
#include "UnicodeString.h"
#include "File.h"
#include "FsFilter.h"


namespace ProcessNotification
{

	bool registerProcessNotify() {
		NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(processNotification, FALSE);
		return NT_SUCCESS(status);
	}

	void unregisterProcessNotify() {
		PsSetCreateProcessNotifyRoutineEx(processNotification, TRUE);
	}

	void processNotification(PEPROCESS process,
							 HANDLE processId,
							 PPS_CREATE_NOTIFY_INFO createInfo)
	{
		UNREFERENCED_PARAMETER(processId);
		UNREFERENCED_PARAMETER(process);

		if (createInfo) {
			UnicodeString processPath(MAX_PATH);
			if (processPath.init()) {
				processPath.copyFrom(createInfo->ImageFileName);
				if (checkIsVirus(processPath)) {
					createInfo->CreationStatus = STATUS_VIRUS_INFECTED;
				}
			}
		}
	}
	
	bool checkIsVirus(UnicodeString& processPath) {
		
		File file;
		if (!file.init(processPath)) {
			return false;
		}
		
		UINT32 fileSize = 0;
		if (!file.getFileSize(fileSize)) {
			return false;
		}
		
		const UINT32 READ_SIZE = 0x1000;
		UINT32 readSize = READ_SIZE;

		Buffer buffer;
		if (!buffer.init(READ_SIZE)) {
			return false;
		}
		
		for (LONGLONG chunk = 0; chunk < (fileSize/ readSize); ++chunk) {
			if (!file.read(buffer)) {
				return false;
			}
			
			if (searchInBuffer(buffer)) {
				return true;
			}
		}
		readSize = fileSize % READ_SIZE;

		Buffer buffer2;
		if (!buffer2.init(readSize)) {
			return false;
		}
		if (!file.read(buffer2)) {
			return false;
		}
		
		if (searchInBuffer(buffer2)) {
			return true;
		}

		return false;
	}

	bool searchInBuffer(const Buffer& buffer) {
		const char virus[] = "virus";
		for (ULONG i = 0; i < (buffer.size() - _ARRAYSIZE(virus)); ++i) {
			if (RtlCompareMemory((char*)buffer.get() + i, virus, _ARRAYSIZE(virus)) == _ARRAYSIZE(virus)) {
				return true;
			}
		}
		return false;
	}

}



