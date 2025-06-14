#pragma once

#include "SingleChoiceQuestion.h"
#include "IQuestion.h"

class MultipleChoiceQuestion : public SingleChoiceQuestion
{
private:
    unsigned char percent;

    void SeparateAnswers(Vector<String>&, String);

public:
    virtual bool AnswerAQuestion() override;

    MultipleChoiceQuestion(IWriter*, IReader*, String&, String&, unsigned int, bool, unsigned int);

    virtual ~MultipleChoiceQuestion() {};
};