#pragma once

#include "User.h";
#include "IGame.h"
#include "Vector.h"

class Player : public User
{
private:
    unsigned int level;
    unsigned int points;
    unsigned int numberCreatedQuizzes;
    unsigned int numberLikedQuizzes;
    unsigned int numberFavoriteQuizzes;
    unsigned int numberFinishedChallenges;
    unsigned int numberSolvedTestQuizzes;
    unsigned int numberSolvedNormalQuizzes;
    unsigned int numberCreatedQuizzesChallengers;

    Vector<String> listCreatedQuizzes;
    Vector<unsigned int> listLikedQuizzes;
    Vector<unsigned int> listFavoriteQuizzes;
    Vector<String> listFinishedChallenges;

    IGame* game;

    void Init();

public:
    Player(IWriter*, IReader*, IFileBaseProvider*, UserStruct*, UserOptions);
    Player(IWriter*, IReader*, IFileBaseProvider*, IGame*);
    virtual ~Player() {};
    virtual void Help() override;
    virtual void SaveData() override;
    virtual String BuildUserData() override;
    virtual void SetUpUserData(UserStruct&, Vector<String>&, UserOptions) override;
};
