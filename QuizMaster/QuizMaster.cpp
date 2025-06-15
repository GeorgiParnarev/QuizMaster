// https://github.com/GeorgiParnarev/QuizMaster
// Георги Парнарев 0IM0600547

#include <iostream>

#include "ConsoleWriter.h"
#include "ConsoleReader.h"
#include "FileBaseProvider.h"
#include "Game.h"

int main()
{
    ConsoleWriter* writer = new ConsoleWriter();
    ConsoleReader* reader = new ConsoleReader();
    FileBaseProvider* provider = new FileBaseProvider();

    IGame* game = new Game(writer, reader, provider);
    try
    {

        game->Init();
        game->Run();
    }
    catch (String s)
    {
        writer->WriteLine(s);
    }

    delete game;
    game = nullptr;

    delete writer;
    writer = nullptr;

    delete reader;
    reader = nullptr;

    delete provider;
    provider = nullptr;

    return 0;
}
