#pragma once

#include "IWriter.h"
#include "IReader.h"
#include "IFileBaseProvider.h"
#include "CommandStruct.h"

class IUser 
{
public:
	virtual String getName() const = 0;
	virtual void setFirstName(const String) = 0;
	virtual void setLastName(const String) = 0;

	virtual String getUsername() const = 0;
	virtual void setUsername(const String) = 0;

	virtual unsigned int getId() const = 0;
	virtual void setId(const unsigned int) = 0;

	virtual bool getIsHasLogin() const = 0;
	virtual void setIsHasLogin(bool) = 0;

	virtual void Action(const CommandStruct&) = 0;

	virtual void Login() = 0;
	virtual void Logout() = 0;

	virtual IWriter& Writer() = 0;
	virtual IReader& Reader() = 0;
	virtual IFileBaseProvider& Provider() = 0;

	virtual void Help() = 0;
	virtual void SaveData() = 0;

	virtual unsigned int Hash(const String&) = 0;
};
