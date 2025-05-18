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
