#pragma once

#include "IReader.h"

class ConsoleReader : public IReader
{
public:
    virtual String* ReadLine() override;
};