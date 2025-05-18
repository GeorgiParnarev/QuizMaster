#pragma once

#include "String.h"

enum ProviderOptions
{
	UserLoad,
	UserSave,
	UserFind,
	NewUserSave,
	QuizzeLoad,
	QuizzeSave,
	ConfigSave,
	ConfigLoad,
};

const String CONFIG_FILE_NAME = "config.txt";
const String USERS_FILE_NAME = "users.txt";

const char FILENAME_TO_DATA_SEPARATOR = '$';
const char ROW_DATA_SEPARATOR = '\n';
const char ELEMENT_DATA_SEPARATOR = ' ';