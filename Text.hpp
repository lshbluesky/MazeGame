#include <iostream>
#include <string>
using namespace std;

// 아래의 gotoxy_C 함수에서 출력할 문자열의 색깔을 지정할 때, 이의 COLOR 라는 열거체에 저장된 값을 인수로 전달하여 사용
enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW, DEEP_WHITE
};

void gotoxy(int x, int y, const char* s);
void gotoxy_C(int x, int y, const char* s, int Color);
void Speak(const char speaking[]);
void Speak_C(const char speaking[], int C);
void TextColor(int color_num);
int str_num(const char* str);
