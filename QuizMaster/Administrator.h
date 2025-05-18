#pragma once

#include "User.h"

class Administrator : public User
{
public:
	virtual void Help() override;
};