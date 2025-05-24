#include "Game.h"
#include "User.h"

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

void Game::Run()
{
    this->GameLoop();
}

void Game::Init()
{
    this->command = new CommandStruct();
    this->user = new User(this->writer, this->reader, this->provider);
}

void Game::GameLoop()
{
    bool isLoopExit = false;

    while (!isLoopExit)
    {
        this->SetCommandStruct();

        if (this->command->command == EXIT)
        {
            isLoopExit = true;
            //this->Exit();
        }
        else if (this->command->command == LOGIN)
        {
            //this->LoginUser();
        }
        else if (this->command->command == LOGOUT)
        {
            if (this->user->getIsHasLogin())
            {
                //this->LogoutUser();
            }
        }
        else if (this->command->command == SIGNUP)
        {
            //this->SignupUser();
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