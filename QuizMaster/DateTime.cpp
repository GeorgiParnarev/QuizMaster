#include "DateTime.h"

String DateTime::DateNow()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    String result = String::UIntToString(timeinfo->tm_mday) + "." + String::UIntToString(timeinfo->tm_mon + 1) + "." + String::UIntToString(timeinfo->tm_year + 1900);

    return result;
}