#include <iostream>
#include <conio.h>
#include "Text.hpp"
#include "fmod.hpp"
#include "Plyer_move.hpp"

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
FMOD::Sound* die(nullptr); // 플레이어 사망 효과음 재생 포인터 생성
FMOD::Sound* stg_clear(nullptr); // 스테이지 통과 효과음 재생 포인터 생성
FMOD::Channel* channel1(nullptr); // 채널 1에서 배경음악을 재생
FMOD::Channel* channel2(nullptr); // 채널 2에서 효과음을 재생
FMOD_RESULT result; // FMOD 관련 함수가 잘 작동하는지의 여부를 체크하기 위한 변수를 생성
void* extradriverdata(nullptr); // FMOD 라이브러리의 init()에서 사용되는 포인터

// 메인 화면을 그리기 위한 클래스 생성
class StartMenu {
private:
	void DrawStartMenu() { // 화면 그리기
		cout << "\n\n\n\n";
		cout << "         #####     ###     ###     #     ####    ##### \n";
		cout << "         #        #       #       # #    #   #   #     \n";
		cout << "         #####    ####    #      #####   ####    ####  \n";
		cout << "         #            #   #      #   #   #       #     \n";
		cout << "         #####    ####     ###   #   #   #       ##### \n";
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
	void InitStartMenu() {
		StartMenu::DrawStartMenu(); // 캡슐화, 정보은닉
	}

	int DrawMenu() { // gotoxy함수를 사용한 메뉴 위치 생성
		int x = 24;
		int y = 12;
		gotoxy(x - 2, y, "> 게 임 시 작");
		gotoxy(x, y + 1, "게 임 정 보");
		gotoxy(x, y + 2, "   종료   ");

		while (1) {
			int n = keyControl(); //  키보드 이벤트를 키값으로 받아오기
			switch (n) {
			case UP: { // 입력된 키의 값이 UP인 경우 (w)
				if (y > 12) { // y는 12~14까지만 이동
					gotoxy(x - 2, y, " "); // x-2하는 이유는 ">"를 두 칸 이전에 출력하기 위해 / " " : 원래 위치 지움
					gotoxy(x - 2, --y, ">"); // 새로 이동한 위치로 이동 / ">" 다시 그리기
				}
				break;
			}
			case DOWN: { // 입력된 키의 값이 UP인 경우 (s)
				if (y < 14) { // 최대 14
					gotoxy(x - 2, y, " ");
					gotoxy(x - 2, ++y, ">");
				}
				break;
			}
			case SUBMIT: {
				return y - 12;
			}
			}
		}

	}
};

int main(void)
{
	system("mode con:cols=120 lines=30");
	StartMenu MenuControl;
	MenuControl.InitStartMenu();
	MenuControl.DrawMenu();

	FMOD::System_Create(&Fmod); // FMOD 라이브러리를 사용하기 위한 FMOD 시스템 객체 생성
	Fmod->init(3, FMOD_INIT_NORMAL, extradriverdata); // 채널 개수 3으로 FMOD 라이브러리를 사용하기 위하여 준비 및 초기화
	Fmod->createStream(".\\Sounds\\title.mp3", FMOD_LOOP_NORMAL, 0, &MainBGM); // 배경음악 사운드 객체 생성, 효과음은 createStream() 대신에 createSound() 함수를 사용하기
	Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 재생

	gotoxy(10, 5, "사운드 재생 테스트...\n", DEEP_OC);
	TextColor(DEEP_YELLOW);
	gotoxy(10, 6, "c키 : 스테이지 통과 효과음 재생");
	gotoxy(10, 7, "d키 : 플레이어 사망 효과음 재생");
	gotoxy(10, 8, "e키 : 종료.");
	Fmod->update();

	while (1)
	{
		Fmod->update(); // 사운드를 계속 재생하기 위해서는 While 문을 사용하여 Fmod->update() 가 계속 실행되어야 함.
		switch (_getch())
		{
		case 'c':
			Fmod->update();
			Fmod->createSound(".\\Sounds\\clear.mp3", FMOD_LOOP_OFF, 0, &stg_clear); // 스테이지 통과 효과음 객체 생성
			Fmod->playSound(stg_clear, 0, false, &channel2); // 스테이지 통과 효과음 재생
			break;
		case 'd':
			Fmod->update();
			Fmod->createSound(".\\Sounds\\die.mp3", FMOD_LOOP_OFF, 0, &die); // 플레이어 사망 효과음 객체 생성
			Fmod->playSound(die, 0, false, &channel2); // 플레이어 사망 효과음 재생
			break;
		case 'e':
			return 0;
		}
	}

	Fmod->release(); // 게임이 종료되기 전에 사운드 관련 객체 해제
	return 0;
}
