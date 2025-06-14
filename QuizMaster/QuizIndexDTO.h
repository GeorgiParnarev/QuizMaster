#pragma once

#include "String.h"
#include "GlobalConstants.h"

struct QuizIndexDTO
{
	//id|quizName|userName|quizFileName|QuizStatus|numOfQuestions|Likes
public:
	unsigned int id;
	String quizName;
	String userName;
	String quizFileName;
	unsigned int quizStatus;
	unsigned int numOfQuestions;
	unsigned int likes;

	void SetElement(String);
	String ToIndexString();
};