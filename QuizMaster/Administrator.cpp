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

void Administrator::ApproveQuiz(const CommandStruct& cmdStr)
{
	String quizIdString = cmdStr.Param1;

	unsigned int quizId = quizIdString.StringToInt();

	String s = this->GetQuiz().FindAllQuizzes();

	Vector<String> quizzesVec, quizVec;

	String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

	for (size_t i = 0; i < quizzesVec.getSize(); i++)
	{
		quizVec.clear();
		String quizString = quizzesVec[i];

		String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, quizString);
		unsigned int id = quizVec[0].StringToInt();
		unsigned int qs = quizVec[4].StringToInt();

		if (id == quizId && (qs == QuizStatus::NewQuiz || qs == QuizStatus::EditQuiz))
		{
			this->GetQuiz().SaveQuiz(QuizStatus::ApprovedQuiz, id);
			return;
		}
	}

	this->Writer().WriteLine("No quiz with the specified id was found pending approval.");
}

void Administrator::Ban(const CommandStruct& cmdStr)
{
	UserStruct* us = new UserStruct();

	us->firstName = EMPTY_STRING;
	us->lastName = EMPTY_STRING;
	us->username = cmdStr.Param1;
	us->password = EMPTY_STRING;

	Vector<String> usersVec, v;
	String users;
	this->AllUsers(users);
	String::Split(ROW_DATA_SEPARATOR, usersVec, users);

	int userIndex = this->FindUserIndex(*us, usersVec);

	if (userIndex < 0)
	{
		this->Writer().WriteLine("No user with that name found!");

		delete us;
		us = nullptr;

		return;
	}

	String::Split(ELEMENT_DATA_SEPARATOR, v, usersVec[userIndex]);

	if (v[3].StringToInt() <= 10)
	{
		this->Writer().WriteLine("Banning an administrator is not allowed!");
	}
	else
	{
		v[4] = String::UIntToString(UserOptions::Empty | UserOptions::Ban);
		String s, s1;

		String::Join(ELEMENT_DATA_SEPARATOR, v, s);

		usersVec[userIndex] = s;
		String::Join(ROW_DATA_SEPARATOR, usersVec, s1);

		this->Provider().Action(s1, ProviderOptions::NewUserSave);
	}

	delete us;
	us = nullptr;
}

void Administrator::Pending()
{
	String s = this->GetQuiz().FindAllQuizzes();

	Vector<String> quizzesVec, quizVec;

	String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

	for (size_t i = 0; i < quizzesVec.getSize(); i++)
	{
		quizVec.clear();
		String quizString = quizzesVec[i];

		String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, quizString);

		if (quizVec[4].StringToInt() == QuizStatus::NewQuiz || quizVec[4].StringToInt() == QuizStatus::EditQuiz)
		{
			String output = "[id " + quizVec[0] + "] " + quizVec[1] + " by " + quizVec[2];
			this->Writer().WriteLine(output);
		}
	}
}

void Administrator::RejectOrRemoveQuiz(String quizIdString, String reason, QuizStatus status)
{
	bool isFound = false;

	unsigned int quizId = quizIdString.StringToInt();

	String ByUserName;

	String s = this->GetQuiz().FindAllQuizzes();

	Vector<String> quizzesVec, quizVec, newQuizzesVec;

	String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

	for (size_t i = 0; i < quizzesVec.getSize(); ++i)
	{
		quizVec.clear();
		String quizString = quizzesVec[i];

		String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, quizString);
		unsigned int id = quizVec[0].StringToInt();

		if (id == quizId)
		{
			ByUserName = quizVec[2];

			isFound = true;

			String quizString = quizVec[0] + QUIZ_ELEMENT_SEPARATOR + quizVec[1] + QUIZ_ELEMENT_SEPARATOR + quizVec[2] + QUIZ_ELEMENT_SEPARATOR + quizVec[3] + QUIZ_ELEMENT_SEPARATOR;
			quizString += String::UIntToString(status) + QUIZ_ELEMENT_SEPARATOR + quizVec[5] + QUIZ_ELEMENT_SEPARATOR + quizVec[6];

			newQuizzesVec.push_back(quizString);
		}
		else
		{
			newQuizzesVec.push_back(quizzesVec[i]);
		}
	}

	if (isFound)
	{
		String allQuizzesString;
		String::Join(ROW_DATA_SEPARATOR, newQuizzesVec, allQuizzesString);

		allQuizzesString = QUIZZES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + allQuizzesString;
		this->Provider().Action(allQuizzesString, ProviderOptions::QuizIndexSave);

		UserStruct* us = new UserStruct();
		us->username = ByUserName;

		this->User::FindUserData(*us, NOT_LOGIN);

		String messages = this->GetMessage().FindAllMessages();
		Vector<String> messagesVec;
		String::Split(ROW_DATA_SEPARATOR, messagesVec, messages);

		String separator = MESSAGE_ELEMENT_SEPARATOR;

		String newMessageString = String::UIntToString(us->id) + separator + reason;

		//UserId|text message - Message to player

		messagesVec.push_back(newMessageString);

		String allMessagesString;
		String::Join(ROW_DATA_SEPARATOR, messagesVec, allMessagesString);

		allMessagesString = MESSAGES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + allMessagesString;

		this->Provider().Action(allMessagesString, ProviderOptions::MessagesSave);

		delete us;
		us = nullptr;
	}
	else
	{
		this->Writer().WriteLine("No quiz found with that id!");
	}
}

void Administrator::ViewReports()
{
	String messagesString = this->GetMessage().FindAllMessages();

	Vector<String> messagesVec, messageVec;

	String::Split(ROW_DATA_SEPARATOR, messagesVec, messagesString);

	for (size_t i = 0; i < messagesVec.getSize(); i++)
	{
		messageVec.clear();
		String message = messagesVec[i];

		String::Split(MESSAGE_ELEMENT_DATA_SEPARATOR, messageVec, message);

		unsigned int id = messageVec[0].StringToInt();

		if (id == 0)
		{
			String msg = messageVec[1] + " | sent By " + messageVec[3] + "	quiz id " + messageVec[2] + " by " + messageVec[4] + " | reason " + messageVec[5];
			this->Writer().WriteLine(msg);
		}
	}
}

void Administrator::Action(CommandStruct& cmdStr)
{
	User::Action(cmdStr);

	if (cmdStr.command == BAN && cmdStr.paramRange == 2)
	{
		this->Ban(cmdStr);
	}
	else if (cmdStr.command == PENDING && cmdStr.paramRange == 1)
	{
		this->Pending();
	}
	else if (cmdStr.command == APPROVE_QUIZ && cmdStr.paramRange == 2)
	{
		this->ApproveQuiz(cmdStr);
	}
	else if ((cmdStr.command == REJECT_QUIZ || cmdStr.command == REMOVE_QUIZ) && cmdStr.paramRange >= 2)
	{
		QuizStatus status = (cmdStr.command == REJECT_QUIZ)
			? QuizStatus::RejectedQuiz
			: QuizStatus::RemovedQuiz;

		if (this->GenerateReason(cmdStr))
		{
			this->RejectOrRemoveQuiz(cmdStr.Param1, cmdStr.Param2, status);
		}
	}
	else if (cmdStr.command == VIEW_REPORTS && cmdStr.paramRange == 1)
	{
		this->ViewReports();
	}
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