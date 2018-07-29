#include "ProcessNotification.h"
#include "UnicodeString.h"
#include "FsFilter.h"

ProcessArray g_processArray;

namespace ProcessNotification
{

	bool registerProcessNotify() {
		RtlZeroMemory(&g_processArray, sizeof(ProcessArray));
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
		HANDLE fileHandle = NULL;
		void* buffer = NULL;
		OBJECT_ATTRIBUTES fileAttributes;
		NTSTATUS status;
		bool returnValue = false;
		InitializeObjectAttributes(&fileAttributes,
								   processPath.getUnicodeString(),
								   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
								   NULL, // RootDirectory
								   NULL // SecurityDescriptor
								   );
		IO_STATUS_BLOCK ioStatus;

		status = ZwCreateFile(&fileHandle,
							  FILE_READ_DATA,
							  &fileAttributes,
							  &ioStatus,
							  NULL,
							  FILE_ATTRIBUTE_NORMAL,
							  FILE_SHARE_READ,
							  FILE_OPEN,
							  FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
							  NULL,
							  0);
		if (!NT_SUCCESS(status)) {
			returnValue = false;
			goto cleanup;
		}

		FILE_STANDARD_INFORMATION standardInfo;
		status = ZwQueryInformationFile(fileHandle,
										&ioStatus,
										&standardInfo,
										sizeof(FILE_STANDARD_INFORMATION),
										FileStandardInformation);
		LONG size;
		if (!NT_SUCCESS(status) || standardInfo.EndOfFile.HighPart != 0) {
			returnValue = false;
			goto cleanup;
		}
		
		size = standardInfo.EndOfFile.LowPart;

		const UINT32 CHUNK_SIZE = 0x1000;

		buffer = ExAllocatePool(PagedPool, CHUNK_SIZE);

		for (LONGLONG chunk = 0; chunk < (size / CHUNK_SIZE); ++chunk) {
			status = ZwReadFile(fileHandle,
								NULL,
								NULL,
								NULL,
								&ioStatus,
								buffer,
								0x1000,
								NULL,
								NULL);

			if (!NT_SUCCESS(status) || ioStatus.Information != CHUNK_SIZE) {
				returnValue = false;
				goto cleanup;
			}

			if (searchInBuffer(buffer, CHUNK_SIZE)) {
				returnValue = true;
				goto cleanup;
			}
		}

		UINT32 chunkSize = size % CHUNK_SIZE;
		status = ZwReadFile(fileHandle,
							NULL,
							NULL,
							NULL,
							&ioStatus,
							buffer,
							chunkSize,
							NULL,
							NULL);

		if (!NT_SUCCESS(status) || ioStatus.Information != chunkSize) {
			returnValue = false;
			goto cleanup;
		}

		if (searchInBuffer(buffer, chunkSize)) {
			returnValue = true;
			goto cleanup;
		}

cleanup:
		if (returnValue) {
			DbgBreakPoint();
		}

		if (buffer != NULL) {
			ExFreePool(buffer);
		}

		if (fileHandle != NULL) {
			ZwClose(fileHandle);
		}

		return returnValue;
	}

	bool searchInBuffer(const void* buffer, UINT32 bufferSize) {
		const char virus[] = "virus";
		for (ULONG i = 0; i < (bufferSize - _ARRAYSIZE(virus)); ++i) {
			if (RtlCompareMemory((char*)buffer + i, virus, _ARRAYSIZE(virus)) == 5) {
				return true;
			}
		}
		return false;
	}

}



