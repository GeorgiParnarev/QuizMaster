#include "Message.h"

Message::Message(IFileBaseProvider* provider)
    : provider(provider)
{
}

String Message::FindAllMessages()
{
    String s = MESSAGES_FILE_NAME;

    this->provider->Action(s, ProviderOptions::MessagesLoad);

    if (s == ERROR)
    {
        s = EMPTY_STRING;
    }

    return s;
}

void Message::SaveNewMessage(String& s)
{
    Vector<String> messagesVec;

    String allMessages = this->FindAllMessages();

    String::Split(ROW_DATA_SEPARATOR, messagesVec, allMessages);

    messagesVec.push_back(s);

    String result;
    String::Join(ROW_DATA_SEPARATOR, messagesVec, result);

    this->SaveMessage(result);
}

void Message::SaveMessage(String& s)
{
    String messages = MESSAGES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + s;
    this->provider->Action(messages, ProviderOptions::MessagesSave);
}

void Message::SendMessage(String& s)
{
    this->SaveNewMessage(s);
}