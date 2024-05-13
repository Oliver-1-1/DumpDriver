#include "dump.hpp"
#include "file_log.hpp"
#include "nt.hpp"

PDUMP_START start = DumpStart;
PDUMP_WRITE write = DumpWrite;
PDUMP_FINISH finish = DumpFinish;
PDUMP_UNLOAD unload = DumpUnload;
PDUMP_READ read = DumpRead;

PCHAR blacklist[] = {
	"\x76\x67\x6b\x2e\x73\x79\x73\x00",
	"\x66\x61\x63\x65\x69\x74\x5f\x61\x63\x2e\x73\x79\x73\x00",
};

VOID
to_lower_str(
	__in PCHAR Str,
	__in PCHAR Out
);

BOOL
IsDriverActive(
	__in PCSTR Name
);

BOOL
IsProhibitedDriverLoaded(
);

PCHAR
GetFilterType(
	__in FILTER_DUMP_TYPE Type
);

VOID
PrintCurrentDriver(
	__in PFILTER_EXTENSION Filter
);

NTSTATUS
DriverEntry(
	__in PFILTER_EXTENSION Filter,
	__in PFILTER_INITIALIZATION_DATA Data
)
{
	PCWSTR registry = L"dumpfve.sys";
	NTSTATUS status;

	if (Filter == NULL || Data == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	if (IsProhibitedDriverLoaded())
	{
		return STATUS_UNSUCCESSFUL;
	}

	//
	// Print relevant information
	//
	WriteLog("Entry!\n");

	WriteLog(
		GetFilterType(Filter->DumpType)
	);

	PrintCurrentDriver(Filter);

	//
	// Setup the drivers callback. This is probably not needed. But i dont want the system trying to access a NULL pointer.
	// If you look at dumpfve.sys they use DriverEntry arguments very weirdly. This is because its actually another struct that is used: PFILTER_INITIALIZATION_DATA. ( look at buttom for example)
	//

	Data->MajorVersion = DUMP_FILTER_MAJOR_VERSION;
	Data->Flags |= 0b1;

	Data->DumpFinish = start;
	Data->DumpWrite = write;
	Data->DumpFinish = finish;
	Data->DumpUnload = unload;
	Data->DumpRead = read;
	
	//
	// Do you shady stuff here:
	//



	//
	// Clear traces, this will also make sure out driver does only load once. There are more traces like with normal drivers. 
	//
	status = RtlWriteRegistryValue(
		RTL_REGISTRY_CONTROL,
		L"CrashControl",
		L"DumpFilters",
		REG_MULTI_SZ,
		(PVOID)registry,
		(ULONG)((ULONG)wcslen(registry) * 2 + 2));

	if (status != STATUS_SUCCESS)
	{
		WriteLog("Failed to clear dumper registry trace!\n");
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

//
// Make all characters in a string non-capital(lower-case).
//
VOID
to_lower_str(
	__in PCHAR Str,
	__in PCHAR Out
)
{
	int i = -1;
	while (Str[++i] != '\0')
	{
		Out[i] = (CHAR)tolower(Str[i]);
	}
}

//
// Check if a driver is loaded at the current time.
//
BOOL
IsDriverActive(
	__in PCSTR Name
)
{
	ULONG length = 0;
	PRTL_PROCESS_MODULES list;
	NTSTATUS status;

	ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, 0, 0, &length);
	length += (10 * 1024);

	list = (PRTL_PROCESS_MODULES)ExAllocatePool((POOL_TYPE)(POOL_COLD_ALLOCATION | PagedPool), length);

	if (list == NULL)
	{
		return FALSE;
	}

	status = ZwQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, list, length, &length);

	if (status != STATUS_SUCCESS) {
		ExFreePool(list);
		return FALSE;
	}

	for (ULONG i = 0; i < list->NumberOfModules; i++)
	{
		PRTL_PROCESS_MODULE_INFORMATION item = &list->Modules[i];

		PCSTR target[256];
		memset(target, 0, 256);

		to_lower_str((PCHAR)item->FullPathName, (PCHAR)target);

		if (!strstr((PCHAR)target, Name))
		{
			continue;
		}

		ExFreePool(list);

		return TRUE;

	}

	ExFreePool(list);


	return FALSE;
}

//
// Check if a prohibited driver is loaded. (Prohibited driver can be found in the blocklist)
//
BOOL
IsProhibitedDriverLoaded(
) 
{
	BOOL loaded = FALSE;

	for (int i = 0; i < ARRAYSIZE(blacklist); i++) {
		loaded |= IsDriverActive(blacklist[i]);
	}

	return loaded;
	
}

//
// Get what filter type in string value.
//
PCHAR
GetFilterType(
	__in FILTER_DUMP_TYPE Type
)
{
	PCHAR string;

	switch (Type)
	{
	case DumpTypeUndefined:
		string = "DumpTypeUndefined\n";
		break;
	case DumpTypeCrashdump:
		string = "DumpTypeCrashdump\n";
		break;
	case DumpTypeHibernation:
		string = "DumpTypeHibernation\n";
		break;
	default:
		string = "Empty\n";
		break;
	}

	return string;
}

//
// Print the drivers name that hold the dump volume object.
//
VOID
PrintCurrentDriver(
	__in PFILTER_EXTENSION Filter
)
{
	UCHAR target[256];
	for (int i = 0; i < Filter->DeviceObject->DriverObject->DriverName.Length; i++) {
		target[i] = (UCHAR)Filter->DeviceObject->DriverObject->DriverName.Buffer[i];
	}
	target[Filter->DeviceObject->DriverObject->DriverName.Length] = 0;
	WriteLog((PCHAR)target);
}

//
//dumpfve.sys
// It adds up with PFILTER_INITIALIZATION_DATA if you create a custom struct in IDA
//

/*
RegistryPath->Buffer = (PWSTR)DumpStart;
	*(ULONGLONG*)&RegistryPath[1].Length = (ULONGLONG)DumpWrite;
	RegistryPath[1].Buffer = (PWSTR)DumpFinish;
	*(ULONGLONG*)&RegistryPath[2].Length = (ULONGLONG)DumpUnload;
	int v18 = *(DWORD*)(&RegistryPath[3].MaximumLength + 1);
	if ((v18 & 1) != 0)
	{
		*(DWORD*)&RegistryPath->Length = 2;
		RegistryPath[3].Buffer = (PWSTR)DumpRead;
	}
*/