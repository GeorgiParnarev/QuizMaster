#pragma once

#include "User.h"

class Administrator : public User
{
public:
	Administrator(IWriter*, IReader*, IFileBaseProvider*);
	virtual ~Administrator() {};


	virtual void Help() override;
	virtual void SaveData() override;
};