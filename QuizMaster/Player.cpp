#include "Player.h"
#include "Quiz.h"
#include "QuizIndexDTO.h"
#include "DateTime.h"

Player::Player(IWriter* writer, IReader* reader, IFileBaseProvider* provider, UserStruct* us, UserOptions uo)
    : User::User(writer, reader, provider)
    , game(nullptr)
{
    this->Init();
    Vector<String> v;
    this->SetUpUserData(*us, v, uo);
    this->SaveData();
}

Player::Player(IWriter* writer, IReader* reader, IFileBaseProvider* provider, IGame* game)
    : User::User(writer, reader, provider)
    , game(game)
{
    this->Init();
}

void Player::Init()
{
    Vector<String> a, b;
    Vector<unsigned int> c, d;
    this->listCreatedQuizzes = a;
    this->listLikedQuizzes = c;
    this->listFavoriteQuizzes = d;
    this->listFinishedChallenges = b;
}

void Player::Help()
{
    User::Help();

    this->Writer().WriteLine("view-profile");
    this->Writer().WriteLine("edit-profile");
    this->Writer().WriteLine("view-challenges");
    this->Writer().WriteLine("view-finished-challenges");
    this->Writer().WriteLine("view <nickname>");
    this->Writer().WriteLine("messages");
    this->Writer().WriteLine("create-quiz");
    this->Writer().WriteLine("edit-quiz <quiz id>");
    this->Writer().WriteLine("quizzes");
    this->Writer().WriteLine("quizzes <username>");
    this->Writer().WriteLine("like-quiz <quiz id>");
    this->Writer().WriteLine("dislike-quiz <quiz id>");
    this->Writer().WriteLine("add-to-favs <quiz id>");
    this->Writer().WriteLine("remove-from-favs <quiz id>");
    this->Writer().WriteLine("start-quiz <quiz id> test | normal (shuffle)");
    this->Writer().WriteLine("save-quiz <quiz id> <filepath>");
    this->Writer().WriteLine("report-quiz <quiz id> <reason>");
}

void Player::Action(CommandStruct& cmdStr)
{
    User::Action(cmdStr);

    if (cmdStr.command == VIEW_PROFILE && cmdStr.paramRange == 1)
    {
        this->ViewSelfProfile(VIEW_SELF_PROFILE);
    }
    else if (cmdStr.command == VIEW && cmdStr.paramRange == 2)
    {
        this->ViewOtherProfile(cmdStr.Param1, VIEW_OTHER_PROFILE);
    }
    else if (cmdStr.command == CREATE_QUIZ && cmdStr.paramRange == 1)
    {
        this->CreateQuiz();
    }
    else if (cmdStr.command == QUIZZES && cmdStr.paramRange == 1)
    {
        String s = EMPTY_STRING;
        this->Quizzes(s);
    }
    else if (cmdStr.command == QUIZZES && cmdStr.paramRange == 2)
    {
        this->Quizzes(cmdStr.Param1);
    }
    else if (cmdStr.command == REPORT_QUIZ && cmdStr.paramRange == 3)
    {
        if (this->GenerateReason(cmdStr))
        {
            this->ReportQuiz(cmdStr.Param1, cmdStr.Param2);
        }
    }
    else if (cmdStr.command == MESSAGES && cmdStr.paramRange == 1)
    {
        this->Message();
    }
    else if (cmdStr.command == ADD_TO_FAVS && cmdStr.paramRange == 2)
    {
        this->AddToFavs(cmdStr.Param1);
    }
    else if (cmdStr.command == REMOVE_FROM_FAVS && cmdStr.paramRange == 2)
    {
        this->RemoveFromFavs(cmdStr.Param1);
    }
    else if (cmdStr.command == LIKE_QUIZ && cmdStr.paramRange == 2)
    {
        this->LikeQuiz(cmdStr.Param1);
    }
    else if (cmdStr.command == DISLIKE_QUIZ && cmdStr.paramRange == 2)
    {
        this->DislikeQuiz(cmdStr.Param1);
    }
    else if (cmdStr.command == VIEW_FINISHED_CHALLENGES && cmdStr.paramRange == 1)
    {
        this->PrintFinishedChallenges();
    }
    else if (cmdStr.command == VIEW_CHALLENGES && cmdStr.paramRange == 1)
    {
        this->PrintChallenges();
    }
    else if (cmdStr.command == EDIT_QUIZ && cmdStr.paramRange == 2)
    {
        this->EditQuiz(cmdStr.Param1);
    }
    else if (cmdStr.command == START_QUIZ && cmdStr.paramRange >= 2)
    {
        this->StartQuiz(cmdStr.Param1, cmdStr.Param2, cmdStr.Param3);
    }
    else if (cmdStr.command == SAVE_QUIZ && cmdStr.paramRange == 3)
    {
        this->SaveQuiz(cmdStr.Param1, cmdStr.Param2);
    }
    else if (cmdStr.command == EDIT_PROFILE && cmdStr.paramRange == 1)
    {
        this->EditProfile();
    }
}

void Player::EditProfile()
{
    this->Writer().Write("Enter password: ");
    String* password = this->Reader().ReadLine();

    if (this->Hash(*password) != this->getPassword())
    {
        this->Writer().WriteLine("Wrong password");
        delete password;
        password = nullptr;
        return;
    }

    delete password;
    password = nullptr;

    this->Writer().Write("Enter new password: ");
    String* newPassword = this->Reader().ReadLine();

    this->Writer().Write("Reenter new password: ");
    String* repeatNewPassword = this->Reader().ReadLine();

    if (*newPassword != *repeatNewPassword)
    {
        this->Writer().WriteLine("Passwords do not match");

        delete newPassword;
        newPassword = nullptr;

        delete repeatNewPassword;
        repeatNewPassword = nullptr;

        return;
    }

    delete repeatNewPassword;
    repeatNewPassword = nullptr;

    UserStruct* us = new UserStruct();

    us->firstName = EMPTY_STRING;
    us->lastName = EMPTY_STRING;
    us->username = this->getUsername();
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

    v[1] = String::UIntToString(this->Hash(*newPassword));
    this->setPassword(this->Hash(*newPassword));

    String s, s1;

    String::Join(ELEMENT_DATA_SEPARATOR, v, s);

    usersVec[userIndex] = s;
    String::Join(ROW_DATA_SEPARATOR, usersVec, s1);

    this->Provider().Action(s1, ProviderOptions::EditUser);

    delete newPassword;
    newPassword = nullptr;

    delete us;
    us = nullptr;
}

void Player::SaveQuiz(String& quizId, String& fileName)
{
    Quiz* quiz = LoadQuiz(quizId, false);

    if (quiz != nullptr && quiz->getUsername() == this->getUsername())
    {
        String fileData = fileName + FILENAME_TO_DATA_SEPARATOR_STRING + quiz->getQuizName() + " - " + String::UIntToString(quiz->getNumberOfQuestions()) + " Questions" + NEW_LINE;

        fileData += "By: " + this->getName() + " " + this->getUsername() + NEW_LINE;

        for (size_t i = 0; i < quiz->getQuestions().getSize(); i++)
        {
            fileData += String::UIntToString(i + 1) + ") " + quiz->getQuestions()[i]->ToStringFile();
            if ((i + 1) < quiz->getQuestions().getSize())
            {
                fileData += NEW_LINE;
            }
        }

        this->Provider().Action(fileData, ProviderOptions::QuizSave);
    }
    else
    {
        this->Writer().WriteLine("The quiz was not recorded.");
    }
}

unsigned int* Player::GetOrder(bool isShuffle, unsigned int numberOfQuestions)
{
    unsigned int* arr = new unsigned int[numberOfQuestions];
    for (size_t i = 0; i < numberOfQuestions; i++)
    {
        arr[i] = i;
    }

    if (!isShuffle)
    {
        return arr;
    }

    srand(time(0));
    unsigned int questionSize = numberOfQuestions;

    unsigned int* arr1 = new unsigned int[numberOfQuestions];

    for (int i = 0; i < numberOfQuestions; i++)
    {
        int randomIndex = rand() % questionSize;

        arr1[i] = arr[randomIndex];
        arr[randomIndex] = arr[--questionSize];
    }

    delete[] arr;
    arr = nullptr;

    return arr1;
}

void Player::StartQuiz(String& quizId, String& mode, String& shuffle)
{
    bool isTest = (mode == TEST)
        ? TEST_MODE
        : NORMAL_MODE;

    bool isShuffle = (shuffle == SHUFFLE)
        ? true
        : false;

    Quiz* quiz = LoadQuiz(quizId, isTest);

    if (quiz != nullptr)
    {
        unsigned int* order = this->GetOrder(isShuffle, quiz->getNumberOfQuestions());

        for (size_t i = 0; i < quiz->getNumberOfQuestions(); ++i)
        {
            unsigned int idx = order[i];
            this->AddPoints(quiz->getQuestions()[idx]->Action());
        }

        if (isTest)
        {
            this->numberSolvedTestQuizzes++;
            this->AddQuizChallenge(ChallengerOptions::TestQuizChallenger);
        }
        else
        {
            this->numberSolvedNormalQuizzes++;
            this->AddQuizChallenge(ChallengerOptions::NormalQuizChallenger);
        }

        delete[] order;
        order = nullptr;

        delete quiz;
        quiz = nullptr;
    }
    else
    {
        this->Writer().WriteLine("Quiz not found");
    }
}

Quiz* Player::LoadQuizHeader(Vector<String>& v)
{
    unsigned int numberOfQuestions = v[1].StringToInt();
    String quizName = v[0];
    String userName = v[2];

    Quiz* quiz = new Quiz(&this->Writer(), &this->Reader(), &this->Provider(), quizName, userName, 0, numberOfQuestions, 0);

    return quiz;
}

IQuestion* Player::LoadTF(Vector<String>& quizVec, unsigned int& indexRow, bool isTest)
{
    String description = quizVec[indexRow];
    indexRow++;

    String answer = quizVec[indexRow];
    indexRow++;

    unsigned int points = quizVec[indexRow].StringToInt();
    indexRow++;

    TrueOrFalseQuestion* question = new TrueOrFalseQuestion(&this->Writer(), &this->Reader(), description, answer, points, isTest);

    return question;
}

IQuestion* Player::LoadSC(Vector<String>& quizVec, unsigned int& indexRow, bool isTest)
{
    String description = quizVec[indexRow];
    indexRow++;

    String answer = quizVec[indexRow];
    indexRow++;

    unsigned int points = quizVec[indexRow].StringToInt();
    indexRow++;

    unsigned char numOfAnswers = quizVec[indexRow].StringToInt();
    indexRow++;

    SingleChoiceQuestion* question = new SingleChoiceQuestion(&this->Writer(), &this->Reader(), description, answer, points, isTest);

    for (size_t i = 0; i < numOfAnswers; i++)
    {
        question->getQuestions().push_back(quizVec[indexRow]);
        indexRow++;
    }

    return question;
}

IQuestion* Player::LoadMC(Vector<String>& quizVec, unsigned int& indexRow, bool isTest)
{
    String description = quizVec[indexRow];
    indexRow++;

    String answer = quizVec[indexRow];
    indexRow++;

    unsigned int points = quizVec[indexRow].StringToInt();
    indexRow++;

    unsigned char numOfAnswers = quizVec[indexRow].StringToInt();
    indexRow++;

    MultipleChoiceQuestion* question = new MultipleChoiceQuestion(&this->Writer(), &this->Reader(), description, answer, points, isTest, numOfAnswers);

    for (size_t i = 0; i < numOfAnswers; i++)
    {
        question->getQuestions().push_back(quizVec[indexRow]);
        indexRow++;
    }

    return question;
}

IQuestion* Player::LoadShA(Vector<String>& quizVec, unsigned int& indexRow, bool isTest)
{
    String description = quizVec[indexRow];
    indexRow++;

    String answer = quizVec[indexRow];
    indexRow++;

    unsigned int points = quizVec[indexRow].StringToInt();
    indexRow++;

    ShortAnswerQuestion* question = new ShortAnswerQuestion(&this->Writer(), &this->Reader(), description, answer, points, isTest);

    return question;
}

IQuestion* Player::LoadMP(Vector<String>& quizVec, unsigned int& indexRow, bool isTest)
{
    String description = quizVec[indexRow];
    indexRow++;

    String answer = quizVec[indexRow];
    indexRow++;

    unsigned int points = quizVec[indexRow].StringToInt();
    indexRow++;

    unsigned char numOfAnswers = quizVec[indexRow].StringToInt();
    indexRow++;

    MatchingPairsQuestion* question = new MatchingPairsQuestion(&this->Writer(), &this->Reader(), description, answer, points, isTest, numOfAnswers);

    for (size_t i = 0; i < numOfAnswers; i++)
    {
        question->getQuestions().push_back(quizVec[indexRow]);
        indexRow++;
    }

    unsigned char numOfAnswers1 = quizVec[indexRow].StringToInt();
    indexRow++;

    for (size_t j = 0; j < numOfAnswers1; j++)
    {
        question->GetAnswersVec().push_back(quizVec[indexRow]);
        indexRow++;
    }

    return question;
}

Quiz* Player::LoadQuiz(String& idString, bool isTest)
{
    unsigned int id = idString.StringToInt();
    String quizString = ERROR;
    Quiz* quiz = nullptr;

    String allQuizzes = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, allQuizzes);

    for (size_t i = 0; i < quizzesVec.getSize(); i++)
    {
        QuizIndexDTO qiDTO;
        String quizElement = quizzesVec[i];
        qiDTO.SetElement(quizElement);

        if (id == qiDTO.id && qiDTO.quizStatus == QuizStatus::ApprovedQuiz)
        {
            quizString = qiDTO.quizFileName;
            this->Provider().Action(quizString, ProviderOptions::QuizLoad);
            break;
        }
    }

    if (quizString != ERROR)
    {
        Vector<String> quizVec;
        String::Split(ROW_DATA_SEPARATOR, quizVec, quizString);

        quiz = this->LoadQuizHeader(quizVec);
        quiz->setId(id);
        unsigned int indexRow = 3;

        for (size_t i = 0; i < quiz->getNumberOfQuestions(); i++)
        {
            IQuestion* question = nullptr;

            unsigned int questionType = quizVec[indexRow].StringToInt();
            indexRow++;

            if (questionType == QuestionType::TF)
            {
                question = this->LoadTF(quizVec, indexRow, isTest);
            }
            else if (questionType == QuestionType::ShA)
            {
                question = this->LoadShA(quizVec, indexRow, isTest);
            }
            else if (questionType == QuestionType::SC)
            {
                question = this->LoadSC(quizVec, indexRow, isTest);
            }
            else if (questionType == QuestionType::MP)
            {
                question = this->LoadMP(quizVec, indexRow, isTest);
            }
            else if (questionType == QuestionType::MC)
            {
                question = this->LoadMC(quizVec, indexRow, isTest);
            }

            quiz->getQuestions().push_back(question);
        }
    }

    return quiz;
}

void Player::EditQuiz(String& idString)
{
    Quiz* quiz = LoadQuiz(idString, false);

    delete quiz;
    quiz = nullptr;
}

void Player::PrintChallenges()
{
    for (int i = 5; i <= 30; i += 5)
    {
        if (this->numberCreatedQuizzes < i)
        {
            String s = "Create " + String::UIntToString(i) + " quizzes (" + String::UIntToString(this->numberCreatedQuizzes) + "/" + String::UIntToString(i) + ")";
            this->Writer().WriteLine(s);
        }
    }

    for (int i = 10; i <= 100; i += 10)
    {
        if (this->numberSolvedTestQuizzes < i)
        {
            String s = "Complete " + String::UIntToString(i) + " quizzes in test mode (" + String::UIntToString(this->numberSolvedTestQuizzes) + "/" + String::UIntToString(i) + ")";
            this->Writer().WriteLine(s);
        }
    }

    for (int i = 10; i <= 100; i += 10)
    {
        if (this->numberSolvedNormalQuizzes < i)
        {
            String s = "Complete " + String::UIntToString(i) + " quizzes in normal mode (" + String::UIntToString(this->numberSolvedNormalQuizzes) + "/" + String::UIntToString(i) + ")";
            this->Writer().WriteLine(s);
        }
    }
}

void Player::PrintFinishedChallenges()
{
    for (size_t i = 0; i < this->listFinishedChallenges.getSize(); i++)
    {
        this->Writer().WriteLine(this->listFinishedChallenges[i]);
    }
}

void Player::DislikeQuiz(String& quizId)
{
    unsigned int id = quizId.StringToInt();

    if (this->ContainLikedQuizzes(id))
    {
        Vector<unsigned int> v;

        for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
        {
            if (this->listLikedQuizzes[i] != id)
            {
                v.push_back(this->listLikedQuizzes[i]);
            }
        }

        this->listLikedQuizzes.clear();

        for (size_t i = 0; i < v.getSize(); i++)
        {
            this->listLikedQuizzes.push_back(v[i]);
        }

        this->numberLikedQuizzes = this->listLikedQuizzes.getSize();
        String quizzesString = this->GetQuiz().FindAllQuizzes();
        Vector<String> quizzesVec, quizVec;
        String::Split(ROW_DATA_SEPARATOR, quizzesVec, quizzesString);

        for (size_t i = 0; i < quizzesVec.getSize(); i++)
        {
            quizVec.clear();
            String quizString = quizzesVec[i];

            QuizIndexDTO qiDTO;
            qiDTO.SetElement(quizzesVec[i]);

            if (qiDTO.id == id)
            {
                this->GetQuiz().SaveQuiz(QuizStatus::DislikeQuiz, qiDTO.id);
                return;
            }
        }
    }
    else
    {
        this->Writer().WriteLine("No test with the specified ID was found to be liked.");
    }
}

void Player::LikeQuiz(String& quizId)
{
    String quizzesString = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec, quizVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, quizzesString);

    for (size_t i = 0; i < quizzesVec.getSize(); i++)
    {
        quizVec.clear();
        String quizString = quizzesVec[i];

        QuizIndexDTO qiDTO;

        qiDTO.SetElement(quizzesVec[i]);

        bool isLikedQuiz = !this->ContainLikedQuizzes(qiDTO.id)
            && (qiDTO.quizStatus == QuizStatus::ApprovedQuiz)
            && (qiDTO.id == quizId.StringToInt());

        if (isLikedQuiz)
        {
            this->GetQuiz().SaveQuiz(QuizStatus::LikeQuiz, qiDTO.id);
            this->listLikedQuizzes.push_back(qiDTO.id);
            this->numberLikedQuizzes = this->listLikedQuizzes.getSize();
            return;
        }
    }

    this->Writer().WriteLine("No test with the specified ID was found to be liked.");
}

void Player::RemoveFromFavs(String& quizId)
{
    Vector<unsigned int> newFavs;

    bool isNotFound = true;

    for (size_t i = 0; i < this->listFavoriteQuizzes.getSize(); ++i)
    {
        unsigned int id = this->listFavoriteQuizzes[i];

        if (id != quizId.StringToInt())
        {
            newFavs.push_back(id);
        }
        else
        {
            isNotFound = false;
        }
    }

    if (isNotFound)
    {
        this->Writer().WriteLine("No quiz found with ID " + quizId);
    }
    else
    {
        this->listFavoriteQuizzes.clear();

        for (size_t i = 0; i < newFavs.getSize(); ++i)
        {
            unsigned int id = newFavs[i];
            this->listFavoriteQuizzes.push_back(id);
        }
    }
}

void Player::AddToFavs(String& quizId)
{
    String quizzesString = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, quizzesString);

    bool isNotFound = true;
    size_t i = 0;

    while (i < quizzesVec.getSize() && isNotFound)
    {
        QuizIndexDTO qiDTO;

        qiDTO.SetElement(quizzesVec[i]);

        if (qiDTO.id == quizId.StringToInt())
        {
            this->listFavoriteQuizzes.push_back(qiDTO.id);
            this->numberFavoriteQuizzes = this->listFavoriteQuizzes.getSize();
            isNotFound = false;
        }

        i++;
    }

    if (isNotFound)
    {
        this->Writer().WriteLine("No quiz found with ID " + quizId);
    }
}

void Player::Message()
{
    String messages = this->GetMessage().FindAllMessages();

    Vector<String> v;

    String::Split(ROW_DATA_SEPARATOR, v, messages);

    for (size_t i = 0; i < v.getSize(); i++)
    {
        String message = v[i];
        Vector<String> vv;

        String::Split(MESSAGE_ELEMENT_DATA_SEPARATOR, vv, message);

        if (vv[0].StringToInt() == this->getId())
        {
            this->Writer().WriteLine(vv[1]);
        }
    }
}

void Player::ReportQuiz(String& quizIdString, String& reason)
{
    //id|quizName|userName|quizFileName|QuizStatus|numOfQuestions|Likes

    //UserId = 0|date|quizId|SendUserName|ByUserName|reason - Message toAdmin
    //UserId|text message - Message to player

    unsigned int quizId = quizIdString.StringToInt();

    String sendedUserName = this->getUsername();
    String ByUserName;

    String s = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec, quizVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

    size_t i = 0;

    bool isLoopExit = false;

    while (!isLoopExit)
    {
        quizVec.clear();
        String quizString = quizzesVec[i];

        String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, quizString);
        unsigned int id = quizVec[0].StringToInt();

        if (id == quizId)
        {
            ByUserName = quizVec[2];

            isLoopExit = true;
        }

        i++;

        if (i >= quizzesVec.getSize())
        {
            isLoopExit = true;
        }
    }

    String messages = this->GetMessage().FindAllMessages();
    Vector<String> messagesVec;
    String::Split(ROW_DATA_SEPARATOR, messagesVec, messages);

    String separator = MESSAGE_ELEMENT_SEPARATOR;

    String newMessageString = "0" + separator + DateTime::DateNow() + separator + quizIdString + separator + sendedUserName;
    newMessageString += separator + ByUserName + separator + reason;

    //UserId = 0|date|quizId|SendUserName|ByUserName|reason - Message toAdmin

    messagesVec.push_back(newMessageString);

    String allMessagesString;
    String::Join(ROW_DATA_SEPARATOR, messagesVec, allMessagesString);

    allMessagesString = MESSAGES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + allMessagesString;

    this->Provider().Action(allMessagesString, ProviderOptions::MessagesSave);
}

void Player::Quizzes(String& username)
{
    String s = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

    for (size_t i = 0; i < quizzesVec.getSize(); i++)
    {
        String quizString = quizzesVec[i];

        QuizIndexDTO qiDTO;

        qiDTO.SetElement(quizString);
        bool flag = false;

        if (qiDTO.quizStatus == QuizStatus::ApprovedQuiz)
        {
            if (username == EMPTY_STRING || username == qiDTO.username)
            {
                flag = true;
            }
            else
            {
                flag = false;
            }
        }

        if (flag)
        {
            String output = String::UIntToString(qiDTO.id) + " | " + qiDTO.quizName + " | " + qiDTO.username + " | " + String::UIntToString(qiDTO.numOfQuestions) + " Questions | " + String::UIntToString(qiDTO.likes) + " likes";
            this->Writer().WriteLine(output);

            //id|quizName|useName|quizFileName|QuizStatus|numOfQuestions|Likes
        }
    }
}

TrueOrFalseQuestion* Player::CreateTF(String* description)
{
    this->Writer().Write("Enter correct answer(True/False): ");
    String* answer = this->Reader().ReadLine();

    this->Writer().Write("Enter points: ");
    String* pointsString = this->Reader().ReadLine();

    unsigned int points = pointsString->StringToInt();

    TrueOrFalseQuestion* question = new TrueOrFalseQuestion(&this->Writer(), &this->Reader(), *description, *answer, points, false);

    delete answer;
    answer = nullptr;

    delete pointsString;
    pointsString = nullptr;

    return question;
}

SingleChoiceQuestion* Player::CreateSC(String* description)
{
    Vector<String> v;

    for (char j = 0; j < MAX_LENGTH_SC_QUESTION; j++)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = 'A' + j;

        String questNum = String(arr);

        this->Writer().Write("Enter answer " + questNum + ": ");
        String* answ = this->Reader().ReadLine();

        v.push_back(*answ);

        delete answ;
        answ = nullptr;

        delete[] arr;
        arr = nullptr;
    }

    this->Writer().Write("Enter correct answer (A, B, C, D): ");
    String* answer = this->Reader().ReadLine();

    this->Writer().Write("Enter points: ");
    String* pointsString = this->Reader().ReadLine();

    unsigned int points = pointsString->StringToInt();

    SingleChoiceQuestion* question = new SingleChoiceQuestion(&this->Writer(), &this->Reader(), *description, *answer, points, false);

    for (size_t j = 0; j < v.getSize(); j++)
    {
        question->getQuestions().push_back(v[j]);
    }

    delete answer;
    answer = nullptr;

    delete pointsString;
    pointsString = nullptr;

    return question;
}

MultipleChoiceQuestion* Player::CreateMC(String* description)
{
    this->Writer().Write("Enter possible answer count: ");
    String* possibleAnswerCountString = this->Reader().ReadLine();

    unsigned int possibleAnswerCount = possibleAnswerCountString->StringToInt();

    Vector<String> v;

    bool isFirst = true;

    char* answersString = new char[1024] {'\0'};
    size_t answersStringIndex = 0;

    for (char j = 0; j < possibleAnswerCount; j++)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = 'A' + j;

        String questNum = String(arr);

        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            answersString[answersStringIndex++] = ',';
            answersString[answersStringIndex++] = ' ';
        }

        answersString[answersStringIndex++] = arr[0];

        this->Writer().Write("Enter answer " + questNum + ": ");
        String* answ = this->Reader().ReadLine();

        v.push_back(*answ);

        delete answ;
        answ = nullptr;

        delete[] arr;
        arr = nullptr;
    }

    String s = String(answersString);

    delete[] answersString;
    answersString = nullptr;

    this->Writer().Write("Enter correct answer (" + s + "): ");
    String* answer = this->Reader().ReadLine();

    this->Writer().Write("Enter points: ");
    String* pointsString = this->Reader().ReadLine();

    unsigned int points = pointsString->StringToInt();

    MultipleChoiceQuestion* question = new MultipleChoiceQuestion(&this->Writer(), &this->Reader(), *description, *answer, points, false, possibleAnswerCount);

    for (size_t j = 0; j < v.getSize(); j++)
    {
        question->getQuestions().push_back(v[j]);
    }

    delete answer;
    answer = nullptr;

    delete pointsString;
    pointsString = nullptr;

    delete possibleAnswerCountString;
    possibleAnswerCountString = nullptr;

    return question;
}

ShortAnswerQuestion* Player::CreateShA(String* description)
{
    this->Writer().Write("Enter correct answer: ");
    String* answer = this->Reader().ReadLine();

    this->Writer().Write("Enter points: ");
    String* pointsString = this->Reader().ReadLine();

    unsigned int points = pointsString->StringToInt();

    ShortAnswerQuestion* question = new ShortAnswerQuestion(&this->Writer(), &this->Reader(), *description, *answer, points, false);

    delete answer;
    answer = nullptr;

    delete pointsString;
    pointsString = nullptr;

    return question;
}

MatchingPairsQuestion* Player::CreateMP(String* description)
{
    this->Writer().Write("Enter left column values count: ");
    String* possibleAnswerCountString = this->Reader().ReadLine();
    unsigned int possibleAnswerCount = possibleAnswerCountString->StringToInt();

    delete possibleAnswerCountString;
    possibleAnswerCountString = nullptr;

    Vector<String> v1, v2;

    for (char j = 0; j < possibleAnswerCount; j++)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = 'A' + j;

        String questNum = String(arr);

        this->Writer().Write("Enter value " + questNum + ": ");
        String* answ = this->Reader().ReadLine();

        v1.push_back(*answ);

        delete answ;
        answ = nullptr;

        delete[] arr;
        arr = nullptr;
    }

    this->Writer().Write("Enter right column values count: ");
    possibleAnswerCountString = this->Reader().ReadLine();
    possibleAnswerCount = possibleAnswerCountString->StringToInt();

    delete possibleAnswerCountString;
    possibleAnswerCountString = nullptr;

    for (char j = 0; j < possibleAnswerCount; j++)
    {
        char* arr = new char[2] {'\0'};
        arr[0] = 'a' + j;

        String questNum = String(arr);

        this->Writer().Write("Enter value " + questNum + ": ");
        String* answ = this->Reader().ReadLine();

        v2.push_back(*answ);

        delete answ;
        answ = nullptr;

        delete[] arr;
        arr = nullptr;
    }

    this->Writer().Write("Enter correct pairs: ");
    String* answer = this->Reader().ReadLine();

    this->Writer().Write("Enter points: ");
    String* pointsString = this->Reader().ReadLine();

    unsigned int points = pointsString->StringToInt();

    MatchingPairsQuestion* question = new MatchingPairsQuestion(&this->Writer(), &this->Reader(), *description, *answer, points, false, possibleAnswerCount);

    for (size_t j = 0; j < v1.getSize(); j++)
    {
        question->getQuestions().push_back(v1[j]);
    }

    for (size_t j = 0; j < v2.getSize(); j++)
    {
        question->GetAnswersVec().push_back(v2[j]);
    }

    delete answer;
    answer = nullptr;

    delete pointsString;
    pointsString = nullptr;

    return question;
}

void Player::CreateQuiz()
{
    this->Writer().Write("Enter quiz title: ");
    String* quizName = this->Reader().ReadLine();

    this->Writer().Write("Enter number of questions: ");
    String* numOfQuestionsString = this->Reader().ReadLine();
    unsigned int numOfQuestions = numOfQuestionsString->StringToInt();

    delete numOfQuestionsString;
    numOfQuestionsString = nullptr;

    unsigned int quizId = this->game->getMaxQuizId() + 1;
    this->game->setMaxQuizId(quizId);

    Quiz* quiz = new Quiz(&this->Writer(), &this->Reader(), &this->Provider(), *quizName, this->getUsername(), quizId, numOfQuestions, 0);

    delete quizName;
    quizName = nullptr;

    for (size_t i = 0; i < numOfQuestions; i++)
    {
        this->Writer().Write("Enter question " + String::UIntToString(i + 1) + " type(T/F, SC, MC, ShA, MP): ");
        String* questionType = this->Reader().ReadLine();

        this->Writer().Write("Enter description: ");
        String* description = this->Reader().ReadLine();

        if (*questionType == TF)
        {
            TrueOrFalseQuestion* question = this->CreateTF(description);
            quiz->getQuestions().push_back(question);
        }
        else if (*questionType == SC)
        {
            SingleChoiceQuestion* question = this->CreateSC(description);
            quiz->getQuestions().push_back(question);
        }
        else if (*questionType == MC)
        {
            MultipleChoiceQuestion* question = this->CreateMC(description);
            quiz->getQuestions().push_back(question);
        }
        else if (*questionType == ShA)
        {
            ShortAnswerQuestion* question = this->CreateShA(description);
            quiz->getQuestions().push_back(question);
        }
        else if (*questionType == MP)
        {
            MatchingPairsQuestion* question = this->CreateMP(description);
            quiz->getQuestions().push_back(question);
        }
        else
        {
            this->Writer().WriteLine("Incorrect Question Type");
            i--;
        }

        delete questionType;
        questionType = nullptr;

        delete description;
        description = nullptr;
    }

    quiz->SaveQuiz(QuizStatus::NewQuiz, 0);
    this->game->SaveConfig();

    delete quiz;
    quiz = nullptr;
}

unsigned int Player::PointsForLevel()
{
    if (this->level < 10)
    {
        return 1000;
    }
    else if (this->level < 20)
    {
        return 2000;
    }
    else if (this->level < 30)
    {
        return 3000;
    }

    return 4000;
}

void Player::ViewSelfProfile(DatBuild option)
{
    char* arr = new char[2] {'\0'};

    arr[0] = ROW_DATA_SEPARATOR;

    String newLine(arr);

    String result = this->getName() + " " + this->getUsername() + newLine;
    result += "Level : " + String::UIntToString(this->level);

    if (option == DatBuild::VIEW_SELF_PROFILE)
    {
        arr[0] = '\t';
        result += String(arr) + String::UIntToString(this->points) + "/";
        result += String::UIntToString(this->PointsForLevel()) + " Points to next level";
    }

    delete[] arr;
    arr = nullptr;

    result += newLine;

    if (option == DatBuild::VIEW_OTHER_PROFILE)
    {
        result += newLine;
    }

    result += "Created Quizzes:" + newLine;

    for (size_t i = 0; i < this->listCreatedQuizzes.getSize(); i++)
    {
        String quizString = this->listCreatedQuizzes[i];
        Vector<String> v;
        String::Split(CREATED_QUIZ_SEPARATOR, v, quizString);

        result += "[" + v[0] + "] " + v[1] + newLine;
    }

    if (option == DatBuild::VIEW_SELF_PROFILE)
    {
        result += newLine + "Liked Quizzes: ";

        for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
        {
            result += "[" + String::UIntToString(this->listLikedQuizzes[i]) + "]";
        }

        result += newLine + "Favorite Quizzes: ";

        for (size_t i = 0; i < this->listFavoriteQuizzes.getSize(); i++)
        {
            result += "[" + String::UIntToString(this->listFavoriteQuizzes[i]) + "]";
        }

        result += newLine;
    }

    this->Writer().WriteLine(result);
}

void Player::ViewOtherProfile(const String& username, DatBuild option)
{
    UserStruct* us = new UserStruct();
    UserStruct& refUs = *us;

    us->username = username;

    int uo = this->FindUserData(*us, NOT_EXSIST);

    if (uo == UserOptions::NotFound)
    {
        this->Writer().WriteLine("User not found!");
    }
    else if ((uo & UserOptions::AlreadyExisist) == UserOptions::AlreadyExisist)
    {
        Player* otherPlayer = new Player(&Writer(), &Reader(), &Provider(), this->game);

        if (us->id <= 10)
        {
            otherPlayer->Writer().WriteLine("Unable to display profile information!\n The specified user is an administrator");
        }
        else
        {
            Vector<String> v;
            otherPlayer->SetUpUserData(refUs, v, UserOptions::Empty);
            otherPlayer->ViewSelfProfile(DatBuild::VIEW_OTHER_PROFILE);
        }

        delete otherPlayer;
        otherPlayer = nullptr;
    }

    delete us;
    us = nullptr;
}

void Player::SaveData()
{
    String s = this->BuildUserData();
    this->Provider().Action(s, ProviderOptions::UserSave);
}

String Player::BuildUserData()
{
    String result = User::BuildUserData();

    this->numberCreatedQuizzes = this->listCreatedQuizzes.getSize();
    this->numberLikedQuizzes = this->listLikedQuizzes.getSize();
    this->numberFavoriteQuizzes = this->listFavoriteQuizzes.getSize();
    this->numberFinishedChallenges = this->listFinishedChallenges.getSize();

    result += String::UIntToString(this->level) + NEW_LINE;
    result += String::UIntToString(this->points) + NEW_LINE;
    result += String::UIntToString(this->numberCreatedQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberLikedQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberFavoriteQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberFinishedChallenges) + NEW_LINE;
    result += String::UIntToString(this->numberSolvedTestQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberSolvedNormalQuizzes) + NEW_LINE;
    result += String::UIntToString(this->numberCreatedQuizzesChallengers) + NEW_LINE;

    for (size_t i = 0; i < this->listCreatedQuizzes.getSize(); i++)
    {
        result = result + this->listCreatedQuizzes[i] + NEW_LINE;
    }

    for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listLikedQuizzes[i]) + NEW_LINE;
    }

    for (size_t i = 0; i < this->listFavoriteQuizzes.getSize(); i++)
    {
        result = result + String::UIntToString(this->listFavoriteQuizzes[i]) + NEW_LINE;
    }

    for (size_t i = 0; i < this->listFinishedChallenges.getSize(); i++)
    {
        result = result + this->listFinishedChallenges[i] + NEW_LINE;
    }

    return result;
}

bool Player::ContainLikedQuizzes(unsigned int quizId)
{
    for (size_t i = 0; i < this->listLikedQuizzes.getSize(); i++)
    {
        if (this->listLikedQuizzes[i] == quizId)
        {
            return true;
        }
    }

    return false;
}

bool Player::ContainCreatedQuizzes(unsigned int quizId)
{
    Vector<String> v;

    for (size_t i = 0; i < this->listCreatedQuizzes.getSize(); i++)
    {
        v.clear();
        String::Split(CREATED_QUIZ_SEPARATOR, v, this->listCreatedQuizzes[i]);

        if (v[0].StringToInt() == quizId)
        {
            return true;
        }
    }

    return false;
}

void Player::AddLevel()
{
    unsigned int pointForLevel = this->PointsForLevel();

    if (pointForLevel - this->points <= 0)
    {
        this->level++;
        this->points -= pointForLevel;

        String message = "Level " + String::UIntToString(this->level) + " reached!";

        this->GetMessage().SendMessage(message);
    }
}

void Player::AddPoints(unsigned int point)
{
    this->points += point;

    this->AddLevel();
}

void Player::AddQuizChallenge(ChallengerOptions co)
{
    unsigned int point = 0;

    if (co == ChallengerOptions::CreateChallenger)
    {
        int createdQuizCount = this->listCreatedQuizzes.getSize();

        bool isChalleeng = (createdQuizCount < 31) && (createdQuizCount % 5 == 0);

        if (isChalleeng)
        {
            point = createdQuizCount * 10 / 2;

            String message = String::UIntToString(this->getId()) + MESSAGE_ELEMENT_SEPARATOR;
            message += "New challenge complited! You create " + String::UIntToString(createdQuizCount) + " quizzes! ";
            message += String::UIntToString(point) + " points added.";

            this->GetMessage().SendMessage(message);

            String finishedChaleng = DateTime::DateNow() + MESSAGE_ELEMENT_SEPARATOR + "Commplete " + String::UIntToString(createdQuizCount);
            finishedChaleng += " create quizzes";

            this->listFinishedChallenges.push_back(finishedChaleng);
            this->numberFinishedChallenges = this->listFinishedChallenges.getSize();
            this->numberCreatedQuizzesChallengers++;
        }
    }
    else if (co == ChallengerOptions::NormalQuizChallenger)
    {
        int normalQuizCount = this->numberSolvedNormalQuizzes;

        bool isChalleeng = (normalQuizCount < 101) && (normalQuizCount % 10 == 0);

        if (isChalleeng)
        {
            point = normalQuizCount * 10 / 3;

            String message = String::UIntToString(this->getId()) + MESSAGE_ELEMENT_SEPARATOR;
            message += "New challenge complited! You solved " + String::UIntToString(normalQuizCount) + " quizzes in normal mode! ";
            message += String::UIntToString(point) + " points added.";

            this->GetMessage().SendMessage(message);

            String finishedChaleng = DateTime::DateNow() + MESSAGE_ELEMENT_SEPARATOR + "Commplete " + String::UIntToString(normalQuizCount);
            finishedChaleng += " quizzes in normal mode";

            this->listFinishedChallenges.push_back(finishedChaleng);
            this->numberFinishedChallenges = this->listFinishedChallenges.getSize();
        }
    }
    else if (co == ChallengerOptions::TestQuizChallenger)
    {
        int testQuizCount = this->numberSolvedTestQuizzes;

        bool isChalleeng = (testQuizCount < 101) && (testQuizCount % 10 == 0);

        if (isChalleeng)
        {
            point = testQuizCount * 10 / 3;

            String message = String::UIntToString(this->getId()) + MESSAGE_ELEMENT_SEPARATOR;
            message += "New challenge complited! You solved " + String::UIntToString(testQuizCount) + " quizzes in test mode! ";
            message += String::UIntToString(point) + " points added.";

            this->GetMessage().SendMessage(message);

            String finishedChaleng = DateTime::DateNow() + MESSAGE_ELEMENT_SEPARATOR + "Commplete " + String::UIntToString(testQuizCount);
            finishedChaleng += " quizzes in test mode";

            this->listFinishedChallenges.push_back(finishedChaleng);
        }
    }

    this->AddPoints(point);
}

void Player::SetUpUserData(UserStruct& us, Vector<String>& v, UserOptions uo)
{
    User::SetUpUserData(us, v, uo);

    if ((uo & UserOptions::NewUserCreated) == UserOptions::NewUserCreated)
    {
        this->level = 0;
        this->points = 0;
        this->numberCreatedQuizzes = 0;
        this->numberLikedQuizzes = 0;
        this->numberFavoriteQuizzes = 0;
        this->numberFinishedChallenges = 0;
        this->numberSolvedTestQuizzes = 0;
        this->numberSolvedNormalQuizzes = 0;
        this->numberCreatedQuizzesChallengers = 0;
        return;
    }

    this->level = v[2].StringToInt();
    this->points = v[3].StringToInt();
    this->numberCreatedQuizzes = v[4].StringToInt();
    this->numberLikedQuizzes = v[5].StringToInt();
    this->numberFavoriteQuizzes = v[6].StringToInt();
    this->numberFinishedChallenges = v[7].StringToInt();
    this->numberSolvedTestQuizzes = v[8].StringToInt();
    this->numberSolvedNormalQuizzes = v[9].StringToInt();
    this->numberCreatedQuizzesChallengers = v[10].StringToInt();

    int i = 11;
    int j = i;

    for (; i < j + this->numberCreatedQuizzes; ++i)
    {
        this->listCreatedQuizzes.push_back(v[i]);
    }

    j = i;

    for (; i < j + this->numberLikedQuizzes; ++i)
    {
        this->listLikedQuizzes.push_back(v[i].StringToInt());
    }

    j = i;

    for (; i < j + this->numberFavoriteQuizzes; ++i)
    {
        this->listFavoriteQuizzes.push_back(v[i].StringToInt());
    }

    j = i;

    for (; i < j + this->numberFinishedChallenges; ++i)
    {
        this->listFinishedChallenges.push_back(v[i]);
    }

    String quizzesString = this->GetQuiz().FindAllQuizzes();

    Vector<String> quizzesVec;

    String::Split(ROW_DATA_SEPARATOR, quizzesVec, quizzesString);

    for (size_t i = 0; i < quizzesVec.getSize(); i++)
    {
        QuizIndexDTO qiDTO;

        qiDTO.SetElement(quizzesVec[i]);

        bool isAddedNewQuiz = !this->ContainCreatedQuizzes(qiDTO.id)
            && (qiDTO.quizStatus == QuizStatus::ApprovedQuiz)
            && (qiDTO.username == this->getUsername());

        if (isAddedNewQuiz)
        {
            String createdQuiz = String::UIntToString(qiDTO.id) + CREATED_QUIZ_SEPARATOR_STRING + qiDTO.quizName;

            this->listCreatedQuizzes.push_back(createdQuiz);
            this->numberCreatedQuizzes = this->listCreatedQuizzes.getSize();
            this->AddQuizChallenge(ChallengerOptions::CreateChallenger);
        }
    }
}