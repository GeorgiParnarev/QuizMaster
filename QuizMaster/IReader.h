#pragma once

#include "String.h"

class IReader
{
public:
	virtual String* ReadLine() = 0;
};