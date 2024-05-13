#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>

#define FILE_PATH L"\\DosDevices\\c:\\file_log.txt"
#define DEBUG_LOG
#define DEBUG_FILE

NTSTATUS WriteLog(
	__in PCHAR buffer,
	__in ...
);

NTSTATUS WriteDebug(
	__in PCHAR buffer
);

NTSTATUS
OpenFile(
	__in PWSTR name,
	__out PHANDLE handle
);

NTSTATUS
WriteFile(
	__in HANDLE handle,
	__in PCHAR buffer
);

NTSTATUS CloseFile(
	__in HANDLE handle
);