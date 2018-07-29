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
		PEPROCESS process,
		HANDLE processId,
		PPS_CREATE_NOTIFY_INFO createInfo)
	{
		UNREFERENCED_PARAMETER(processId);
		
		UnicodeString processPath(MAX_PATH);
		if (processPath.init()) {
			Utils::getProcessPath(process, processPath);

		}
		//if (checkVirusFile(ProcessId))
		//{
		//	killProcess(Process);
		//}
		//
		if (createInfo)
		{
			g_processArray.processes[g_processArray.processIndex] = process;
			g_processArray.processIndex++;
		}
	}
	/*
	bool checkIsVirus(PEPROCESS Process)
	{
		Process->
	}*/
}



