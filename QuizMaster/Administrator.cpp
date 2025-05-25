#include "Administrator.h"

Administrator::Administrator(IWriter* writer, IReader* reader, IFileBaseProvider* provider)
	: User::User(writer, reader, provider)
{
}

void Administrator::Help()
{
	User::Help();

	this->Writer().WriteLine("pending");
	this->Writer().WriteLine("approve-quiz <quiz id>");
	this->Writer().WriteLine("approve-quiz <quiz id>");
	this->Writer().WriteLine("reject-quiz <quiz id> <reason>");
	this->Writer().WriteLine("view-reports");
	this->Writer().WriteLine("ban <username>");
}

String Administrator::BuildUserData()
{
	String result = User::BuildUserData();
	return result;
}

void Administrator::Action(const CommandStruct& cmdStr)
{
	User::Action(cmdStr);
}

void Administrator::SaveData()
{

}