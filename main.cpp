#include <iostream>
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
	void DrawStartMenu() { // 화면 그리기
		TextColor(DEEP_GREEN);
		cout << "\n\n\n\n\n\n";
		cout << "			#####     ###     ###     #     ####    ##### \n";
		cout << "			#        #       #       # #    #   #   #     \n";
		cout << "			#####    ####    #      #####   ####    ####  \n";
		cout << "			#            #   #      #   #   #       #     \n";
		cout << "			#####    ####     ###   #   #   #       ##### \n";
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
	void InitStartMenu() {
		StartMenu::DrawStartMenu(); // 캡슐화, 정보은닉
	}

	int DrawMenu() { // gotoxy함수를 사용한 메뉴 위치 생성
		int x = 42;
		int y = 14;
		gotoxy(x - 3, y, "▶ 게 임 시 작", DEEP_WHITE);
		gotoxy(x, y + 1, "게 임 정 보", DEEP_WHITE);
		gotoxy(x, y + 2, "   종 료   ", DEEP_WHITE);
		Fmod->createSound(".\\Sounds\\Select.mp3", FMOD_LOOP_OFF, 0, &Select); // 선택 효과음 객체 생성
		
		while (1) {
			int n = keyControl(); //  키보드 이벤트를 키값으로 받아오기
			switch (n) {
			case UP: { // 입력된 키의 값이 UP인 경우 (w)
				if (y > 14) { // y는 14~16까지만 이동
					gotoxy(x - 3, y, "  "); // x-2하는 이유는 "▶"를 두 칸 이전에 출력하기 위해 / "  " : 원래 위치 지움
					gotoxy(x - 3, --y, "▶"); // 새로 이동한 위치로 이동 / ">" 다시 그리기
				}
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				break;
			}
			case DOWN: { // 입력된 키의 값이 UP인 경우 (s)
				if (y < 16) { // 최대 16
					gotoxy(x - 3, y, "  ");
					gotoxy(x - 3, ++y, "▶");
				}
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				break;
			}
			case SUBMIT: {
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				return y - 12;
			}
			}
		}
	}
};

int main(void)
{
	FMOD::System_Create(&Fmod); // FMOD 라이브러리를 사용하기 위한 FMOD 시스템 객체 생성
	Fmod->init(3, FMOD_INIT_NORMAL, extradriverdata); // 채널 개수 3으로 FMOD 라이브러리를 사용하기 위하여 준비 및 초기화
	Fmod->createStream(".\\Sounds\\Menu_Main.mp3", FMOD_LOOP_NORMAL, 0, &MainBGM); // 배경음악 사운드 객체 생성, 효과음은 createStream() 대신에 createSound() 함수를 사용하기
	Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 재생
	Fmod->update();

	system("mode con:cols=110 lines=34");
	StartMenu MenuControl;
	MenuControl.InitStartMenu();
	MenuControl.DrawMenu();
	
	Fmod->release(); // 게임이 종료되기 전에 사운드 관련 객체 해제
	return 0;
}
