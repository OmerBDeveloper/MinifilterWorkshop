#include "Utils.h"

typedef NTSTATUS(*queryProcessInformation)(
	__in HANDLE,
	__in PROCESSINFOCLASS,
	__out PVOID,
	__in ULONG,
	__out_opt PULONG);

UNICODE_STRING ZwQueryInformationProcess =  RTL_CONSTANT_STRING(L"ZwQueryInformationProcess");
void Utils::getProcessPath(PEPROCESS process, PUNICODE_STRING processPath)
{
	PUNICODE_STRING ImageFileName;
	UNICODE_STRING NullString = { 0 };

	NTSTATUS Status;

	ULONG returnedSize = 0;

	queryProcessInformation queryProcess;

	queryProcess = (queryProcessInformation)MmGetSystemRoutineAddress(&ZwQueryInformationProcess);

	HANDLE processHandle;

	ObOpenObjectByPointer(process, OBJ_KERNEL_HANDLE, NULL, PROCESS_ALL_ACCESS, NULL, KernelMode, &processHandle);

	Status = queryProcess(processHandle, ProcessImageFileName, NULL, 0, &returnedSize);
	if (Status != STATUS_INFO_LENGTH_MISMATCH) {
		ImageFileName = &NullString;
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Fail in NtQueryInformationProcess %s", Status);
	}
	
	ImageFileName = (PUNICODE_STRING)ExAllocatePool(NonPagedPool, returnedSize + 2);

	Status = queryProcess(processHandle, ProcessImageFileName, ImageFileName, returnedSize, &returnedSize);

	if (!NT_SUCCESS(Status)) {
		ImageFileName = &NullString;
	}

	RtlAppendUnicodeStringToString(processPath, ImageFileName);
	if (ImageFileName != NULL)
	{
		ExFreePool(ImageFileName);
	}

	if (processHandle != NULL)
	{
		ZwClose(processHandle);
	}
}


