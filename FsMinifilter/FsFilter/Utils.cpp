#include "Utils.h"

typedef NTSTATUS(*QueryProcessInformation_t)(
	__in HANDLE,
	__in PROCESSINFOCLASS,
	__out PVOID,
	__in ULONG,
	__out_opt PULONG);

UNICODE_STRING ZwQueryInformationProcessName = RTL_CONSTANT_STRING(L"ZwQueryInformationProcess");
bool Utils::getProcessPath(PEPROCESS process, PUNICODE_STRING processPath)
{

	PUNICODE_STRING ImageFileNameBuffer = NULL;
	NTSTATUS Status;
	ULONG returnedSize = 0;
	bool returnValue = true;
	HANDLE processHandle = NULL;


	Status = ObOpenObjectByPointer(process, OBJ_KERNEL_HANDLE, NULL, PROCESS_ALL_ACCESS, NULL, KernelMode, &processHandle);
	if (!NT_SUCCESS(Status)) {
		returnValue = false;
		goto cleanup;
	}

	QueryProcessInformation_t queryInformationProcess = (QueryProcessInformation_t)MmGetSystemRoutineAddress(&ZwQueryInformationProcessName);
	if (queryInformationProcess == NULL) {
		returnValue = false;
		goto cleanup;
	}

	Status = queryInformationProcess(processHandle, ProcessImageFileName, NULL, 0, &returnedSize);
	if (Status != STATUS_INFO_LENGTH_MISMATCH) {
		returnValue = false;
		goto cleanup;
	}
	
	ImageFileNameBuffer = (PUNICODE_STRING)ExAllocatePool(NonPagedPool, returnedSize + 2);
	if (ImageFileNameBuffer == NULL) {
		returnValue = false;
		goto cleanup;
	}

	Status = queryInformationProcess(processHandle, ProcessImageFileName, ImageFileNameBuffer, returnedSize, &returnedSize);
	if (!NT_SUCCESS(Status)) {
		returnValue = false;
		goto cleanup;
	}

	Status = RtlAppendUnicodeStringToString(processPath, ImageFileNameBuffer);
	if (!NT_SUCCESS(Status)) {
		returnValue = false;
		goto cleanup;
	}


cleanup:

	if (ImageFileNameBuffer != NULL) {
		ExFreePool(ImageFileNameBuffer);
	}

	if (processHandle != NULL) {
		ZwClose(processHandle);
	}

	return returnValue;
}


