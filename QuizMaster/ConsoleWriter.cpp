#include "ConsoleWriter.h"

void ConsoleWriter::Write(const String& str)
{
    std::cout << str;
}

void ConsoleWriter::WriteLine(const String& str)
{
    std::cout << str << std::endl;
}