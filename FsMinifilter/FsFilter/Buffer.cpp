#include "Buffer.h"

Buffer::Buffer() : m_size(0), m_buffer(NULL) {
}


Buffer::~Buffer() {
	if (m_buffer != NULL) {
		ExFreePool(m_buffer);
	}
}

bool Buffer::init(UINT32 size) {
	m_size = size;
	m_buffer = ExAllocatePool(PagedPool, size);
	if (m_buffer == NULL) {
		return false;
	}
	return true;
}

void* Buffer::get() {
	return m_buffer;
}

const void* Buffer::get() const{
	return m_buffer;
}


UINT32 Buffer::size() const {
	return m_size;
}

