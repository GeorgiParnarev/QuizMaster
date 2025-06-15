#pragma once

#include "CommandStruct.h"
#include "IWriter.h"
#include "IReader.h"
#include "IFileBaseProvider.h"
#include "UserStruct.h"
#include "GlobalConstants.h"

class IUser 
{
public:
    virtual String getName() const = 0;
    virtual void setFirstName(const String) = 0;
    virtual void setLastName(const String) = 0;
    virtual String getUsername() const = 0;
    virtual void setUsername(const String) = 0;
    virtual void setId(const unsigned int) = 0;
    virtual unsigned int getId() const = 0;
    virtual String getFileName() const = 0;
    virtual void setFileName(const String) = 0;
    virtual void Login() = 0;
    virtual void Logout() = 0;
    virtual void Action(CommandStruct&) = 0;
    virtual IWriter& Writer() = 0;
    virtual IReader& Reader() = 0;
    virtual IFileBaseProvider& Provider() = 0;
    virtual void Print() = 0;
    virtual bool getIsHasLogin() const = 0;
    virtual void setIsHasLogin(bool) = 0;
    virtual String BuildUserData() = 0;
    virtual void Help() = 0;
    virtual unsigned int Hash(const String&) = 0;
    virtual int FindUserData(UserStruct&, bool) = 0;
    virtual void AllUsers(String&) = 0;
    virtual void SetUpUserData(UserStruct&, Vector<String>&, UserOptions) = 0;
    virtual void SaveData() = 0;
};
