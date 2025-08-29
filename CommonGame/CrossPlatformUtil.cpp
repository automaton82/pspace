#include "WinUtil.h"
#include <string>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

using namespace std;

string getLocalTime()
{
#ifdef _WIN32
    SYSTEMTIME timeStruct;
    GetLocalTime(&timeStruct);
    
    int hour = timeStruct.wHour;
    int minute = timeStruct.wMinute;
#else
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    int hour = ltm->tm_hour;
    int minute = ltm->tm_min;
#endif

    string ampm;
    if(hour == 0) {
        hour = 12;
        ampm = "am";
    } else if(hour > 12) {
        hour -= 12;
        ampm = "pm";
    } else if(hour == 12) {
        ampm = "pm";
    } else {
        ampm = "am";
    }

    stringstream ss;
    ss << hour << ":" << setfill('0') << setw(2) << minute << ampm;
    return ss.str();
}

string getLocalTime24()
{
#ifdef _WIN32
    SYSTEMTIME timeStruct;
    GetLocalTime(&timeStruct);
    
    int hour = timeStruct.wHour;
    int minute = timeStruct.wMinute;
    int second = timeStruct.wSecond;
#else
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    int hour = ltm->tm_hour;
    int minute = ltm->tm_min;
    int second = ltm->tm_sec;
#endif

    stringstream ss;
    ss << setfill('0') << setw(2) << hour << ":" 
       << setfill('0') << setw(2) << minute << ":" 
       << setfill('0') << setw(2) << second;
    return ss.str();
}
