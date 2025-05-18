#pragma once

#include "User.h";
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

public:
    virtual void Help() override;
};
