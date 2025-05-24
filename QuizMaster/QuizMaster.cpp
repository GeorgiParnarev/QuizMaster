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

    Game* game = new Game(writer, reader, provider);
    game->Init();
    game->Run();

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
