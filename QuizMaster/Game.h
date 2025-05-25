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
    void SaveConfig();

public:
    Game(IWriter*, IReader*, IFileBaseProvider*);
    ~Game();

    void Init();
    void Run();
    void Exit();

    void LoginUser();
    void LogoutUser();
    void SignupUser();

   /* virtual unsigned int GetMaxQuizId() const override;
    virtual void SetMaxQuizId(unsigned int) override;*/
};