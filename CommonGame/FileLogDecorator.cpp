#include "FileLogDecorator.h"

#include <windows.h>

FileLogDecorator::~FileLogDecorator()
{
}

void FileLogDecorator::writeArgs(const char* format, va_list argList) const
{
	base_->writeArgs(format, argList);
}

