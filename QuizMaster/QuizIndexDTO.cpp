#include "QuizIndexDTO.h"

void QuizIndexDTO::SetElement(String s)
{
    Vector<String> quizVec;
    String::Split(QUIZ_ELEMENT_DATA_SEPARATOR, quizVec, s);

    this->id = quizVec[0].StringToInt();
    this->quizName = quizVec[1];
    this->userName = quizVec[2];
    this->quizFileName = quizVec[3];
    this->quizStatus = quizVec[4].StringToInt();
    this->numOfQuestions = quizVec[5].StringToInt();
    this->likes = quizVec[6].StringToInt();
}

String QuizIndexDTO::ToIndexString()
{
    String s = String::UIntToString(this->id) + "|" + this->quizName + "|" + this->userName + "|" + this->quizFileName;
    s += "|" + String::UIntToString(this->quizStatus) + "|" + String::UIntToString(this->numOfQuestions) + "|";
    s += String::UIntToString(this->likes);

    return s;
}