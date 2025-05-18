#include "ConsoleReader.h"
#include "String.h"

String* ConsoleReader::ReadLine()
{

    char* arr = new char[1024] {'\0'};
    std::cin.getline(arr, 1024);
    String* s = new String(arr);

    delete[] arr;
    return s;
}