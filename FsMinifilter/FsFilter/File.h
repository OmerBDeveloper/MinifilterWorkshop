#pragma once

#include <ntifs.h>
#include "UnicodeString.h"
#include "Buffer.h"

class File {
public:
	File();
	~File();

	bool init(UnicodeString& path);
	bool File::getFileSize(UINT32& size);
	bool read(Buffer& buffer);

private:
	HANDLE m_fileHandle;
};

