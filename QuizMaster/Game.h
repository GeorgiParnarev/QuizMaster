#pragma once

#include "IUser.h"
#include "Player.h"
#include "CommandStruct.h"
#include "IGame.h"

class Game : public IGame
{
private:
    IUser* user;
    IReader* reader;
    IWriter* writer;
    IFileBaseProvider* provider;
    CommandStruct* command;

    unsigned int maxUserId;
    unsigned int maxQuizId;

    void Free();
    void GameLoop();
    void SetCommandStruct();
    void LoadConfig();

public:
    Game(IWriter*, IReader*, IFileBaseProvider*);
    ~Game();

    virtual void Init() override;
    virtual void Run() override;
    virtual void Exit() override;

    void LoginUser();
    void LogoutUser();
    void SignupUser();

    void SaveUser();
    void LoadUser(UserStruct& us);

    virtual unsigned int getMaxQuizId() const override;
    virtual void setMaxQuizId(unsigned int) override;
    virtual void SaveConfig() override;
};