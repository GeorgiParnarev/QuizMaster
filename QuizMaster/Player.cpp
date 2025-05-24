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

    char* arr = new char[2] {'\0'};

    arr[0] = ROW_DATA_SEPARATOR;

    String newLine(arr);

    delete[] arr;
    arr = nullptr;

    result += String::UIntToString(this->level) + newLine;
    result += String::UIntToString(this->points) + newLine;
    result += String::UIntToString(this->numberCreatedQuizzes) + newLine;
    result += String::UIntToString(this->numberLikedQuizzes) + newLine;
    result += String::UIntToString(this->numberFavoriteQuizzes) + newLine;
    result += String::UIntToString(this->numberFinishedChallenges) + newLine;
    result += String::UIntToString(this->numberSolvedTestQuizzes) + newLine;
    result += String::UIntToString(this->numberSolvedNormalQuizzes) + newLine;
    result += String::UIntToString(this->numberCreatedQuizzesChallengers) + newLine;

    for (size_t i = 0; i < this->listCreatedQuizzes.getSize(); i++)
    {
        result = result + this->listCreatedQuizzes[i] + newLine;
    }

    for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listLikedQuizzes[i]) + newLine;
    }

    for (size_t i = 0; i < this->listFavoriteQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listFavoriteQuizzes[i]) + newLine;
    }

    for (size_t i = 0; i < this->listFinishedChallenges.getSize(); i++)
    {
        result = result + this->listFinishedChallenges[i] + newLine;
    }

    return result;
}
