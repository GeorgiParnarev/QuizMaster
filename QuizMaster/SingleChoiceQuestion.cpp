#include "SingleChoiceQuestion.h"
#include "GlobalConstants.h"

Vector<String>& SingleChoiceQuestion::getQuestions()
{
    return this->questions;
}

SingleChoiceQuestion::SingleChoiceQuestion(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest)
    : Question::Question(writer, reader, description, correctAnswer, points, isTest, 4)
{
    this->setQt(QuestionType::SC);
}

SingleChoiceQuestion::SingleChoiceQuestion(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest, unsigned int questionsCount)
    : Question::Question(writer, reader, description, correctAnswer, points, isTest, questionsCount)
{
    this->setQt(QuestionType::SC);
}

unsigned int SingleChoiceQuestion::Action()
{
    unsigned int result = this->getPoints();

    this->PrintQuestion();

    if (!this->AnswerAQuestion())
    {
        result = 0;
    }
    this->Writer()->WriteLine(EMPTY_STRING);

    this->PrintTestCondition();

    return result;
}

void SingleChoiceQuestion::SetUpData(String& dataString)
{
    Vector<String> quest;

    String::Split(ROW_DATA_SEPARATOR, quest, dataString);

    for (int i = 0; i < quest.getSize(); ++i)
    {
        this->questions.push_back(quest[i]);
    }
}

String SingleChoiceQuestion::BuildQuestionData()
{
    String result = String::UIntToString(this->getQt()) + NEW_LINE;
    result += this->getDescription() + NEW_LINE;
    result += this->getCorrectAnswer() + NEW_LINE;
    result += String::UIntToString(this->getPoints()) + NEW_LINE;
    result += String::UIntToString(this->questions.getSize()) + NEW_LINE;

    for (int i = 0; i < this->questions.getSize(); ++i)
    {
        result += this->questions[i] + NEW_LINE;
    }

    return result;
}

void SingleChoiceQuestion::PrintQuestion()
{
    this->Writer()->WriteLine(this->getDescription() + "\t(" + String::UIntToString(this->getPoints()) + " points)");

    for (int i = 0; i < this->questions.getSize(); ++i)
    {
        this->Writer()->WriteLine("this.questions[i]");
    }

    Question::PrintQuestion();
}

bool SingleChoiceQuestion::AnswerAQuestion()
{
    bool result = false;

    String* answer = this->Reader()->ReadLine();

    if (*answer == this->getCorrectAnswer())
    {
        result = true;
    }

    delete answer;
    answer = nullptr;

    return result;
}

String SingleChoiceQuestion::ToStringFile()
{
    String result = "Description: " + this->getDescription() + NEW_LINE;

    char* arr = new char[2] {'\0'};

    result += "Posible answers:" + NEW_LINE;

    for (size_t i = 0; i < this->getQuestions().getSize(); i++)
    {
        arr[0] = 'a' + i;
        String questNum = String(arr);

        result += questNum + ") " + this->getQuestions()[i] + NEW_LINE;
    }

    delete[] arr;
    arr = nullptr;

    result += "Correct answer: " + this->getCorrectAnswer() + NEW_LINE;

    return result;
}