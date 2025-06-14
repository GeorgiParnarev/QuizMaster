#pragma once

#include "String.h"

enum ProviderOptions
{
	UserLoad,
	UserSave,
	UserFind,
	NewUserSave,
	QuizLoad,
	QuizSave,
	QuizFind,
	QuizIndexSave,
	ConfigSave,
	ConfigLoad,
};

enum QuestionType
{
	TF,
	SC,
	MC,
	ShA,
	MP,
};

enum QuizStatus
{
	NewQuiz = 0,
	EditQuiz,
	ApprovedQuiz,
	RejectedQuiz,
	RemovedQuiz,
	LikeQuiz,
	UnlikeQuiz,
};

enum UserOptions
{
	Empty = 0b00000000,
	OK = 0b00000001,
	NotFound = 0b00000010,
	WrongPassword = 0b00000100,
	AlreadyExisist = 0b00001000,
	Ban = 0b00010000,
	NewUserCreated = 0b00100000,
};

const String CONFIG_FILE_NAME = "config.txt";
const String USERS_FILE_NAME = "users.txt";
const String QUIZZES_FILE_NAME = "quizzes.txt";

const String EXIT = "exit";
const String LOGIN = "login";
const String SIGNUP = "signup";
const String LOGOUT = "logout";
const String HELP = "help";

const bool EXSIST = true;
const bool NOT_EXSIST = false;

const char FILENAME_TO_DATA_SEPARATOR_CHAR = '$';
const char ROW_DATA_SEPARATOR = '\n';
const char ELEMENT_DATA_SEPARATOR = ' ';
const char QUIZ_ELEMENT_DATA_SEPARATOR = '|';
const char MESSAGE_ELEMENT_DATA_SEPARATOR = '|';
const char COMMA_DATA_SEPARATOR = ',';
const char QUOTES_DATA_SEPARATOR = '"';
const char CREATED_QUIZ_SEPARATOR = '#';

const String FILENAME_TO_DATA_SEPARATOR_STRING = "$";
const String EMPTY_STRING = "";
const String PROMPT_STRING = "> ";
const String NEW_LINE = "\n";
const String CREATED_QUIZ_SEPARATOR_STRING = "#";
const String QUIZ_ELEMENT_SEPARATOR = "|";
const String MESSAGE_ELEMENT_SEPARATOR = "|";
const String ERROR = "error";
