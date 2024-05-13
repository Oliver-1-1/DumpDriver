#pragma once
#include <ntifs.h>
//#include <ntddk.h>
#include <windef.h>
#include <ntimage.h>
#include <ntdddisk.h>

#include <ntdddump.h>
#include "file_log.hpp"
#define DUMP_SUCCESS 0
#define DUMP_UNSUCCESSFUL 1

NTSTATUS
DumpStart(
    _In_ PFILTER_EXTENSION FilterExtension
);

NTSTATUS
DumpWrite(
    _In_ PFILTER_EXTENSION FilterExtension,
    _Inout_ PLARGE_INTEGER DiskByteOffset,
    _Inout_ PMDL Mdl
);

NTSTATUS
DumpFinish(
    _In_ PFILTER_EXTENSION FilterExtension
);


NTSTATUS
DumpUnload(
    _In_ PFILTER_EXTENSION FilterExtension
);

NTSTATUS
DumpRead(
    _In_ PFILTER_EXTENSION FilterExtension,
    _In_ PLARGE_INTEGER DiskByteOffset,
    _In_ PMDL Mdl
);
 
//_declspec(dllexport)
//PVOID
//DumpPreInitialize(
//);
//
//_declspec(dllexport)
//PVOID
//DumpUninitialize(
//);