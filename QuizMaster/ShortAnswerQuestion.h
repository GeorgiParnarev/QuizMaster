#pragma once

#include "TrueOrFalseQuestion.h"

class ShortAnswerQuestion : public TrueOrFalseQuestion
{
public:
	ShortAnswerQuestion(IWriter*, IReader*, String&, String&, unsigned int, bool);
	virtual ~ShortAnswerQuestion() {};
};
