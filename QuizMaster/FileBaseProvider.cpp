#include "FileBaseProvider.h"

void FileBaseProvider::FileSave(String& str)
{
    Vector<String> v;
    String::Split(FILENAME_TO_DATA_SEPARATOR_CHAR, v, str);
    std::ofstream ofs;

    ofs.open(v[0].c_str(), std::fstream::out | std::fstream::trunc);
    if (ofs.is_open())
    {
        for (int i = 0; i < v[1].getSize(); ++i)
        {
            ofs.put(v[1][i]);
        }

        ofs.close();
    }
}

String FileBaseProvider::FileLoad(String& str)
{
    std::ifstream ifs;
    char* arr = new char[1024] {'\0'};


    ifs.open(str.c_str());

    String s = "";

    if (!ifs.is_open())
    {
        std::cerr << "Error opening the file!" << std::endl;
        s = "error";
        return s;
    }

    while (!(ifs.eof() || ifs.fail()))
    {
        ifs.getline(arr, 1024);
        s += String(arr) + String("\n");
    }

    ifs.close();

    //std::cout << s << std::endl;

    delete[] arr;

    return s;
}

void FileBaseProvider::FileDelete(String& str, ProviderOptions options)
{
    //TODO
}

void FileBaseProvider::Action(String& str, ProviderOptions options)
{
    if (options == ProviderOptions::ConfigLoad)
    {
        String s = CONFIG_FILE_NAME;
        str = FileLoad(s);
    }
    else if (options == ProviderOptions::ConfigSave)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;
        String s = CONFIG_FILE_NAME + String(arr) + str;
        FileSave(s);

        delete[] arr;
        arr = nullptr;
    }
    else if (options == ProviderOptions::UserFind)
    {
        String s = USERS_FILE_NAME;
        str = FileLoad(s);
    }
    else if (options == ProviderOptions::NewUserSave || options == ProviderOptions::EditUser)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;
        String s = USERS_FILE_NAME + String(arr) + str;
        FileSave(s);

        delete[] arr;
        arr = nullptr;
    }
    else if ((options == ProviderOptions::UserLoad) || (options == ProviderOptions::QuizFind) || (options == ProviderOptions::MessagesLoad) || (options == ProviderOptions::QuizLoad))
    {
        String s = str;
        str = FileLoad(s);
    }
    else if ((options == ProviderOptions::UserSave) || (options == ProviderOptions::QuizSave) || (ProviderOptions::QuizIndexSave) || (ProviderOptions::MessagesSave))
    {
        String s = str;
        FileSave(s);
    }
}