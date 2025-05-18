#include "Administrator.h"

void Administrator::Help()
{
	User::Help();

	this->Writer().WriteLine("pending");
	this->Writer().WriteLine("approve-quiz <quiz id>");
	this->Writer().WriteLine("approve-quiz <quiz id>");
	this->Writer().WriteLine("reject-quiz <quiz id> <reason>");
	this->Writer().WriteLine("view-reports");
	this->Writer().WriteLine("ban <username>");
}