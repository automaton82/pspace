#include "FileLogBase.h"

FileLogBase::~FileLogBase()
{
}

const FILE*	FileLogBase::getFile() const
{
	return file_;
}

void FileLogBase::setFile(FILE* file)
{
	file_ = file;
}
