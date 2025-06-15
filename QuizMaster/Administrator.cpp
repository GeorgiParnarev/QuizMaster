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
	this->Writer().WriteLine("reject-quiz <quiz id> <reason>");
	this->Writer().WriteLine("view-reports");
	this->Writer().WriteLine("remove-quiz <quiz id> <reason>");
	this->Writer().WriteLine("ban <username>");
}

void Administrator::Action(CommandStruct& cmdStr)
{
	User::Action(cmdStr);

}

String Administrator::BuildUserData()
{
	String result = User::BuildUserData();
	return result;
}

void Administrator::SaveData()
{
	return;
}