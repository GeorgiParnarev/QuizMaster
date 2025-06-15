#include "Quiz.h"
#include "UserStruct.h"
#include "Question.h"
#include "QuizIndexDTO.h"
#include "User.h"

Quiz::Quiz(IWriter* writer, IReader* reader, IFileBaseProvider* provider, String name, String username, unsigned int id, unsigned int numberOfQuestions, unsigned int numberOfLikes)
    : writer(writer)
    , reader(reader)
    , provider(provider)
    , quizName(name)
    , username(username)
    , id(id)
    , numberOfQuestions(numberOfQuestions)
    , numberOfLikes(numberOfLikes)
{
}

String Quiz::getQuizName() const
{
    return this->quizName;
}

String Quiz::getUsername() const
{
    return this->username;
}

String Quiz::getUserFullName() const
{
    UserStruct* us = new UserStruct();
    UserStruct& refUs = *us;

    us->username = username;

    User* user = new User(writer, reader, provider);

    int uo = user->FindUserData(*us, NOT_EXSIST);

    String result = us->firstName + " " + us->lastName;

    delete user;
    user = nullptr;

    return result;
}

unsigned int Quiz::getNumberOfQuestions() const
{
    return this->numberOfQuestions;
}

unsigned int Quiz::getId() const
{
    return this->id;
}

void Quiz::setId(unsigned int id)
{
    this->id = id;
}

unsigned int Quiz::getLikes() const
{
    return this->numberOfLikes;
}

void Quiz::IncrementLikes(int likes)
{
    if ((this->numberOfLikes > 0 && likes < 0) || likes > 0)
    {
        this->numberOfLikes += likes;
    }
}

Vector<IQuestion*>& Quiz::getQuestions()
{
    return this->questions;
}

String Quiz::FindAllQuizzes()
{
    String s = QUIZZES_FILE_NAME;

    this->provider->Action(s, ProviderOptions::QuizFind);

    if (s == ERROR)
    {
        s = EMPTY_STRING;
    }

    return s;
}

void Quiz::SaveQuiz(QuizStatus qs, unsigned int quizId)
{
    String quizFileName;
    String s = this->FindAllQuizzes();

    if (qs == QuizStatus::NewQuiz)
    {
        Vector<String> v;

        String::Split(ROW_DATA_SEPARATOR, v, s);

        char* arr = new char[2] {'\0'};

        arr[0] = QUIZ_ELEMENT_DATA_SEPARATOR;

        String separator = String(arr);

        quizFileName = String::UIntToString(this->getId()) + "Quiz.txt";

        String newQuizString = String::UIntToString(this->getId()) + separator + this->getQuizName() + separator + this->getUsername() + separator + quizFileName;
        newQuizString += separator + String::UIntToString(qs) + separator + String::UIntToString(this->getNumberOfQuestions());
        newQuizString += separator + String::UIntToString(this->getLikes());

        v.push_back(newQuizString);

        String allQuizzesString;
        String::Join(ROW_DATA_SEPARATOR, v, allQuizzesString);

        arr[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;

        allQuizzesString = QUIZZES_FILE_NAME + String(arr) + allQuizzesString;

        this->provider->Action(allQuizzesString, ProviderOptions::QuizIndexSave);

        delete[] arr;
        arr = nullptr;
    }
    else if (qs == QuizStatus::ApprovedQuiz)
    {
        Vector<String> quizzesVec, quizVec, resultVec;

        String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

        for (size_t i = 0; i < quizzesVec.getSize(); i++)
        {
            quizVec.clear();
            String quizString = quizzesVec[i];

            String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, quizString);
            unsigned int id = quizVec[0].StringToInt();

            if (id == quizId)
            {
                String approveString = quizVec[0] + QUIZ_ELEMENT_SEPARATOR + quizVec[1] + QUIZ_ELEMENT_SEPARATOR + quizVec[2] + QUIZ_ELEMENT_SEPARATOR + quizVec[3];
                approveString += QUIZ_ELEMENT_SEPARATOR + String::UIntToString(QuizStatus::ApprovedQuiz) + QUIZ_ELEMENT_SEPARATOR + quizVec[5] + QUIZ_ELEMENT_SEPARATOR + quizVec[6];
                resultVec.push_back(approveString);
            }
            else
            {
                resultVec.push_back(quizString);
            }
        }

        String allQuizzesString;
        String::Join(ROW_DATA_SEPARATOR, resultVec, allQuizzesString);

        char* arr2 = new char[2] {'\0'};
        arr2[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;

        allQuizzesString = QUIZZES_FILE_NAME + String(arr2) + allQuizzesString;

        this->provider->Action(allQuizzesString, ProviderOptions::QuizIndexSave);

        delete[] arr2;
        arr2 = nullptr;
    }
    else if (qs == QuizStatus::LikeQuiz)
    {
        Vector<String> quizzesVec, resultVec;

        String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

        for (size_t i = 0; i < quizzesVec.getSize(); i++)
        {
            String quizString = quizzesVec[i];

            QuizIndexDTO qiDTO;

            qiDTO.SetElement(quizString);

            if (qiDTO.id == quizId)
            {
                qiDTO.likes++;
                String incLikeString = qiDTO.ToIndexString();
                resultVec.push_back(incLikeString);
            }
            else
            {
                resultVec.push_back(quizString);
            }
        }

        String allQuizzesString;
        String::Join(ROW_DATA_SEPARATOR, resultVec, allQuizzesString);

        allQuizzesString = QUIZZES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + allQuizzesString;

        this->provider->Action(allQuizzesString, ProviderOptions::QuizIndexSave);
    }
    else if (qs == QuizStatus::DislikeQuiz)
    {
        Vector<String> quizzesVec, resultVec;

        String::Split(ROW_DATA_SEPARATOR, quizzesVec, s);

        for (size_t i = 0; i < quizzesVec.getSize(); i++)
        {
            String quizString = quizzesVec[i];

            QuizIndexDTO qiDTO;

            qiDTO.SetElement(quizString);

            if (qiDTO.id == quizId)
            {
                qiDTO.likes--;
                String incLikeString = qiDTO.ToIndexString();
                resultVec.push_back(incLikeString);
            }
            else
            {
                resultVec.push_back(quizString);
            }
        }

        String allQuizzesString;
        String::Join(ROW_DATA_SEPARATOR, resultVec, allQuizzesString);

        allQuizzesString = QUIZZES_FILE_NAME + FILENAME_TO_DATA_SEPARATOR_STRING + allQuizzesString;

        this->provider->Action(allQuizzesString, ProviderOptions::QuizIndexSave);
    }

    if (qs == QuizStatus::NewQuiz)
    {
        char* arr1 = new char[2] {'\0'};
        arr1[0] = FILENAME_TO_DATA_SEPARATOR_CHAR;

        String allQuizData = quizFileName + String(arr1) + this->getQuizName() + NEW_LINE + String::UIntToString(this->getNumberOfQuestions());
        allQuizData += NEW_LINE + this->getUsername() + NEW_LINE;

        for (size_t i = 0; i < this->getNumberOfQuestions(); i++)
        {
            allQuizData += this->getQuestions()[i]->BuildQuestionData();
        }

        this->provider->Action(allQuizData, ProviderOptions::QuizSave);

        arr1[0] = QUOTES_DATA_SEPARATOR;
        this->writer->WriteLine("Quiz " + String(arr1) + this->getQuizName() + String(arr1) + " with ID " + String::UIntToString(this->getId()) + " sent for admin approval!");

        delete[] arr1;
        arr1 = nullptr;
    }
}