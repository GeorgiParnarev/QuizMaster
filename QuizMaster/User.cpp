#include "User.h"

unsigned int User::Hash(const String& str)
{
	unsigned int hash = 1315423911;

	for (std::size_t i = 0; i < str.getSize(); i++)
	{
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}

	return (hash & 0x7FFFFFFF);
}

User::User(IWriter* writer, IReader* reader, IFileBaseProvider* provider)
	: writer(writer)
	, reader(reader)
	, provider(provider)
	, isHasLogin(false)
	, id(0u)
	, password(0)
{

}

String User::getName() const
{
	return this->firstName + " " + this->lastName;
}

void User::setFirstName(const String firstName)
{
	this->firstName = firstName;
}

void User::setLastName(const String lastName)
{
	this->lastName = lastName;
}

String User::getUsername() const
{
	return this->username;
}

void User::setUsername(const String username)
{
	this->username = username;
}

unsigned int User::getId() const
{
	return this->id;
}

void User::setId(const unsigned int id)
{
	this->id = id;
}

bool User::getIsHasLogin() const
{
	return this->isHasLogin;
}

void User::setIsHasLogin(const bool isHasLogin)
{
	this->isHasLogin = isHasLogin;
}

unsigned int User::GetPassword() const
{
	return this->password;
}

void User::SetPassword(unsigned int password)
{
	this->password = password;
}

void User::Login()
{

	if (!this->isHasLogin)
	{

	}

	//TODO
}

void User::Logout()
{
	//TODO
}

void User::SaveData()
{

}

void User::Action(const CommandStruct& cmdStr)
{
	if (cmdStr.command == HELP)
	{
		this->Help();
	}

	//TODO
}

IWriter& User::Writer()
{
	return *writer;
}

IReader& User::Reader()
{
	return *reader;
}

IFileBaseProvider& User::Provider()
{
	return *provider;
}

void User::Help()
{
	this->writer->WriteLine("signup <first-name> <last-name> <username> <password1> <password2>");
	this->writer->WriteLine("login <username> <password>");
	this->writer->WriteLine("logout");
	this->writer->WriteLine("help");
	this->writer->WriteLine("exit");
}
