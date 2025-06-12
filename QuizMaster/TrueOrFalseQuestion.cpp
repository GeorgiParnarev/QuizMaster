#include "TrueOrFalseQuestion.h"
#include "GlobalConstants.h"

TrueOrFalseQuestion::TrueOrFalseQuestion(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest)
    : Question::Question(writer, reader, description, correctAnswer, points, isTest, 1)
{
    this->setQt(QuestionType::TF);
}

unsigned int TrueOrFalseQuestion::Action()
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

void TrueOrFalseQuestion::SetUpData(String& dataString)
{
    //TODO
}

String TrueOrFalseQuestion::ToStringFile()
{
    String result = "Description: " + this->getDescription() + NEW_LINE;
    result += "Correct answer: " + this->getCorrectAnswer() + NEW_LINE;

    return result;
}

String TrueOrFalseQuestion::BuildQuestionData()
{
    char* arr = new char[2] {'\0'};
    arr[0] = ROW_DATA_SEPARATOR;
    String newLine(arr);

    delete[] arr;

    String result = String::UIntToString(this->getQt()) + newLine;
    result += this->getDescription() + newLine;
    result += this->getCorrectAnswer() + newLine;
    result += String::UIntToString(this->getPoints()) + newLine;

    return result;
}

bool TrueOrFalseQuestion::AnswerAQuestion()
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

void TrueOrFalseQuestion::PrintQuestion()
{
    this->Writer()->WriteLine(this->getDescription() + "\t(" + String::UIntToString(this->getPoints()) + " points)");
    Question::PrintQuestion();
}