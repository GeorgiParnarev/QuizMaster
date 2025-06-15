#pragma once

#include "Question.h"
#include "IQuestion.h"

class TrueOrFalseQuestion : public Question, public IQuestion
{
public:
    TrueOrFalseQuestion(IWriter*, IReader*, String&, String&, unsigned int, bool);
    virtual unsigned int Action() override;
    virtual void SetUpData(String&) override;
    virtual String BuildQuestionData() override;
    virtual bool AnswerAQuestion() override;
    virtual void PrintQuestion() override;
    virtual String ToStringFile() override;
    virtual ~TrueOrFalseQuestion() = default;
};
