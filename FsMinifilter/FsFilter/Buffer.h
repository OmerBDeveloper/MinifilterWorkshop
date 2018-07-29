#pragma once
#include <ntifs.h>

class Buffer {
public:
	Buffer();
	~Buffer();

	bool init(UINT32 size);
	void* get();
	const void* get() const;
	UINT32 size() const;

private:
	UINT32 m_size;
	void* m_buffer;
};

