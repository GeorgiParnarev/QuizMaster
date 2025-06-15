#include <iostream>

#include "Game.h"
#include "User.h"
#include "Administrator.h"
#include "Player.h"
#include "GlobalConstants.h"

Game::Game(IWriter* writer, IReader* reader, IFileBaseProvider* provider)
    : writer(writer)
    , reader(reader)
    , provider(provider)
    , maxUserId(0)
    , maxQuizId(0)
{
}

void Game::Free()
{
    delete this->user;
    this->user = nullptr;
    delete this->command;
    this->command = nullptr;
}

Game::~Game()
{
    this->Free();
}

void Game::Init()
{
    this->command = new CommandStruct();
    this->user = new User(this->writer, this->reader, this->provider);
    this->LoadConfig();
}

void Game::Run()
{
    this->GameLoop();
}

void Game::GameLoop()
{
    bool isLoopExit = false;

    while (!isLoopExit)
    {
        this->SetCommandStruct();

        if (this->command->command == EXIT && this->command->paramRange == 1)
        {
            isLoopExit = true;
            this->Exit();
        }
        else if (this->command->command == LOGIN && this->command->paramRange == 3)
        {
            this->LoginUser();
        }
        else if (this->command->command == LOGOUT && this->command->paramRange == 1)
        {
            if (this->user->getIsHasLogin())
            {
                this->LogoutUser();
            }
        }
        else if (this->command->command == SIGNUP && this->command->paramRange == 6)
        {
            this->SignupUser();
        }
        else
        {
            this->user->Action(*this->command);
        }

        //TODO
    }
}

void Game::SetCommandStruct()
{
    Vector<String> commandLine;
    this->command->command = EMPTY_STRING;

    this->writer->Write(PROMPT_STRING);

    String* s = this->reader->ReadLine();
    String::Split(ELEMENT_DATA_SEPARATOR, commandLine, *s);

    this->command->CommandLine = *s;
    this->command->paramRange = commandLine.getSize();

    delete s;
    s = nullptr;

    if (commandLine.getSize() > 0)
    {
        this->command->command = commandLine[0];

        for (size_t i = commandLine.getSize(); i < 6; i++)
        {
            commandLine.push_back(EMPTY_STRING);
        }

        this->command->Param1 = commandLine[1];
        this->command->Param2 = commandLine[2];
        this->command->Param3 = commandLine[3];
        this->command->Param4 = commandLine[4];
        this->command->Param5 = commandLine[5];
    }
}

void Game::Exit()
{
    if (this->user->getIsHasLogin())
    {
        this->LogoutUser();
    }

    this->SaveConfig();

    this->Free();
}

void Game::LoadConfig()
{
    Vector<String> v;
    String configString;
    this->provider->Action(configString, ProviderOptions::ConfigLoad);
    String::Split('\n', v, configString);

    this->maxUserId = v[0].StringToInt();
    this->maxQuizId = v[1].StringToInt();
}

void Game::SaveConfig()
{
    String configString = String::UIntToString(this->maxUserId) + "\n" + String::UIntToString(this->maxQuizId);

    this->provider->Action(configString, ProviderOptions::ConfigSave);
}

void Game::LoginUser()
{
    if (this->user->getIsHasLogin())
    {
        this->writer->WriteLine("You cannot log in a new user before logging out!");
        return;
    }

    UserStruct* us = new UserStruct();

    us->username = this->command->Param1;
    us->password = this->command->Param2;

    int uo = this->user->FindUserData(*us, EXSIST);

    if (uo == UserOptions::NotFound)
    {
        this->writer->WriteLine("User not found!");
    }
    else if (uo == UserOptions::WrongPassword)
    {
        this->writer->WriteLine("Wrong password!");
    }
    else if (uo == UserOptions::Ban)
    {
        this->writer->WriteLine("Sorry, the user has been banned!");
    }
    else if ((uo & UserOptions::OK) == UserOptions::OK)
    {
        this->LoadUser(*us);
        String s = "Welcome " + this->user->getName() + "!";
        this->writer->WriteLine(s);
    }

    delete us;
    us = nullptr;
}

void Game::LogoutUser()
{
    this->user->SaveData();
    delete this->user;
    this->user = nullptr;

    if (this->command->command != EXIT)
    {
        this->user = new User(this->writer, this->reader, this->provider);
    }
}

void Game::SignupUser()
{
    if (this->user->getIsHasLogin())
    {
        this->writer->WriteLine("You cannot log in a new user before logging out!");
        return;
    }

    UserStruct* us = new UserStruct();

    us->firstName = this->command->Param1;
    us->lastName = this->command->Param2;
    us->username = this->command->Param3;
    us->password = this->command->Param4;

    int uo = this->user->FindUserData(*us, NOT_EXSIST);

    if ((uo & UserOptions::AlreadyExisist) == UserOptions::AlreadyExisist)
    {
        this->writer->WriteLine("Such a user already exists!");
        delete us;
        us = nullptr;
        return;
    }
    else if (us->password != this->command->Param5)
    {
        this->writer->WriteLine("Passwords do not match!");
        delete us;
        us = nullptr;
        return;
    }

    Vector<String> usersVec;
    String users;
    this->user->AllUsers(users);
    String::Split(ROW_DATA_SEPARATOR, usersVec, users);

    String newUser = us->username + " " + String::UIntToString(this->user->Hash(us->password)) + " ";

    String fileName = us->username + String::UIntToString(++this->maxUserId) + ".txt";

    newUser += fileName + " " + String::UIntToString(this->maxUserId) + " " + String::UIntToString(UserOptions::OK);
    us->fileName = fileName;
    us->id = this->maxUserId;
    us->password = EMPTY_STRING;
    usersVec.push_back(newUser);

    String usersString = EMPTY_STRING;

    String::Join(ROW_DATA_SEPARATOR, usersVec, usersString);

    this->provider->Action(usersString, ProviderOptions::NewUserSave);

    Player* newPlayer = new Player(this->writer, this->reader, this->provider, us, UserOptions::NewUserCreated);

    this->writer->WriteLine("Signup " + us->username + " successful!");

    this->SaveConfig();

    delete us;
    us = nullptr;

    delete newPlayer;
    newPlayer = nullptr;
}

void Game::SaveUser()
{
    //TODO
}

void Game::LoadUser(UserStruct& us)
{
    if (us.id <= 10)
    {
        delete this->user;
        this->user = nullptr;

        this->user = new Administrator(this->writer, this->reader, this->provider);
    }
    else
    {
        this->user = new Player(this->writer, this->reader, this->provider, this);
    }

    Vector<String> v;

    this->user->SetUpUserData(us, v, UserOptions::Empty);
}

unsigned int Game::getMaxQuizId() const
{
    return this->maxQuizId;
}

void Game::setMaxQuizId(unsigned int quizId)
{
    this->maxQuizId = quizId;
}