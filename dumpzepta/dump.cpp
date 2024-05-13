#include "dump.hpp"

NTSTATUS
DumpStart(
    _In_ PFILTER_EXTENSION FilterExtension
) 
{
    WriteLog("DumpStart!\n");

    return DUMP_SUCCESS;
}

NTSTATUS
DumpWrite(
    _In_ PFILTER_EXTENSION FilterExtension,
    _Inout_ PLARGE_INTEGER DiskByteOffset,
    _Inout_ PMDL Mdl
) 
{
    WriteLog("DumpWrite!\n");

    return DUMP_SUCCESS;
}

NTSTATUS
DumpFinish(
    _In_ PFILTER_EXTENSION FilterExtension
)
{
    WriteLog("DumpFinish!\n");

    return DUMP_SUCCESS;

}

NTSTATUS
DumpUnload(
    _In_ PFILTER_EXTENSION FilterExtension
)
{
    WriteLog("DumpUnload!\n");

    return DUMP_SUCCESS;

}

NTSTATUS
DumpRead(
    _In_ PFILTER_EXTENSION FilterExtension,
    _In_ PLARGE_INTEGER DiskByteOffset,
    _In_ PMDL Mdl
)
{
    WriteLog("DumpRead!\n");

    return DUMP_SUCCESS;

}