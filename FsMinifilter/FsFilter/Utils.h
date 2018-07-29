#pragma once
#include <ntifs.h>
#include "UnicodeString.h"

class Utils
{
public:
	static bool getProcessPath(PEPROCESS process, UnicodeString& processPath);
};

