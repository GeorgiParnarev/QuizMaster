#pragma once

#include "IFileBaseProvider.h"

class Message
{
private:
    IFileBaseProvider* provider;

    void SaveNewMessage(String&);
    void SaveMessage(String&);

public:
    Message(IFileBaseProvider*);
    String FindAllMessages();
    void SendMessage(String&);
};