#ifndef _ASCIIUTIL_H_
#define _ASCIIUTIL_H_

#include <string>
using std::string;

#include <vector>
using std::vector;

namespace AsciiUtil
{
	static const string Whitespace = " \t\n\r\0";

	bool isAlpha(char c);
	bool isNumeric(char c);
	bool isNumeric(const string& s);
	bool isWhiteSpace(char c);
	bool isEndOfLine(char c);

	string itoa(int number, int radix = 10);	//max digits = 255
	string itoaFixed(int number, int width, int radix = 10);

	string ftoa(double number, unsigned int decimalPlaces);			//assumes conversion to base 10

	string emptyString(unsigned int width);	//empty with spaces
	string filledString(unsigned int width, char value); //string filled with all value

	enum CenterType
	{
		CENTERED,
		LEFT_JUSTIFIED,
		RIGHT_JUSTIFIED,
		UNKNOWN_JUSTIFIED,

		// these are deprecated
		centered = CENTERED,
		leftJustified = LEFT_JUSTIFIED,
		rightJustified = RIGHT_JUSTIFIED,

		unknownCenterType = UNKNOWN_JUSTIFIED
	};

	struct fixedWidthStringData
	{
		fixedWidthStringData(const string& s, unsigned int fixedWidth, CenterType just = leftJustified) :
			value(s), width(fixedWidth), justification(just)
		{}

		string value;
		unsigned int width;
		CenterType justification;
	};
	typedef fixedWidthStringData fwStr;
	typedef vector<fwStr> fwStrList;

	string fixedWidthString(const string& s, unsigned int width, CenterType centerType = leftJustified);
	string fixedWidthString(const fwStrList& fwsList);

	bool matchesIn(char c, const string& matches);

	vector<string> tokenize(const string& s, const string& separators = Whitespace);
	vector<string> tokenizeWithTokens(const string& s, const string& separators = Whitespace);
	//string getToken(const string& s, unsigned int offset, char stopChar = 0);
	string getToken(const string& s, unsigned int offset, const string& separators = Whitespace);
	unsigned int getTokenIndex(const string& s, unsigned int offset, const string& separators = Whitespace);
	string getWord(const string& s, unsigned int offset, const string& separators = Whitespace, bool ignoreFrontSeparators = false);
	unsigned int getWordOffset(const string& s, unsigned int offset, const string& separators = Whitespace, bool ignoreFrontSeparators = false);
	string clipWhitespace(const string& s, bool front = true, bool back = true);
	string trim(const string& s);

	string condense(const string& s, const string& values = Whitespace);
	string condenseWhitespace(const string& s);

	string tolower(const string& s);
	string toupper(const string& s);

	string printfString(const char* format, ...);

	//time formatting
	string timeFormat(double timeInSeconds, bool showHours = true, bool showMinutes = true, bool showSeconds = true);

	//word wrap
	vector<string> wordWrap(const string& s, unsigned int lineLength, const string& separators = Whitespace);
		//number of lines string takes up by using word wrap
	unsigned int countWrapLines(const string& s, unsigned int lineLength, const string& separators = Whitespace);
}

#endif