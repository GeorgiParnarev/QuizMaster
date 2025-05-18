#pragma once

#include "String.h"
#include "IUser.h"

class User : public IUser
{
private:
	unsigned int id;

	String firstName;
	String lastName;
	String username;

	IWriter* writer;
	IReader* reader;

protected:
	virtual IWriter& Writer() override;
	virtual IReader& Reader() override;

public:
	virtual void Help() override;
};