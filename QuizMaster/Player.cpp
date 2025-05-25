#include "Player.h"

void Player::Help()
{
    User::Help();

    this->Writer().WriteLine("view-profile");
    this->Writer().WriteLine("edit-profile");
    this->Writer().WriteLine("view-challeenges");
    this->Writer().WriteLine("view-finished-challeenges");
    this->Writer().WriteLine("view <nickname>");
    this->Writer().WriteLine("messages");
    this->Writer().WriteLine("create-quiz");
    this->Writer().WriteLine("edit-quiz <quiz id>");
    this->Writer().WriteLine("quizzes");
    this->Writer().WriteLine("quizzes <usernane>");
    this->Writer().WriteLine("like-quiz <quiz id>");
    this->Writer().WriteLine("unlike-quiz <quiz id>");
    this->Writer().WriteLine("add-to-favs <quiz id>");
    this->Writer().WriteLine("remove-from-favs <quiz id>");
    this->Writer().WriteLine("start-quiz <quiz id> test | normal (shuffle)");
    this->Writer().WriteLine("save-quiz <quiz id> <filepath>");
    this->Writer().WriteLine("report-quiz <quiz id> <reason>");
}

void Player::SaveData()
{
    String s = this->BuildUserData();
    this->Provider().Action(s, ProviderOptions::UserSave);
}

String Player::BuildUserData()
{
    String result = User::BuildUserData();

    result += String::UIntToString(this->level) + NEW_LINE;
    result += String::UIntToString(this->points) + NEW_LINE;
    result += String::UIntToString(this->numberCreatedQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberLikedQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberFavoriteQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberFinishedChallenges) + NEW_LINE;
    result += String::UIntToString(this->numberSolvedTestQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberSolvedNormalQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberCreatedQuizzesChallengers) + NEW_LINE;

    for (size_t i = 0; i < this->listCreatedQuizzes.getSize(); i++)
    {
        result = result + this->listCreatedQuizzes[i] + NEW_LINE;
    }

    for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listLikedQuizzes[i]) + NEW_LINE;
    }

    for (size_t i = 0; i < this->listFavoriteQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listFavoriteQuizzes[i]) + NEW_LINE;
    }

    for (size_t i = 0; i < this->listFinishedChallenges.getSize(); i++)
    {
        result = result + this->listFinishedChallenges[i] + NEW_LINE;
    }

    return result;
}
