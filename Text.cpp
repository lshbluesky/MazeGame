#include <iostream>
#include "Text.hpp"

// 위치를 지정하여 문자열 출력
void gotoxy(int x, int y, const char* s)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}

// 위치를 지정하고 색깔도 추가로 지정하여 해당 컬러로 문자열을 한 글자씩 출력
void gotoxy(int x, int y, const char* s, int Color)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	Speak(s, Color); // 받은 문자열을 받은 색깔로 한 글자씩 자연스럽게 출력 
}

// 흰색으로 한 글자씩 문자열 출력
void Speak(const char speaking[])
{
	int i = 0;
	TextColor(DEEP_WHITE);
	for (; i < str_num(speaking); i++)
	{
		printf("%c", speaking[i]); Sleep(25);
	}
}

// 흰색이 아닌 다른 색을 인수로 직접 전달하여 해당 컬러로 문자열 출력
void Speak(const char speaking[], int C)
{
	int i = 0;
	TextColor(C);
	for (; i < str_num(speaking); i++)
	{
		printf("%c", speaking[i]); Sleep(25);
	}
}

// 텍스트 컬러를 지정해주는 함수
void TextColor(int color_num)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}

// 배열의 문자수를 확인해주는 함수
int str_num(const char* str)
{
	int i = 0;
	while (1)
	{
		if (*(str + i) == NULL)
			break;

		if (*(str + i) != NULL)
			i++;
	}
	return i; // 문자열의 수 출력 
}
