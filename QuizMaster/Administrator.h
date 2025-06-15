#pragma once

#include "User.h"

class Administrator : public User
{
public:
    Administrator(IWriter*, IReader*, IFileBaseProvider*);
    virtual ~Administrator() {};

    void Ban(const CommandStruct&);
    void Pending();
    void ApproveQuiz(const CommandStruct&);
    void RejectOrRemoveQuiz(String, String, QuizStatus);
    void ViewReports();

    virtual void Help() override;
    virtual void Action(CommandStruct&) override;
    virtual String BuildUserData() override;
    virtual void SaveData() override;
};