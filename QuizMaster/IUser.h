#pragma once

#include "IWriter.h"
#include "IReader.h"

class IUser 
{
public:
	virtual IWriter& Writer() = 0;
	virtual IReader& Reader() = 0;
	virtual void Login() = 0;
	virtual void Logout() = 0;
	virtual void Help() = 0;
};
