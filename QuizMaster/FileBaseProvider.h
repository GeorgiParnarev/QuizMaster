#pragma once

#include "IFileBaseProvider.h"

class FileBaseProvider : public IFileBaseProvider
{
private:
	void FileSave(String& str);
	String FileLoad(String& str);
	void FileDelete(String& str, ProviderOptions options);
public:
	virtual void Action(String& str, ProviderOptions options) override;

	virtual ~FileBaseProvider() {}
};