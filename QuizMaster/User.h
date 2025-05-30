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
	String fileName;

	unsigned int password;

	bool isHasLogin;

	IWriter* writer;
	IReader* reader;
	IFileBaseProvider* provider;

protected:
	unsigned int GetPassword() const;
	void SetPassword(unsigned int);

	virtual IWriter& Writer() override;
	virtual IReader& Reader() override;
	virtual IFileBaseProvider& Provider() override;

	int FindUserIndex(UserStruct& us, Vector<String>& usersVec);

public:
	User(IWriter*, IReader*, IFileBaseProvider*);

	virtual String getName() const override;
	virtual void setFirstName(const String) override;
	virtual void setLastName(const String) override;

	virtual String getUsername() const override;
	virtual void setUsername(const String) override;

	virtual unsigned int getId() const override;
	virtual void setId(const unsigned int) override;

	virtual bool getIsHasLogin() const override;
	virtual void setIsHasLogin(const bool) override;

	virtual void Login() override;
	virtual void Logout() override;
	virtual void Action(const CommandStruct&) override;

	virtual void Help() override;
	virtual unsigned int Hash(const String& str) override;
	virtual int FindUserData(UserStruct&, bool) override;
	virtual void SaveData() override;
	virtual void AllUsers(String&) override;
	virtual String BuildUserData() override;
	virtual void SetUpUserData(UserStruct&, Vector<String>&, UserOptions) override;

	virtual ~User() {}
};