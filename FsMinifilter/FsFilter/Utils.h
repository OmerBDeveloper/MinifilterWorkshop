#pragma once
#include <ntifs.h>

class Utils
{
public:
	static void getProcessPath(PEPROCESS process, PUNICODE_STRING processPath);
};

