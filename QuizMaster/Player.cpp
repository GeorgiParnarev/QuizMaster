#include "Player.h"

Player::Player(IWriter* writer, IReader* reader, IFileBaseProvider* provider, UserStruct* us, UserOptions uo)
    : User::User(writer, reader, provider)
    , game(nullptr)
{
    this->Init();
    Vector<String> v;
    this->SetUpUserData(*us, v, uo);
    this->SaveData();
}

Player::Player(IWriter* writer, IReader* reader, IFileBaseProvider* provider, IGame* game)
    : User::User(writer, reader, provider)
    , game(game)
{
    this->Init();
}

void Player::Init()
{
    Vector<String> a, b;
    Vector<unsigned int> c, d;
    this->listCreatedQuizzes = a;
    this->listLikedQuizzes = c;
    this->listFavoriteQuizzes = d;
    this->listFinishedChallenges = b;
}

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

void Player::SetUpUserData(UserStruct& us, Vector<String>& v, UserOptions uo)
{
    User::SetUpUserData(us, v, uo);

    if ((uo & UserOptions::NewUserCreated) == UserOptions::NewUserCreated)
    {
        this->level = 0;
        this->points = 0;
        this->numberCreatedQuizzes = 0;
        this->numberLikedQuizzes = 0;
        this->numberFavoriteQuizzes = 0;
        this->numberFinishedChallenges = 0;
        this->numberSolvedTestQuizzes = 0;
        this->numberSolvedNormalQuizzes = 0;
        this->numberCreatedQuizzesChallengers = 0;
        return;
    }

    this->level = v[2].StringToInt();
    this->points = v[3].StringToInt();
    this->numberCreatedQuizzes = v[4].StringToInt();
    this->numberLikedQuizzes = v[5].StringToInt();
    this->numberFavoriteQuizzes = v[6].StringToInt();
    this->numberFinishedChallenges = v[7].StringToInt();
    this->numberSolvedTestQuizzes = v[8].StringToInt();
    this->numberSolvedNormalQuizzes = v[9].StringToInt();
    this->numberCreatedQuizzesChallengers = v[10].StringToInt();

    int i = 11;
    int j = i;

    for (; i < j + this->numberCreatedQuizzes; ++i)
    {
        this->listCreatedQuizzes.push_back(v[i]);
    }

    j = i;

    for (; i < j + this->numberLikedQuizzes; ++i)
    {
        this->listLikedQuizzes.push_back(v[i].StringToInt());
    }

    j = i;

    for (; i < j + this->numberFavoriteQuizzes; ++i)
    {
        this->listFavoriteQuizzes.push_back(v[i].StringToInt());
    }

    j = i;

    for (; i < j + this->numberFinishedChallenges; ++i)
    {
        this->listFinishedChallenges.push_back(v[i]);
    }
}
