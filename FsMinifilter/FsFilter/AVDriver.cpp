#include "AVHeader.h"

UNICODE_STRING THREAT_NAME = RTL_CONSTANT_STRING(L"*VIRUS*");

bool IsThreatByFilename(PUNICODE_STRING fileName) {

	/************************************************************************/
	/* STEP 2:																*
	/*			This function should return true if the name of the file 	*
	/*			is virus.exe.												*
	/*																		*
	/************************************************************************/

	// ENTER CODE HERE
	//UNREFERENCED_PARAMETER(fileName);
	BOOLEAN isVirus = FsRtlIsNameInExpression(
		&THREAT_NAME,
		fileName,
		TRUE,
		NULL);

	return isVirus;
}


FLT_PREOP_CALLBACK_STATUS AntiVirusPreReadOperation(_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID *CompletionContext) {

	UNREFERENCED_PARAMETER(CompletionContext);

	PUNICODE_STRING fileName = &FltObjects->FileObject->FileName;
	if (IsThreatByFilename(fileName)) {

		/************************************************************************/
		/* STEP 3:																*
		/*		If we found a threat, we need to change the iostatus of the		*
		/*		request and return a value which indicates that we are done		*		
		/*		and the request is complete.									*
		/************************************************************************/
		// ENTER CODE HERE
		Data->IoStatus.Status = STATUS_VIRUS_INFECTED;
		Data->IoStatus.Information = 0;
		return FLT_PREOP_COMPLETE;

	} else {
		Data->IoStatus.Status = STATUS_SUCCESS;
		Data->IoStatus.Information = 0;
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}
}

