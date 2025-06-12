#pragma once

#include "String.h"
#include "IWriter.h"
#include "IReader.h"
#include "GlobalConstants.h"

class Question 
{
private:
	bool isTest;
	unsigned char numOfAnswers;
	unsigned int points;
	QuestionType qt; 
	String correctAnswer;
	String description;

	IWriter* writer;
	IReader* reader;
protected:
	String getDescription() const;
	unsigned int getPoints() const;
	void setPoints(unsigned int);
	int getQt() const;
	void setQt(QuestionType);
	String getCorrectAnswer() const;
	bool getIsTest() const;
	unsigned char getNumOfAnswers() const;

	void PrintTestCondition();

	IReader* Reader();
	IWriter* Writer();

	virtual void PrintQuestion();
	virtual bool AnswerAQuestion();
public:
	Question(IWriter*, IReader*, String&, String&, unsigned int, bool, unsigned char);
	virtual ~Question() {};
};
