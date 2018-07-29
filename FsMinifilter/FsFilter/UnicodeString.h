#pragma once
#include <basetsd.h>
#include <ntdef.h>
#include <Ntifs.h>


class UnicodeString {
public:
	UnicodeString(USHORT MaxSize);
	~UnicodeString();

	bool init();
	bool copyFrom(PUNICODE_STRING other);

private:
	USHORT m_maxSize;
	UNICODE_STRING m_string;
};

