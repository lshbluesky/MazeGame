/*
만든 사람: 신찬희
클래스는 2개 사용 예정(Player, Move)
Player 클래스에서는 위치정보와 추가될 기능(폭탄, 적의 경로)
Move 클래스에서는 이동과 추가될 기능(벽 밀기, 적의 움직임)
Player는 가능하다면 싱글톤도 고려
*/

#include <random>
#include <conio.h>			// _getch() 함수에서 필요
#include <windows.h>		// COORD 등을 사용하는 isWall() 함수에서 필요
#include "Text.hpp"
#include "Plyer_move.hpp"
using namespace std;

class Player
{
private:
	int x = NULL;
	int y = NULL;
	// 현재 위치

	int prev_x;
	int prev_y;
	// 이전 위치
public:
	Player(int start_x, int start_y) : x(start_x), y(start_y), prev_x(start_x), prev_y(start_y) {}
	// 이전·현재위치를 시작위치로 초기화

	void Move(int direction) {		// 입력받아 행동하는 함수
		prev_x = x;
		prev_y = y;
		// 현재 위치를 기록

		switch (direction)			// 입력에 따른 행동 정의
		{
		case UP:
			y--; break;
		case DOWN:
			y++; break;
		case LEFT:
			x--; break;
		case RIGHT:
			x++; break;
		}

		int new_x = x;		// 이동한 위치를 기록
		int new_y = y;

		// 이동 위치가 벽일 경우(isWall()의 반환값이 true일 경우) >>>  이전 위치로 돌아감
		// 이동 위치가 벽이 아닐 경우(isWall()의 반환값이 false일 경우) >>>  해당 위치로 이동	
		if (!isWall(new_x, new_y)) {
			x = new_x;
			y = new_y;
		}
		else {
			x = prev_x;
			y = prev_y;
		}
	}

	// 사실상 플레이어 위치만 표기하면 됨.
	void print() {					// 화면 갱신
		gotoxy(x, y, "★");				// 플레이어의 위치는 ★로 표기
	}

	int get_x() {			// 나중에 x,y 위치 필요할 때 쓰는거
		return x;
	}
	int get_y() {
		return y;
	}
};

int start_x, start_y; // 시작 위치 변수 저장용

// 아래 함수 내용은 그냥 main에 복붙하는 편이 편할듯, 최소한 Randomstart랑 Player는 해야함
void Playing() {
	char input;				// 입력 저장용 변수

	RandomStart(5, 5);		// 미로 범위를 받도록
	Player player(start_x, start_y);		// 객체 생성, RandomStart 나중 먼저 상관 없을듯?

	while (true)			// 입력을 받으면 움직이도록
	{
		player.print();
		cursor(0);			// 0 = 깜빡임 제거 / 1 = 깜빡임 생성

		if (kbhit()) {		// kbhit() <<< 키보드 입력 확인

			input = getch(); // 입력 저장

			switch (input)
			{
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
				player.Move(input); break;
			}
		}
		Sleep(5);			// 5ms 간격으로 화면 갱신
		system("cls");		// 화면 갱신
	}
}

void cursor(int n) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

// 시작 위치를 랜덤하게, 미로의 최대 범위를 받도록
int RandomStart(int max_x, int max_y) {
	random_device rd;
	mt19937  generator(rd());			// mt19937 엔진 사용

	// x, y의 최대 범위 내의 값을 생성하는 distribution
	uniform_int_distribution<int> distribution_x(0, max_x); // x 좌표의 범위
	uniform_int_distribution<int> distribution_y(0, max_y); // y 좌표의 범위

	do {
		start_x = distribution_x(generator); // 무작위 위치에서 시작
		start_y = distribution_y(generator);

	} while (isWall(start_x, start_y)); // 만약 (x, y)가 벽으로 판정되면 다시 위치를 랜덤하게 선택, isWall 함수로 벽 판정을 한다고 가정

	return start_x, start_y;
}

// 해당 위치가 벽인지 확인하는 함수 / 여기서는 벽을 문자 '#'으로 정의
bool isWall(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);		// 콘솔과 상호작용하기 위한 핸들
	CHAR_INFO Pos_buffer;									// 화면 문자를 저장할 변수
	COORD bufferSize = { 1, 1 };							// 1x1 크기의 문자를 읽고 저장
	COORD bufferCoord = { 0, 0 };							// 좌상단부터 문자를 읽음

	// 읽어올 화면 영역을 설정, 현재 위치의 1x1 크기의 영역을 읽음
	SMALL_RECT readRegion = { static_cast<SHORT>(x), static_cast<SHORT>(y), static_cast<SHORT>(x), static_cast<SHORT>(y) };

	// 현재 위치의 화면 문자를 읽고 Pos_buffer에 저장
	ReadConsoleOutput(hConsole, &Pos_buffer, bufferSize, bufferCoord, &readRegion);

	// 읽어온 문자가 '#'(벽) 문자인지 확인하여 반환
	return (Pos_buffer.Char.AsciiChar == '#');
}
