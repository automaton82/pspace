// Minimal Linux stubs for FileLogger class
#include "FileLogger.h"

FileLogger::FileLogger() {}
FileLogger::~FileLogger() {}

const FILE* FileLogger::getLog() const { return nullptr; }
void FileLogger::setLog(FILE* log) {}
void FileLogger::setHeader(const string& header) {}
string FileLogger::getHeader() const { return ""; }
void FileLogger::setShowTimestamp(bool show) {}
bool FileLogger::getShowTimestamp() const { return false; }
void FileLogger::log(const char* format, ...) const {}
void FileLogger::debugLog(const char* format, ...) const {}
