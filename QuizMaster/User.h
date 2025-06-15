#pragma once

#include "String.h"
#include "IUser.h"
#include "Quiz.h"
#include "Message.h"

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
    Quiz* quiz;
    Message* message;

protected:
    void setIsHasLogin(bool);

    unsigned int getPassword() const;
    void setPassword(unsigned int);
    virtual IWriter& Writer() override;
    virtual IReader& Reader() override;
    virtual IFileBaseProvider& Provider() override;
    int FindUserIndex(UserStruct& us, Vector<String>& usersVec);
    bool GenerateReason(CommandStruct&);
    Quiz& GetQuiz();
    Message& GetMessage();

public:
    User(IWriter*, IReader*, IFileBaseProvider*);
    virtual String getName() const override;
    virtual void setFirstName(const String) override;
    virtual void setLastName(const String) override;

    virtual String getUsername() const override;
    virtual void setUsername(const String) override;

    virtual unsigned int getId() const override;
    virtual void setId(const unsigned int) override;

    virtual String getFileName() const override;
    virtual void setFileName(const String) override;

    virtual void Login() override;
    virtual void Logout() override;
    virtual void Action(CommandStruct&) override;

    virtual bool getIsHasLogin() const override;
    virtual unsigned int Hash(const String& str) override;
    virtual int FindUserData(UserStruct&, bool) override;
    virtual void AllUsers(String&) override;
    virtual void SetUpUserData(UserStruct&, Vector<String>&, UserOptions) override;

    virtual void Print() override;
    virtual String BuildUserData() override;
    virtual void Help() override;
    virtual void SaveData() override;

    virtual ~User();
};