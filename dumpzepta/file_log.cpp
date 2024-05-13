#include "file_log.hpp"

NTSTATUS WriteLog(
    __in PCHAR string,
    __in ...
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PCHAR buffer;
    if (string == NULL) 
    {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Format string UNIMPLEMENTED
    //

    buffer = string;

    //
    // Log to dbgview
    //
#ifdef DEBUG_LOG
    //
    // We don't the return because it will return a waiting status?
    //
    WriteDebug(string);
#endif

    //
    // Log to file
    //
#ifdef DEBUG_FILE
    HANDLE handle = 0;
    status = OpenFile(FILE_PATH, &handle);

    if (status != STATUS_SUCCESS)
    {
        return STATUS_UNSUCCESSFUL;
    }

    status = WriteFile(handle, string);

    if (status != STATUS_SUCCESS)
    {
        CloseFile(handle);
        return STATUS_UNSUCCESSFUL;
    }

   status = CloseFile(handle);

#endif 

   return status;

}

NTSTATUS WriteDebug(
    __in PCHAR buffer
) 
{
    NTSTATUS status;

    status = DbgPrintEx(
        DPFLTR_SYSTEM_ID,
        DPFLTR_ERROR_LEVEL,
        buffer);

    return status;
}

NTSTATUS 
OpenFile(
    __in PWSTR name,
    __out PHANDLE handle
)
{
    UNICODE_STRING unicode;
    OBJECT_ATTRIBUTES attributes;
    IO_STATUS_BLOCK io;
    NTSTATUS status;

    if (name == NULL || handle == NULL) 
    {
        return STATUS_INVALID_PARAMETER;
    }

    RtlInitUnicodeString(&unicode, name);

    InitializeObjectAttributes(
        &attributes,
        &unicode,
        OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        NULL,
        NULL);

    status = ZwCreateFile(
        handle,
        GENERIC_WRITE,
        &attributes,
        &io,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_OPEN_IF,
        FILE_RANDOM_ACCESS,
        NULL,
        0);

    return status;
}

NTSTATUS
WriteFile(
    __in HANDLE handle,
    __in PCHAR buffer
)
{
    IO_STATUS_BLOCK io;
    LARGE_INTEGER offset;
    NTSTATUS status;

    if (handle == NULL || buffer == NULL) 
    {
        return STATUS_INVALID_PARAMETER;
    }

    offset.HighPart = -1;
    offset.LowPart = FILE_WRITE_TO_END_OF_FILE;

    status = ZwWriteFile(
        handle,
        NULL,
        NULL,
        NULL,
        &io,
        buffer,
        (ULONG)strlen(buffer),
        &offset,
        NULL);

    return status;

}

NTSTATUS
CloseFile(
    __in HANDLE handle
)
{
    NTSTATUS status;

    if (handle == NULL) {
        return STATUS_INVALID_PARAMETER;
    }

    status = ZwClose(handle);

    return status;
}
