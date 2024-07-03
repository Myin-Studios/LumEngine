/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;

class Log
{
public:
    template <typename _T>
	static void Debug(const _T& msg)
	{
		// current date and time on the current system
		time_t now = time(0);

		tm* t = new tm;
        localtime_s(t, &now);

		cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg << endl;
	}

    static void Warning(const string& msg)
    {
        // current date and time on the current system
        time_t now = time(0);

        tm* t = new tm;
        localtime_s(t, &now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }

    template <typename _T>
    static void Error(const _T& msg)
    {
        time_t now = time(0);

        tm* t = new tm;
        localtime_s(t, &now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // you can loop k higher to see more color choices
        SetConsoleTextAttribute(hConsole, 12);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }

    static void Succeed(const string& msg)
    {
        time_t now = time(0);

        tm* t = new tm;
        localtime_s(t, &now);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // you can loop k higher to see more color choices
        SetConsoleTextAttribute(hConsole, 10);
        cout << "[" << t->tm_mday << "-" << 1 + t->tm_mon << "-" << 1900 + t->tm_year << " | " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "] " << msg.c_str() << endl;
        SetConsoleTextAttribute(hConsole, 15);
    }
};