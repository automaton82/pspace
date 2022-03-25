#include "WinUtil.h"

#include "AsciiUtil.h"

string getLastWindowsError()
{
	DWORD error = GetLastError();

	char buffer[255];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, buffer, 255, 0);

	return buffer;
}

string getLocalTime()
{
	string localTime;

	SYSTEMTIME timeStruct;
	GetLocalTime(&timeStruct);
 
	int hour, minute;
	string ampm;

	if(timeStruct.wHour == 0)
	{
		hour = 12;
		ampm = "am";
	}
	else if(timeStruct.wHour > 12)
	{
		hour = timeStruct.wHour - 12;
		ampm = "pm";
	}
	else
	{
		ampm = "am";
		hour = timeStruct.wHour;
	}

	minute = timeStruct.wMinute;

	localTime = AsciiUtil::itoa(hour) + ":" + AsciiUtil::itoaFixed(minute, 2) + ampm;

	return localTime;
}

string getLocalTime24()
{
	string localTime;

	SYSTEMTIME timeStruct;
	GetLocalTime(&timeStruct);
 
	localTime = AsciiUtil::itoaFixed(timeStruct.wHour, 2) 
				+ ":" + AsciiUtil::itoaFixed(timeStruct.wMinute, 2) 
				+ ":" + AsciiUtil::itoaFixed(timeStruct.wSecond, 2);
	return localTime;
}

BOOL getCursorClientPos(HWND hWnd, LPPOINT point)
{
	BOOL retval = FALSE;
	
	retval = GetCursorPos(point);
	if(retval == FALSE)
		return retval;

	retval = ScreenToClient(hWnd, point);
	return retval;
}