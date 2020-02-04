///*
//==================================================================================================================================================================
//|	Modified code from:
//|
//| Copyright 2018 Oscar Lara	- scarlara@hotmail.com																												   |
//|																																								   |
//|See individual libraries separate legal notices																												   |
//|																																								   |
//|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
//|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,				   |
//|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :					   |
//|																																								   |
//|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.									   |
//|																																								   |
//|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
//|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
//|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.		   |
//==================================================================================================================================================================
//*/
//
#include "cConsole.h"

void cConsole::init() {
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = FALSE;

	SetConsoleCursorInfo(hOut, &ConCurInf);

	is_init = true;
}

void cConsole::gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hOut, coord);
}

void cConsole::log(int row_number, const char* message, ...) {

	va_list argptr;
	va_start(argptr, message);

	gotoxy(0, row_number);

	vprintf(message, argptr);
	va_end(argptr);

}


