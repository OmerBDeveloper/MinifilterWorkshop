#include "Utils.h"

typedef NTSTATUS(*QueryProcessInformation_t)(
	__in HANDLE,
	__in PROCESSINFOCLASS,
	__out PVOID,
	__in ULONG,
	__out_opt PULONG);

UNICODE_STRING ZwQueryInformationProcessName = RTL_CONSTANT_STRING(L"ZwQueryInformationProcess");
bool Utils::getProcessPath(PEPROCESS process, UnicodeString& processPath)
{

	PUNICODE_STRING imageFileNameBuffer = NULL;
	NTSTATUS status;
	ULONG returnedSize = 0;
	bool returnValue = true;
	HANDLE processHandle = NULL;


	status = ObOpenObjectByPointer(process, OBJ_KERNEL_HANDLE, NULL, PROCESS_ALL_ACCESS, NULL, KernelMode, &processHandle);
	if (!NT_SUCCESS(status)) {
		returnValue = false;
		goto cleanup;
	}

	QueryProcessInformation_t queryInformationProcess = (QueryProcessInformation_t)MmGetSystemRoutineAddress(&ZwQueryInformationProcessName);
	if (queryInformationProcess == NULL) {
		returnValue = false;
		goto cleanup;
	}

	status = queryInformationProcess(processHandle, ProcessImageFileName, NULL, 0, &returnedSize);
	if (status != STATUS_INFO_LENGTH_MISMATCH) {
		returnValue = false;
		goto cleanup;
	}
	
	imageFileNameBuffer = (PUNICODE_STRING)ExAllocatePool(NonPagedPool, returnedSize + 2);
	if (imageFileNameBuffer == NULL) {
		returnValue = false;
		goto cleanup;
	}

	status = queryInformationProcess(processHandle, ProcessImageFileName, imageFileNameBuffer, returnedSize, &returnedSize);
	if (!NT_SUCCESS(status)) {
		returnValue = false;
		goto cleanup;
	}

	if (!processPath.copyFrom(imageFileNameBuffer)) {
		returnValue = false;
		goto cleanup;
	}

cleanup:

	if (imageFileNameBuffer != NULL) {
		ExFreePool(imageFileNameBuffer);
	}

	if (processHandle != NULL) {
		ZwClose(processHandle);
	}

	return returnValue;
}


