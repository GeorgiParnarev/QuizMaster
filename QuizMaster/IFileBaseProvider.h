#pragma once

#include "GlobalConstants.h"

class IFileBaseProvider
{
public:
	virtual void Action(String& str, ProviderOptions options) = 0;
};