#pragma once

#include "String.h"

class IWriter
{
public:
	virtual void Write(const String& str) = 0;
	virtual void WriteLine(const String& str) = 0;
};