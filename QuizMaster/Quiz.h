#pragma once

#include "String.h"
#include "IWriter.h"
#include "IReader.h"
#include "IFileBaseProvider.h"
#include "IQuestion.h"

class Quiz
{
private:
    unsigned int id;
    unsigned int numberOfQuestions;
    unsigned int numberOfLikes;
    String quizName;
    String username;

    IWriter* writer;
    IReader* reader;
    IFileBaseProvider* provider;

    Vector<IQuestion*> questions;

public:
    Quiz(IWriter*, IReader*, IFileBaseProvider*, String, String, unsigned int, unsigned int, unsigned int);
    ~Quiz() = default;
    String getQuizName() const;
    String getUsername() const;
    String getUserFullName() const;
    unsigned int getNumberOfQuestions() const;
    unsigned int getId() const;
    void setId(unsigned int);
    unsigned int getLikes() const;
    void IncrementLikes(int);
    Vector<IQuestion*>& getQuestions();
    void SaveQuiz(QuizStatus, unsigned int);
    String FindAllQuizzes();
};