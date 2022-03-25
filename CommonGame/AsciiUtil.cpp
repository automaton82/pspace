#include "AsciiUtil.h"

#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdarg.h>

#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

namespace AsciiUtil
{

bool isAlpha(char c)
{
	if(c >= ' ' && c <= '~')
		return true;
	else 
		return false;
}

bool isEndOfLine(char c)
{
	if(c == '\n' || c == '\r')
		return true;
	else 
		return false;
}

bool isNumeric(char c)
{
	if(c >= '0' && c <= '9')
		return true;
	else
		return false;
}

bool isNumeric(const string& s)
{
	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		if(!isNumeric(*i))
			return false;
	}

	return true;
}

bool isWhiteSpace(char c)
{
	if(	c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return true;
	else
		return false;
}

string itoa(int number, int radix)
{
	char buffer[255];

	::itoa(number, buffer, radix);

	return buffer;
}

string itoaFixed(int number, int width, int radix)
{
	assert(width > 0);

	char buffer[255];
	string val, retval;
	//int i;

	::itoa(number, buffer, radix);

	val = buffer;

	retval = filledString( max(0, width-(int)val.size()), '0') + val.substr(0, width);
	/*for(i = 0; i < width - (int)val.size(); ++i)
	{												//insert zeros
		retval += '0';
	}
	retval += val.substr(0, width);	//only include 'width' characters for larger strings*/

	return retval;
}

string ftoa(double number, unsigned int decimalPlaces)
{
	int decimal, sign;
	char* buffer = fcvt(number, decimalPlaces, &decimal, &sign);
	string numStr(buffer);

	string retval;
	if(sign < 0)
		retval += '-';
	
	if(number >= 1)
		retval += numStr.substr(0, decimal);
	else
		retval += "0";

	if(decimalPlaces > 0)
	{
		retval += '.';
		if(decimal >= 0)				//TODO: why does decimal give -1 on error?
			retval += numStr.substr(decimal);
		else
			retval += filledString(decimalPlaces, '0');
	}

	return retval;
}

string /*AsciiUtil::*/emptyString(unsigned int width)
{
	/*string result;
	result.resize(width);

	for(unsigned int i=0; i < width; ++i)
	{
		result[i] = ' ';
	}

	return result;*/

	return filledString(width, ' ');
}

string filledString(unsigned int width, char value)
{
	string result;
	//result.resize(width);

	for(unsigned int i=0; i < width; ++i)
	{
		//result[i] = value;
		result += value;
	}

	return result;
}

string /*AsciiUtil::*/fixedWidthString(const string& str, unsigned int width, CenterType centerType)
{
#ifdef _DEBUG
	if(str.size() != string(str.c_str()).size())
	{
		printf("Warning: fixedWidthString %s[%d], %s.c_str()[%d] has bad null string values\n", str.c_str(), str.size(), str.c_str(), string(str.c_str()).size());
	}
	//assert(str.size() == string(str.c_str()).size());	//checks for strings like "test\0\0\0\0\0", which is length 9, not 4
#endif

	string s = str.c_str();
	
	string result;
	//result.resize(width);
	
	switch(centerType)
	{
	case leftJustified:
		if(s.size() >= width)
		{
			result = s.substr(0, width);
		}
		else
		{
			result = s + emptyString(width - s.size());
		}
		break;
	case rightJustified:
		if(s.size() <= width)
		{
            result = emptyString(width - s.size()) + s;
		}
		else
		{
			result = s.substr(s.size() - width);
		}
		break;
	case centered:
		unsigned int frontSpacing, backSpacing;

		if(s.size() <= width)
		{	
			frontSpacing = (width - s.size()) / 2;
			backSpacing = width - (frontSpacing + s.size());
			result = emptyString(frontSpacing) + s + emptyString(backSpacing);
		}
		else
		{
			frontSpacing = ((unsigned int)s.size() - width) / 2;
			result = s.substr(frontSpacing, width);
		}
		break;
	default:	//left justified
		result = s.substr(0, width);
		break;
	}

	assert(result.size() == width);

	return result;
}

string fixedWidthString(const fwStrList& fwsList)
{
	string result;
	
	fwStrList::const_iterator i;
	for(i = fwsList.begin(); i != fwsList.end(); ++i)
	{
		const fwStr& fws = (*i);

		string temp = fixedWidthString(fws.value, fws.width, fws.justification);
		//printf("Adding to FWS: \"%s\"[%d], \"%s\"[%d]\n", fws.value.c_str(), fws.value.size(), temp.c_str(), temp.size());
		
		result += fixedWidthString(fws.value, fws.width, fws.justification);
		//result += fws.value;
		//result += "test";

		//printf("FWS Result: \"%s\"\n", result.c_str());
	}
	//printf("FWS: \"%s\"\n", result.c_str());

	return result;
}

bool matchesIn(char c, const string& matches)
{
	size_t i = matches.find(c);

	return (i != string.npos);	
}

vector<string> tokenize(const string& s, const string& separator)
{
	vector<string> tokens;
	string currentToken;
	
	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		char c = (*i);

		if(matchesIn(c, separator))	
		{
			tokens.push_back(currentToken);
			currentToken.clear();
		}
		else
		{
			currentToken += c;
		}
	}
	if(currentToken.size() > 0)
		tokens.push_back(currentToken);

	return tokens;
}

vector<string> tokenizeWithTokens(const string& s, const string& separator)
{
	vector<string> tokens;
	string currentToken;
	string parseToken;
		
	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		char c = (*i);

		if(matchesIn(c, separator))	
		{
			if(currentToken.size() > 0)
			{
				tokens.push_back(currentToken);
				currentToken.clear();
			}

			parseToken += c;
		}
		else
		{
			if(parseToken.size() > 0)
			{
				tokens.push_back(parseToken);
				parseToken.clear();
			}

			currentToken += c;
		}
	}
	if(currentToken.size() > 0)
		tokens.push_back(currentToken);
	if(parseToken.size() > 0)
		tokens.push_back(parseToken);

	return tokens;
}

/*string getToken(const string& s, unsigned int offset, char stopChar)
{
	string result;

	string::const_iterator i = s.begin() + offset;
	for(i; i != s.end(); ++i)
	{
		char c = (*i);

		if(c == stopChar)
			break;

		result += c;
	}

	return result;
}*/

string getToken(const string& s, unsigned int offset, const string& separators)
{
	size_t endOffset = s.find_first_of(separators, offset);
	if(endOffset == s.npos)
		endOffset = s.size();

	return s.substr(offset, endOffset-offset);
}

unsigned int getTokenIndex(const string& s, unsigned int offset, const string& separators)
{
	size_t endOffset = s.find_first_of(separators, offset);
	if(endOffset == s.npos)
		endOffset = s.size();

	return endOffset;
}

string getWord(const string& s, unsigned int offset, const string& separators, bool ignoreFrontSeparators)
{
	string result;

	/*string::const_iterator i = s.begin() + offset;
	if(ignoreFrontSeparators)
	{
		while(matchesIn(*i, separators))//isWhiteSpace(*i))
			++i;
	}

	for(i; i != s.end(); ++i)
	{
		char c = (*i);

		if(!matchesIn(c, separators))//isWhiteSpace(c))
			result += c;
		else
			break;
	}*/

	size_t startOffset = offset;
	size_t endOffset = s.size(); 
	if(ignoreFrontSeparators)
	{
		startOffset = s.find_first_not_of(separators, offset);
		if(startOffset == s.npos)
			startOffset = s.size();
	}

	endOffset = s.find_first_of(separators, startOffset);
	if(endOffset == s.npos)
		endOffset = s.size();

	result = s.substr(startOffset, endOffset-startOffset);

	return result;
}

unsigned int getWordOffset(const string& s, unsigned int offset, const string& separators, bool ignoreFrontSeparators)
{
	string result;

	size_t startOffset = offset;
	size_t endOffset = s.size(); 
	if(ignoreFrontSeparators)
	{
		startOffset = s.find_first_not_of(separators, offset);
		if(startOffset == s.npos)
			startOffset = s.size();
	}

	endOffset = s.find_first_of(separators, startOffset);
	if(endOffset == s.npos)
		endOffset = s.size();

	return endOffset;
}

string clipWhitespace(const string& s, bool front, bool back)
{
	string result;

	/*string::const_iterator i = s.begin();

	while(isWhiteSpace(*i))	//remove front whitespace
		++i;

	string whitespace;
	for(i; i != s.end(); ++i)
	{
		char c = (*i);

		if(!isWhiteSpace(c))
		{
			result += whitespace;	//add buffered whitespace
			whitespace.clear();

			result += c;
		}
		else
			whitespace += c;
	}*/

	size_t startOffset = 0;
	size_t endOffset = s.size();
	if(front)
	{
		startOffset = s.find_first_not_of(Whitespace);
		if(startOffset == s.npos) 
			startOffset = s.size();
	}

	if(back)
	{
		endOffset = s.find_last_not_of(Whitespace);		//find_last_not_of is odd - doesn't return last value
		if(endOffset == s.npos)
			endOffset = s.size();
	}

	result = s.substr(startOffset, endOffset - startOffset + 1);

	return result;
}

string trim(const string& s)
{
	size_t startOffset = s.find_first_not_of(Whitespace);
	int i = s.size()-1;
	for(i=s.size()-1; i >= 0; --i)
	{
		char c = s[i];	
		if(c != ' ' && c != '\n' && c != '\r' && c != '\t' && c != '\0')		
			break;
	}
	//size_t endOffset = s.find_last_of(Whitespace);		// there is some bug with this, it doesn't recognize '\0'
	size_t endOffset = i;	
	return s.substr(startOffset, endOffset-startOffset+1);
}

string condense(const string& s, const string& values)
{
	string retval;

	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		if(values.find_first_of(*i) == values.npos)	// char is not in condensable chars (values)
			retval += *i;
	}

	return retval;
}

/*string removeWhitespace(const string& s)
{
	condense(s, Whitespace);
}*/

string tolower(const string& s)
{
	string retval;

	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		retval += (char)::tolower(*i);
	}

	return retval;
}

string toupper(const string& s)
{
	string retval;

	string::const_iterator i;
	for(i = s.begin(); i != s.end(); ++i)
	{
		retval += (char)::toupper(*i);
	}

	return retval;
}

string printfString(const char* format, ...)
{
	// TODO: make this less dangerous, the buffer could be too small

	string retval;
	char* buffer = new char[strlen(format)*2 + 1 + 50];
	
	va_list argList;

	va_start(argList, format);
	vsprintf(buffer, format, argList);
	va_end(argList);

	retval = buffer;
	delete [] buffer;

	return retval;
}

string timeFormat(double timeInSeconds, bool showHours, bool showMinutes, bool showSeconds)
{
	string retval;

	int hours = (int)(timeInSeconds / 3600);
	timeInSeconds -= hours*3600;

	int minutes = (int)(timeInSeconds / 60);
	timeInSeconds -= minutes*60;

	int seconds = (int)(timeInSeconds);

	if(showHours)
		retval += itoaFixed(hours, 2);

	if(showMinutes)
	{
		if(showHours)
			retval += ":";
		retval += itoaFixed(minutes, 2);
	}

	if(showSeconds)
	{
		if(showHours || showSeconds)
			retval += ":";
		retval += itoaFixed(seconds, 2);
	}

	return retval;
}

vector<string> wordWrap(const string& s, unsigned int lineLength, const string& separators)
{
	assert(lineLength > 0);

	vector<string> retval;
	size_t offset = 0;
	//size_t line = 0;
	string word;
	size_t wordOffset = 0;
	size_t wordIndex = 0;

	retval.push_back(string());
	
	while(offset < s.size())
	{
		wordIndex = s.find_first_of(separators, offset);
		if(wordIndex == offset)		// on a separator
			wordIndex = s.find_first_not_of(separators, offset);
		word = s.substr(offset, wordIndex - offset);
		offset = wordIndex;
		
		if(retval.back().size() + word.size() <= lineLength)	// fits on this line
			retval.back() += word;							// put on this line			
		else												// longer than leftover line space
		{
			if(retval.back().size() > 0)
				retval.push_back(string());				
		
			retval.back() += word.substr(0, lineLength);
			for(wordOffset=lineLength; wordOffset < word.size(); wordOffset += lineLength)	
			{		
				retval.push_back(string());	
				retval.back() += word.substr(wordOffset, lineLength);	// put piece on this line								
			}
		}
	}

	return retval;
}

unsigned int countWrapLines(const string& s, unsigned int lineLength, const string& separators)
{
	size_t lines = 1;
	size_t offset = 0;
	size_t currentWidth = 0;
	size_t wordLength = 0;
	size_t wordIndex = 0;

	while(offset < s.size())
	{
		wordIndex = s.find_first_of(separators, offset);
		if(wordIndex == offset)		// on a separator
			wordIndex = s.find_first_not_of(separators, offset);
		if(wordIndex == s.npos)		// could occur in searching for separators
			wordIndex = s.size();
		wordLength = wordIndex - offset;		
		offset = wordIndex;
		
		if(currentWidth + wordLength <= lineLength)		// fits on this line
			currentWidth += wordLength;						// put on this line
		else											// longer than leftover line space
		{
			if(wordLength < lineLength)						// fits on one line
			{			
				++lines;										// put on its own line
				currentWidth = wordLength;
			}
			else											// longer than one line
			{
				lines += wordLength / lineLength;				// number of new lines
				currentWidth = wordLength % lineLength;			// put piece on this line
			}
		}
	}

	return lines;
}

}	//namespace AsciiUtil