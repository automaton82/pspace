// Minimal Linux stubs for TimeStream class
#include "TimeStream.h"

TimeStream::TimeStream() {}
TimeStream::TimeStream(OutputStreamDecorator* base) {}
TimeStream::~TimeStream() {}

void TimeStream::flush() {}
void TimeStream::put(char c) {}
void TimeStream::write(const string& s) {}
