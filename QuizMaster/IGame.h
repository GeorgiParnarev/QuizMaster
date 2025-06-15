#pragma once

class IGame
{
public:
    virtual void Init() = 0;
    virtual void Run() = 0;
    virtual void Exit() = 0;

    virtual unsigned int getMaxQuizId() const = 0;
    virtual void setMaxQuizId(unsigned int) = 0;
    virtual void SaveConfig() = 0;
};