#include "ProcessNotification.h"
#include "Utils.h"
#include "UnicodeString.h"

ProcessArray g_processArray;

namespace ProcessNotification
{

	void registerProcessNotify()
	{
		RtlZeroMemory(&g_processArray, sizeof(ProcessArray));
		NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(processNotification, FALSE);
		if (!NT_SUCCESS(status))
		{
			DbgBreakPoint();
		}
	}

	void unregisterProcessNotify()
	{
		PsSetCreateProcessNotifyRoutineEx(processNotification, TRUE);
	}

	void processNotification(
		PEPROCESS Process,
		HANDLE ProcessId,
		PPS_CREATE_NOTIFY_INFO CreateInfo)
	{
		UNREFERENCED_PARAMETER(ProcessId);
		
		UnicodeString processPath(MAX_PATH);
		if (processPath.init()) {
			Utils::getProcessPath(Process, processPath);

		}
		//if (checkVirusFile(ProcessId))
		//{
		//	killProcess(Process);
		//}
		//
		if (CreateInfo)
		{
			g_processArray.processes[g_processArray.processIndex] = Process;
			g_processArray.processIndex++;
		}
	}
	/*
	bool checkIsVirus(PEPROCESS Process)
	{
		Process->
	}*/
}



