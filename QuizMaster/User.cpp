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

String User::BuildUserData()
{
	String result = EMPTY_STRING;

	result += this->fileName + FILENAME_TO_DATA_SEPARATOR_STRING;
	result += this->firstName + NEW_LINE;
	result += this->lastName + NEW_LINE;

	return result;
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

int User::FindUserIndex(UserStruct& us, Vector<String>& usersVec)
{
	Vector<String> v;

	int result = -1;

	int i = 0;

	bool isLoopExit = false;
	bool isFound = false;
	bool notEmptyVector = usersVec.getSize() > 0;

	while (notEmptyVector && !(isLoopExit || isFound))
	{
		v.clear();
		String user = usersVec[i];
		String::Split(ELEMENT_DATA_SEPARATOR, v, user);

		if (us.username == v[0])
		{
			isFound = true;
			result = i;
		}

		i++;

		if (i >= usersVec.getSize())
		{
			isLoopExit = true;
		}
	}

	return result;
}

void User::AllUsers(String& users)
{
	this->provider->Action(users, ProviderOptions::UserFind);
}

int User::FindUserData(UserStruct& us, bool exsist)
{
	Vector<String> usersVec, v;
	String users;
	this->AllUsers(users);
	String::Split(ROW_DATA_SEPARATOR, usersVec, users);

	int userIndex = this->FindUserIndex(us, usersVec);

	if (userIndex > -1)
	{
		String user = usersVec[userIndex];
		String::Split(ELEMENT_DATA_SEPARATOR, v, user);
		if (exsist && this->Hash(us.password) != v[1].StringToInt())
		{
			return UserOptions::WrongPassword;
		}
		else if (exsist && (v[4].StringToInt() & UserOptions::Ban) == UserOptions::Ban)
		{
			return UserOptions::Ban;
		}

		us.fileName = v[2];
		us.id = v[3].StringToInt();
		us.firstName = v[0];
		us.lastName = v[1];

		return (UserOptions::Empty | UserOptions::OK | UserOptions::AlreadyExisist);
	}

	return UserOptions::NotFound;
}

void User::SetUpUserData(UserStruct& us, Vector<String>& v, UserOptions uo)
{
	if ((uo & UserOptions::NewUserCreated) == UserOptions::NewUserCreated)
	{
		this->firstName = us.firstName;
		this->lastName = us.lastName;
	}
	else
	{
		String s = us.fileName;
		this->provider->Action(s, ProviderOptions::UserLoad);

		String::Split(ROW_DATA_SEPARATOR, v, s);

		this->firstName = v[0];
		this->lastName = v[1];
		this->setIsHasLogin(true);
	}

	this->fileName = us.fileName;
	this->id = us.id;
	this->username = us.username;
}

void User::Help()
{
	this->writer->WriteLine("signup <first-name> <last-name> <username> <password1> <password2>");
	this->writer->WriteLine("login <username> <password>");
	this->writer->WriteLine("logout");
	this->writer->WriteLine("help");
	this->writer->WriteLine("exit");
}
