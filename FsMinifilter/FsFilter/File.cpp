#include "File.h"
#include <ntdef.h>

File::File() :m_fileHandle(NULL) {
	// empty
}

File::~File() {
	if (m_fileHandle != NULL) {
		ZwClose(m_fileHandle);
	}
}

bool File::init(UnicodeString& path) {
	
	OBJECT_ATTRIBUTES fileAttributes;
	NTSTATUS status;

	InitializeObjectAttributes(&fileAttributes,
							   path.getUnicodeString(),
							   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
							   NULL, // RootDirectory
							   NULL // SecurityDescriptor
							   );
	IO_STATUS_BLOCK ioStatus;

	status = ZwCreateFile(&m_fileHandle,
						  FILE_READ_DATA,
						  &fileAttributes,
						  &ioStatus,
						  NULL,
						  FILE_ATTRIBUTE_NORMAL,
						  FILE_SHARE_READ,
						  FILE_OPEN,
						  FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
						  NULL,
						  0);
	if (!NT_SUCCESS(status)) {
		return false;
	}
	return true;
}

bool File::getFileSize(UINT32& size) {
	FILE_STANDARD_INFORMATION standardInfo;
	IO_STATUS_BLOCK ioStatus;

	NTSTATUS status = ZwQueryInformationFile(m_fileHandle,
											 &ioStatus,
											 &standardInfo,
											 sizeof(FILE_STANDARD_INFORMATION),
											 FileStandardInformation);
	if (!NT_SUCCESS(status) || standardInfo.EndOfFile.HighPart != 0) {
		return false;
	}

	size = standardInfo.EndOfFile.LowPart;
	return true;
}

bool File::read(Buffer& buffer) {
	IO_STATUS_BLOCK ioStatus;
	NTSTATUS status = ZwReadFile(m_fileHandle,
								 NULL,
								 NULL,
								 NULL,
								 &ioStatus,
								 buffer.get(),
								 buffer.size(),
								 NULL,
								 NULL);

	if (!NT_SUCCESS(status) || ioStatus.Information != buffer.size()) {
		return false;
	}
	return true;

}
