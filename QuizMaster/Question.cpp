#include "Question.h"

String Question::getDescription() const
{
    return this->description;
}

void Question::setPoints(unsigned int points)
{
    this->points = points;
}

unsigned int Question::getPoints() const
{
    return this->points;
}

int Question::getQt() const
{
    return this->qt;
}

void Question::setQt(QuestionType qt)
{
    this->qt = qt;
}

String Question::getCorrectAnswer() const
{
    return this->correctAnswer;
}

bool Question::getIsTest() const
{
    return this->isTest;
}

unsigned char Question::getNumOfAnswers() const
{
    return this->numOfAnswers;
}

Question::Question(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest, unsigned char numOfAnswers)
    : writer(writer)
    , reader(reader)
    , description(description)
    , correctAnswer(correctAnswer)
    , points(points)
    , isTest(isTest)
    , numOfAnswers(numOfAnswers)
{

}

void Question::PrintTestCondition()
{
    if (this->getIsTest())
    {
        this->writer->WriteLine("The correct answer is: " + this->correctAnswer);
    }
}

void Question::PrintQuestion()
{
    this->Writer()->Write("Enter your answer here: ");
}

bool Question::AnswerAQuestion()
{
    return true;
}

IReader* Question::Reader()
{
    return this->reader;
}

IWriter* Question::Writer()
{
    return this->writer;
}