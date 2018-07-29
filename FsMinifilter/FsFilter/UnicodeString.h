#pragma once
#include <basetsd.h>
#include <ntdef.h>
#include <Ntifs.h>


class UnicodeString {
public:
	UnicodeString(USHORT MaxSize);
	~UnicodeString();

	bool init();
	bool copyFrom(PCUNICODE_STRING other);
	PUNICODE_STRING getUnicodeString();

private:
	USHORT m_maxSize;
	UNICODE_STRING m_string;
};

