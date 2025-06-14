#include "ShortAnswerQuestion.h"

ShortAnswerQuestion::ShortAnswerQuestion(IWriter* writer, IReader* reader, String& description, String& correctAnswer, unsigned int points, bool isTest)
	: TrueOrFalseQuestion::TrueOrFalseQuestion(writer, reader, description, correctAnswer, points, isTest)
{
	this->setQt(QuestionType::ShA);
}