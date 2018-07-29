#include "UnicodeString.h"

UnicodeString::UnicodeString(USHORT MaxSize) : m_maxSize(MaxSize) {
	m_string.Buffer = NULL;
	m_string.Length = 0;
	m_string.MaximumLength = 0;
}


UnicodeString::~UnicodeString() {
	if (m_string.Buffer != NULL) {
		ExFreePool(m_string.Buffer);
	}
}

bool UnicodeString::init() {
	m_string.Buffer = (PWCH)ExAllocatePool(PagedPool, m_maxSize);
	if (m_string.Buffer == NULL) {
		return false;
	}
	m_string.Length = 0;
	m_string.MaximumLength = m_maxSize;
	return true;
}

bool UnicodeString::copyFrom(PCUNICODE_STRING other) {
	NTSTATUS status = RtlAppendUnicodeStringToString(&m_string, other);
	return NT_SUCCESS(status);
}

PUNICODE_STRING UnicodeString::getUnicodeString() {
	return &m_string;
}
