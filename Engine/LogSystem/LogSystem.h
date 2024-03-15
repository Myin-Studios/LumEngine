#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;

class CMLog
{
public:
	static void Debug(const string& msg)
	{
		// current date and time on the current system
		time_t now = time(0);

		tm* t = new tm;

		t = localtime(&now);

		cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
	}

    static void Warning(const string& msg)
    {
        // current date and time on the current system
        time_t now = time(0);

        tm* t = new tm;

        t = localtime(&now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }

    static void Error(const string& msg)
    {
        time_t now = time(0);

        tm* t = new tm;

        t = localtime(&now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // you can loop k higher to see more color choices
        SetConsoleTextAttribute(hConsole, 12);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }

    static void Succeed(const string& msg)
    {
        time_t now = time(0);

        tm* t = new tm;

        t = localtime(&now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // you can loop k higher to see more color choices
        SetConsoleTextAttribute(hConsole, 10);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }
};