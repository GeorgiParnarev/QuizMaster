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
    this->quiz = new Quiz(this->writer, this->reader, this->provider, "", "", 0, 0, 0);
    this->message = new Message(this->provider);
}

User::~User()
{
    if (this->quiz != nullptr)
    {
        delete this->quiz;
        this->quiz = nullptr;
    }

    if (this->message != nullptr)
    {
        delete this->message;
        this->message = nullptr;
    }
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

void User::setUsername(const String userName)
{
    this->username = userName;
}

void User::setId(const unsigned int id)
{
    this->id = id;
}

unsigned int User::getId() const
{
    return this->id;
}

String User::getUsername() const
{
    return this->username;
}

String User::getFileName() const
{
    return this->fileName;
}

void User::setFileName(const String fileName)
{
    this->fileName = fileName;
}

unsigned int User::getPassword() const
{
    return this->password;
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

void User::Action(CommandStruct& cmdStr)
{
    if (cmdStr.command == HELP && cmdStr.paramRange == 1)
    {
        this->Help();
    }

    //TODO
}

bool User::GenerateReason(CommandStruct& cmdStr)
{
    Vector<String> v, v1;
    String::Split(ELEMENT_DATA_SEPARATOR, v, cmdStr.CommandLine);

    size_t size = v.getSize();

    if (size > 1)
    {
        for (size_t i = 2; i < size; i++)
        {
            v1.push_back(v[i]);
        }

        String s;

        String::Join(ELEMENT_DATA_SEPARATOR, v1, s);
        cmdStr.Param2 = s;

        return true;
    }

    return false;
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

bool User::getIsHasLogin() const
{
    return this->isHasLogin;
}

void User::setIsHasLogin(bool isLogin)
{
    this->isHasLogin = isLogin;
}

void User::setPassword(unsigned int password)
{
    this->password = password;
}

void User::Print()
{
    this->writer->WriteLine("I am User");
}

void User::SaveData()
{
    return;
}

String User::BuildUserData()
{
    String result = EMPTY_STRING;

    char* arr = new char[2] {'\0'};
    arr[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;

    result += this->fileName + String(arr);

    arr[0] = ROW_DATA_SEPARATOR;
    String newLine = String(arr);

    result += this->firstName + newLine;
    result += this->lastName + newLine;

    delete[] arr;

    return result;
}

Quiz& User::GetQuiz()
{
    return *quiz;
}

Message& User::GetMessage()
{
    return *message;
}

void User::Help()
{
    this->writer->WriteLine("signup <first-name> <last-name> <username> <password1> <password2>");
    this->writer->WriteLine("login <username> <password>");
    this->writer->WriteLine("logout");
    this->writer->WriteLine("help");
    this->writer->WriteLine("exit");
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
        us.password = v[1];

        return (UserOptions::Empty | UserOptions::OK | UserOptions::AlreadyExisist);
    }

    return UserOptions::NotFound;
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
    this->password = us.password.StringToInt();
}