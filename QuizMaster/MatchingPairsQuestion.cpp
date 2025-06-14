#include "MatchingPairsQuestion.h"

MatchingPairsQuestion::MatchingPairsQuestion(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest, unsigned int questionsCount)
    : MultipleChoiceQuestion::MultipleChoiceQuestion(writer, reader, description, correctAnswer, points, isTest, questionsCount)
{
    this->setQt(QuestionType::MP);
}

Vector<String>& MatchingPairsQuestion::GetAnswersVec()
{
    return this->answers;
}

void MatchingPairsQuestion::SetUpData(String& s)
{
    throw "NotImplement - void MatchingPairsQuestion::SetUpData(String& s)";
    //TODO
}


String MatchingPairsQuestion::BuildQuestionData()
{
    String result = MultipleChoiceQuestion::BuildQuestionData();

    result += String::UIntToString(this->answers.getSize()) + NEW_LINE;

    for (size_t i = 0; i < this->answers.getSize(); i++)
    {
        result += this->answers[i] + NEW_LINE;
    }

    return result;
}

String MatchingPairsQuestion::ToStringFile()
{
    String result = "Description: " + this->getDescription() + NEW_LINE;

    char* arr = new char[2] {'\0'};

    result += "First group answers:" + NEW_LINE;

    for (size_t i = 0; i < this->getQuestions().getSize(); i++)
    {
        arr[0] = 'A' + i;
        String questNum = String(arr);

        result += questNum + ") " + this->getQuestions()[i] + NEW_LINE;
    }

    result += "Second group answers:" + NEW_LINE;

    for (size_t i = 0; i < this->GetAnswersVec().getSize(); i++)
    {
        arr[0] = 'a' + i;
        String questNum = String(arr);

        result += questNum + ") " + this->GetAnswersVec()[i] + NEW_LINE;
    }

    delete[] arr;
    arr = nullptr;

    result += "Correct answer: " + this->getCorrectAnswer() + NEW_LINE;

    return result;
}

void MatchingPairsQuestion::PrintQuestion()
{
    this->Writer()->WriteLine(this->getDescription() + "\t(" + String::UIntToString(this->getPoints()) + " points)");

    for (int i = 0; i < this->getQuestions().getSize(); ++i)
    {
        this->Writer()->WriteLine(this->getQuestions()[i]);
    }

    this->Writer()->WriteLine(EMPTY_STRING);

    for (int i = 0; i < this->GetAnswersVec().getSize(); ++i)
    {
        this->Writer()->WriteLine(this->GetAnswersVec()[i]);
    }

    Question::PrintQuestion();
}