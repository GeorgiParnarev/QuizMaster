#pragma once

#include "String.h"

class IQuestion
{
public:
    virtual unsigned int Action() = 0;
    virtual void SetUpData(String&) = 0;
    virtual String BuildQuestionData() = 0;
    virtual String ToStringFile() = 0;
};