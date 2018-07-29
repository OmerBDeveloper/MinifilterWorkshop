#pragma once
#include <ntifs.h>

class Utils
{
public:
	static bool getProcessPath(PEPROCESS process, PUNICODE_STRING processPath);
};

