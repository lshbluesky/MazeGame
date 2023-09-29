﻿#include <iostream>
#include <conio.h>
#include "Text.hpp"
#include "fmod.hpp"
#include "Player_move.hpp"

// 키보드 값
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

using namespace std;
using namespace FMOD;

//FMOD 라이브러리 및 사운드 출력을 위한 전역 변수 정의
FMOD::System* Fmod(nullptr); // FMOD 라이브러리를 사용하기 위한 Fmod 시스템 클래스를 가리키는 Fmod 포인터 생성
FMOD::Sound* MainBGM(nullptr); // 배경음악을 재생하기 위한 사운드 객체를 가리키는 MainBGM 포인터 생성
FMOD::Sound* Die(nullptr); // 플레이어 사망 효과음 재생 포인터 생성
FMOD::Sound* Select(nullptr); // 선택 효과음 재생 포인터 생성
FMOD::Sound* Stage_Clear(nullptr); // 스테이지 통과 효과음 재생 포인터 생성
FMOD::Channel* channel1(nullptr); // 채널 1에서 배경음악을 재생
FMOD::Channel* channel2(nullptr); // 채널 2에서 효과음을 재생
FMOD_RESULT result; // FMOD 관련 함수가 잘 작동하는지의 여부를 체크하기 위한 변수를 생성
void* extradriverdata(nullptr); // FMOD 라이브러리의 init()에서 사용되는 포인터

// 메인 화면을 그리기 위한 클래스 생성
class StartMenu {
private:
	int x = 42; // 선택 메뉴를 출력하는 위치의 x 좌표
	int y = 14; // 선택 메뉴를 출력하는 위치의 y 좌표

	void DrawStartMenu() { // 메인 화면 그리기
		TextColor(DEEP_GREEN);
		gotoxy(24, 6, "#####     ###     ###     #     ####    #####");
		gotoxy(24, 7, "#        #       #       # #    #   #   #    ");
		gotoxy(24, 8, "#####    ####    #      #####   ####    ####  ");
		gotoxy(24, 9, "#            #   #      #   #   #       #    ");
		gotoxy(24, 10, "#####    ####     ###   #   #   #       #####");
		TextColor(DEEP_WHITE);
		gotoxy(2, 1, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
		gotoxy(2, 2, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
		gotoxy(2, 27, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
		gotoxy(2, 28, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
		TextColor(DEEP_YELLOW);
		gotoxy(6, 25, "★ 즐거운 BGM과 함께하는 아찔아찔 미로게임~♬");
		TextColor(DEEP_WHITE);
		gotoxy(70, 25, "Made by Object");
		TextColor(DEEP_OC);
		gotoxy(66, 25, "▦");
		gotoxy(86, 25, "▦");
		TextColor(DEEP_JAJU);
		gotoxy(68, 25, "δ");
		gotoxy(84, 25, "δ");
	}

	int keyControl() {
		char temp = getch();

		if (temp == 'w' || temp == 'W') {
			return UP;
		}
		else if (temp == 'a' || temp == 'A') {
			return LEFT;
		}
		else if (temp == 's' || temp == 'S') {
			return DOWN;
		}
		else if (temp == 'd' || temp == 'D') {
			return RIGHT;
		}
		else if (temp == ' ') { // 스페이스바(공백)가 선택 버튼
			return SUBMIT;
		}
	}

public:
	int InitStartMenu() {
		StartMenu::DrawStartMenu(); // 캡슐화, 정보은닉
		StartMenu::DrawMenu();
		return StartMenu::DrawMenuNum();
	}

	void DrawMenu() { // gotoxy 함수를 사용한 메뉴 위치 생성
		x = 42;
		y = 14;
		gotoxy(x - 3, y, "▶ 게 임 시 작", DEEP_WHITE);
		gotoxy(x, y + 1, "게 임 정 보", DEEP_WHITE);
		gotoxy(x, y + 2, "   종 료   ", DEEP_WHITE);
		Fmod->createSound(".\\Sounds\\Select.mp3", FMOD_LOOP_OFF, 0, &Select); // 선택 효과음 객체 생성
	}

	int DrawMenuNum() { // 반환되는 값만 계산하기 위하여 DrawMenu 함수에서 while 구문만 별도로 분리하여 DrawMenuNum 함수를 생성
		while (1) {
			int n = keyControl(); //  키보드 이벤트를 키값으로 받아오기
			switch (n) {
			case UP: { // 입력된 키의 값이 UP 인 경우 (w)
				if (y > 14) { // y는 14~16 까지만 이동
					gotoxy(x - 3, y, "  "); // x-2 하는 이유는 "▶"를 두 칸 이전에 출력하기 위해 / "  " : 원래 위치 지움
					gotoxy(x - 3, --y, "▶"); // 새로 이동한 위치로 이동 / "▶" 다시 그리기
				}
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				break;
			}
			case DOWN: { // 입력된 키의 값이 UP 인 경우 (s)
				if (y < 16) { // 최대 16
					gotoxy(x - 3, y, "  ");
					gotoxy(x - 3, ++y, "▶");
				}
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				break;
			}
			case SUBMIT: {
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				return y - 14;
			}
			}
		}
	}
};

void DrawGameInfo() // 게임 정보 화면을 출력하는 함수
{
	TextColor(DEEP_WHITE); // 메인 화면의 테두리와 디자인 요소는 그대로 출력
	gotoxy(2, 1, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	gotoxy(2, 2, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 27, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 28, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	TextColor(DEEP_YELLOW);
	gotoxy(6, 25, "★ 즐거운 BGM과 함께하는 아찔아찔 미로게임~♬");
	TextColor(DEEP_WHITE);
	gotoxy(70, 25, "Made by Object");
	TextColor(DEEP_OC);
	gotoxy(66, 25, "▦");
	gotoxy(86, 25, "▦");
	TextColor(DEEP_JAJU);
	gotoxy(68, 25, "δ");
	gotoxy(84, 25, "δ");

	TextColor(DEEP_YELLOW); // 이 부분부터 게임 정보 내용을 입력
	gotoxy(6, 5, "※ 게임 정보 ※");
	TextColor(DEEP_WHITE);
	gotoxy(6, 7, "DFS 알고리즘을 활용하여 생성된 미로를 지정된 장소로 이동하여 탈출하는 게임입니다.");
	TextColor(DEEP_OC);
	gotoxy(6, 10, "▦ 스테이지 및 난이도 구성");
	TextColor(DEEP_YELLOW);
	gotoxy(8, 11, "▦ 유아용 스테이지");
	gotoxy(8, 14, "▦ 일반 플레이어용 스테이지");
	gotoxy(8, 17, "▦ 노인분들을 위한 스테이지");
	TextColor(DEEP_WHITE);
	gotoxy(10, 12, "- Level 1");
	gotoxy(10, 13, "- Level 2");
	gotoxy(10, 15, "- Level 1");
	gotoxy(10, 16, "- Level 2");
	gotoxy(10, 18, "- Level 1");
	gotoxy(10, 19, "- Level 2");

	TextColor(DEEP_OC); //조작키 설명 입력
	gotoxy(52, 10, "▦ 조작키 설명");
	TextColor(DEEP_WHITE);
	gotoxy(54, 12, "▦ w : 위");
	gotoxy(54, 13, "▦ s : 아래");
	gotoxy(54, 14, "▦ a : 왼쪽");
	gotoxy(54, 15, "▦ d : 오른쪽");

	gotoxy(6, 22, "※ 아무키나 누르면 메인 화면으로 돌아갑니다...", DEEP_WHITE);
	system("pause>null"); // 게임 정보 화면을 출력하고 아무 키를 입력받을 때까지 일시정지, 아무 키나 누르면 다시 메인 화면으로 돌아감.
	Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
	system("cls");
}

int main(void)
{
	FMOD::System_Create(&Fmod); // FMOD 라이브러리를 사용하기 위한 FMOD 시스템 객체 생성
	Fmod->init(3, FMOD_INIT_NORMAL, extradriverdata); // 채널 개수 3으로 FMOD 라이브러리를 사용하기 위하여 준비 및 초기화
	Fmod->createStream(".\\Sounds\\Menu_Main.mp3", FMOD_LOOP_NORMAL, 0, &MainBGM); // 배경음악 사운드 객체 생성, 효과음은 createStream() 대신에 createSound() 함수를 사용하기
	Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 재생
	Fmod->update();

	system("mode con:cols=110 lines=34");
	StartMenu MenuControl;

	while (1)
	{
		switch (MenuControl.InitStartMenu()) // 메인 화면에서 선택한 메뉴에 해당하는 값으로 메뉴를 판단 (DrawMenu 함수의 return y - 14; 구문에서 반환되는 값)
		{
		case 0: // 게임 시작을 선택한 경우
			break;
		case 1: // 게임 정보를 선택한 경우
			system("cls");
			DrawGameInfo();
			break;
		case 2: // 게임 종료를 선택한 경우
			return 0;
		}
	}

	Fmod->release(); // 게임이 종료되기 전에 사운드 관련 객체 해제
	return 0;
}
