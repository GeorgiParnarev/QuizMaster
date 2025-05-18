#include "User.h"

void User::Help()
{
    this->writer->WriteLine("signup <first-name> <last-name> <username> <password1> <password2>");
    this->writer->WriteLine("login <username> <password>");
    this->writer->WriteLine("logout");
    this->writer->WriteLine("help");
    this->writer->WriteLine("exit");
}
