#pragma once

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <ntdef.h>

extern PFLT_FILTER gFilterHandle;

/*************************************************************************
Prototypes
*************************************************************************/

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);

NTSTATUS FsFilterUnload(_In_ FLT_FILTER_UNLOAD_FLAGS Flags);

EXTERN_C_END
