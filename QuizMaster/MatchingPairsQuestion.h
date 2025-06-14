#pragma once

#include "MultipleChoiceQuestion.h"

class MatchingPairsQuestion : public MultipleChoiceQuestion
{
private:
	const String MP_SEPARATOR_STRING = "<PairsSeparator>";
	Vector<String> answers;

public:
	MatchingPairsQuestion(IWriter*, IReader*, String&, String&, unsigned int, bool, unsigned int);

	Vector<String>& GetAnswersVec();

	virtual void SetUpData(String&) override;
	virtual String BuildQuestionData() override;
	virtual void PrintQuestion() override;
	virtual String ToStringFile() override;

	virtual ~MatchingPairsQuestion() {};
};
