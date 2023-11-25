#include <iostream>
#include <conio.h>
#include <chrono>
#include "Text.hpp"
#include "fmod.hpp"
#include "Player_move.hpp"
#include "ranking.hpp"

// 키보드 값
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4

using namespace std;
using namespace FMOD;

// FMOD 라이브러리 및 사운드 출력을 위한 전역 변수 정의
FMOD::System* Fmod(nullptr); // FMOD 라이브러리를 사용하기 위한 Fmod 시스템 클래스를 가리키는 Fmod 포인터 생성
FMOD::Sound* MainBGM(nullptr); // 배경음악을 재생하기 위한 사운드 객체를 가리키는 MainBGM 포인터 생성
FMOD::Sound* StageBGM(nullptr); // 스테이지 선택 메뉴 음악을 재생하기 위한 사운드 객체를 가리키는 StageBGM 포인터 생성
FMOD::Sound* Stg1Lv1(nullptr); // 유아용 스테이지 Level 1 BGM 포인터 생성
FMOD::Sound* Stg1Lv2(nullptr); // 유아용 스테이지 Level 2 BGM 포인터 생성
FMOD::Sound* Stg2Lv1(nullptr); // 일반 플레이어용 스테이지 Level 1 BGM 포인터 생성
FMOD::Sound* Stg2Lv2(nullptr); // 일반 플레이어용 스테이지 Level 2 BGM 포인터 생성
FMOD::Sound* Stg3Lv1(nullptr); // 노인분들을 위한 스테이지 Level 1 BGM 포인터 생성
FMOD::Sound* Stg3Lv2(nullptr); // 노인분들을 위한 스테이지 Level 2 BGM 포인터 생성
FMOD::Sound* Die(nullptr); // 플레이어 사망 효과음 재생 포인터 생성
FMOD::Sound* Select(nullptr); // 선택 효과음 재생 포인터 생성
FMOD::Sound* Stage_Clear(nullptr); // 스테이지 통과 효과음 재생 포인터 생성
FMOD::Channel* channel1(nullptr); // 채널 1에서 배경음악을 재생
FMOD::Channel* channel2(nullptr); // 채널 2에서 효과음을 재생
FMOD_RESULT result; // FMOD 관련 함수가 잘 작동하는지의 여부를 체크하기 위한 변수를 생성
void* extradriverdata(nullptr); // FMOD 라이브러리의 init()에서 사용되는 포인터

// 플레이 시간 측정을 위한 전역 변수를 외부 참조
extern chrono::steady_clock::time_point startTime;
extern chrono::steady_clock::time_point endTime;
extern chrono::duration<double> elapsed_seconds;

// 유니버셜 디자인 - 글꼴 크기 조절을 위한 구조체 전역 변수 정의
CONSOLE_FONT_INFOEX fontInfo;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void fontSize(unsigned short n) { // 글꼴 크기를 매개 변수로 받은 값으로 조절하는 함수
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = n / 2; // 가로 글꼴 크기
	fontInfo.dwFontSize.Y = n; // 세로 글꼴 크기 (콘솔의 특성상 가로:세로 = 1:2 비율로 지정해야 함)
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo); // SetCurrentConsoleFontEx 함수를 호출하여 글꼴 크기 적용
}

// 메인 화면을 그리기 위한 클래스 생성
class StartMenu {
private:
	int x = 42; // 선택 메뉴를 출력하는 위치의 x 좌표
	int y = 14; // 선택 메뉴를 출력하는 위치의 y 좌표

	void DrawStartMenu() { // 메인 화면 그리기
		TextColor(DEEP_GREEN);
		gotoxy(24, 6,  "#####    ###     ####     #     ####    #####");
		gotoxy(24, 7,  "#       #       #        # #    #   #   #    ");
		gotoxy(24, 8,  "#####   ####    #       #####   ####    ####  ");
		gotoxy(24, 9,  "#           #   #       #   #   #       #    ");
		gotoxy(24, 10, "#####   ####     ####   #   #   #       #####");
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
		gotoxy(x, y + 2, "랭 킹 보 기", DEEP_WHITE);
		gotoxy(x, y + 3, "   종 료   ", DEEP_WHITE);
		Fmod->createSound(".\\Sounds\\Select.mp3", FMOD_LOOP_OFF, 0, &Select); // 선택 효과음 객체 생성
	}

	int DrawMenuNum() { // 반환되는 값만 계산하기 위하여 DrawMenu 함수에서 while 구문만 별도로 분리하여 DrawMenuNum 함수를 생성
		while (1) {
			int n = keyControl(); //  키보드 이벤트를 키값으로 받아오기
			switch (n) {
			case UP: { // 입력된 키의 값이 UP 인 경우 (w)
				if (y > 14) { // y는 14~17 까지만 이동
					gotoxy(x - 3, y, "  "); // x-2 하는 이유는 "▶"를 두 칸 이전에 출력하기 위해 / "  " : 원래 위치 지움
					gotoxy(x - 3, --y, "▶"); // 새로 이동한 위치로 이동 / "▶" 다시 그리기
				}
				Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
				break;
			}
			case DOWN: { // 입력된 키의 값이 UP 인 경우 (s)
				if (y < 17) { // 최대 17
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

	TextColor(DEEP_OC); // 아이콘 설명 입력
	gotoxy(40, 10, "▦ 아이콘 설명");
	TextColor(DEEP_WHITE);
	gotoxy(42, 12, "★ : 플레이어");
	gotoxy(42, 13, "◈ : 도착 지점");
	gotoxy(42, 14, "♠ : 폭탄(밟으면 원위치로)");
	gotoxy(42, 15, "■ : 벽");

	TextColor(DEEP_OC); // 조작키 설명 입력
	gotoxy(71, 10, "▦ 조작키 설명");
	TextColor(DEEP_WHITE);
	gotoxy(73, 12, "▦ w : 위");
	gotoxy(73, 13, "▦ s : 아래");
	gotoxy(73, 14, "▦ a : 왼쪽");
	gotoxy(73, 15, "▦ d : 오른쪽");

	gotoxy(6, 22, "※ 아무키나 누르면 메인 화면으로 돌아갑니다...", DEEP_WHITE);
	system("pause>null"); // 게임 정보 화면을 출력하고 아무 키를 입력받을 때까지 일시정지, 아무 키나 누르면 다시 메인 화면으로 돌아감.
	Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
	system("cls");
}

void DrawRanking() // 플레이어 별 플레이 시간의 순위 랭킹 화면을 출력하는 함수
{
	TextColor(DEEP_WHITE); // 메인 화면의 테두리와 디자인 요소는 그대로 출력
	gotoxy(2, 1, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	gotoxy(2, 2, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 27, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 28, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	TextColor(DEEP_WHITE);
	gotoxy(70, 25, "Made by Object");
	TextColor(DEEP_OC);
	gotoxy(66, 25, "▦");
	gotoxy(86, 25, "▦");
	TextColor(DEEP_JAJU);
	gotoxy(68, 25, "δ");
	gotoxy(84, 25, "δ");

	TextColor(DEEP_YELLOW); // 이 부분부터 플레이어 별 순위 랭킹 정보 입력
	gotoxy(6, 4, "※ 플레이어 별 순위 랭킹 정보 ※");
	TextColor(DEEP_WHITE);
	showRanking(); // 플레이어와 플레이 시간의 순위 랭킹을 출력하는 함수 호출

	gotoxy(45, 4, "▦ 아무키나 누르면 메인 화면으로 돌아갑니다...", DEEP_WHITE);
	system("pause>null"); // 순위 랭킹 화면을 출력하고 아무 키를 입력받을 때까지 일시정지, 아무 키나 누르면 다시 메인 화면으로 돌아감.
	Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
	system("cls");
}

int DrawStageMenu() { // 플레이어 연령 별 스테이지를 선택하는 화면을 출력하는 함수
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

	TextColor(DEEP_OC); // 이 부분부터 미로 게임의 스테이지 메뉴 정보를 입력
	gotoxy(6, 5, "※ 스테이지 메뉴 정보 및 선택 ※");
	TextColor(DEEP_YELLOW);
	gotoxy(6, 8, "▦ 유아용 스테이지");
	gotoxy(31, 8, "▦ 일반 플레이어용 스테이지");
	gotoxy(62, 8, "▦ 노인분들을 위한 스테이지");
	TextColor(DEEP_WHITE);
	gotoxy(62, 5, "9. 처음 메인 화면으로 이동");
	gotoxy(6, 10, "1. Level 1");
	gotoxy(6, 11, "2. Level 2");
	gotoxy(31, 10, "3. Level 1");
	gotoxy(31, 11, "4. Level 2");
	gotoxy(62, 10, "5. Level 1");
	gotoxy(62, 11, "6. Level 2");

	unsigned int menu; // 스테이지 메뉴 번호를 입력받아서 저장할 menu 변수를 선언.
	gotoxy(6, 21, "▦ 플레이하려는 스테이지 번호 입력 : ", DEEP_OC);
	cin >> menu; // 스테이지 메뉴 번호를 입력받아서 menu 변수에 저장.
	TextColor(DEEP_WHITE);
	Fmod->playSound(Select, 0, false, &channel2); // 선택 효과음 재생
	Fmod->update();
	return menu; // 입력받은 스테이지 메뉴 번호를 반환. (StageMenu 함수의 switch 문에서 이의 반환값이 사용됨.)
}

int BombCount;	  // 폭탄 개수 저장 변수
int StageMenu() { // 스테이지 선택 메뉴 화면을 담당하는 함수
	Fmod->createStream(".\\Sounds\\Menu_SelectStage.ogg", FMOD_LOOP_NORMAL, 0, &StageBGM); // 스테이지 선택 메뉴 배경음악 사운드 객체 생성.
	Fmod->playSound(StageBGM, 0, false, &channel1); // 스테이지 선택 메뉴 배경음악 재생
	Fmod->update();
	while (1)
	{
		Fmod->update();
		switch (DrawStageMenu())
		{
		case 1: // 유아용 스테이지의 Level 1을 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg1Lv1.mp3", FMOD_LOOP_NORMAL, 0, &Stg1Lv1);
			Fmod->playSound(Stg1Lv1, 0, false, &channel1); // 유아용 스테이지 Level 1 BGM 재생
			system("cls");
			BombCount = 0;
			Playing(13);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 2: // 유아용 스테이지의 Level 2를 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg1Lv2.ogg", FMOD_LOOP_NORMAL, 0, &Stg1Lv2);
			Fmod->playSound(Stg1Lv2, 0, false, &channel1); // 유아용 스테이지 Level 2 BGM 재생
			system("cls");
			BombCount = 2;
			Playing(15);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 3: // 일반 플레이어용 스테이지의 Level 1을 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg2Lv1.mp3", FMOD_LOOP_NORMAL, 0, &Stg2Lv1);
			Fmod->playSound(Stg2Lv1, 0, false, &channel1); // 일반 플레이어용 스테이지 Level 1 BGM 재생
			system("cls");
			BombCount = 4;
			Playing(17);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 4: // 일반 플레이어용 스테이지의 Level 2를 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg2Lv2.mp3", FMOD_LOOP_NORMAL, 0, &Stg2Lv2);
			Fmod->playSound(Stg2Lv2, 0, false, &channel1); // 일반 플레이어용 스테이지 Level 2 BGM 재생
			system("cls");
			BombCount = 7;
			Playing(19);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 5: // 노인분들을 위한 스테이지의 Level 1을 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg3Lv1.mp3", FMOD_LOOP_NORMAL, 0, &Stg3Lv1);
			Fmod->playSound(Stg3Lv1, 0, false, &channel1); // 노인분들을 위한 스테이지 Level 1 BGM 재생
			system("cls");
			system("mode con:cols=48 lines=16");
			fontSize(38); // 글꼴 크기를 크게 조절(유니버셜 디자인)
			Playing(15);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 6: // 노인분들을 위한 스테이지의 Level 2를 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 스테이지 선택 메뉴 배경음악 재생 정지
			Fmod->createStream(".\\Sounds\\Stg3Lv2.ogg", FMOD_LOOP_NORMAL, 0, &Stg3Lv2);
			Fmod->playSound(Stg3Lv2, 0, false, &channel1); // 노인분들을 위한 스테이지 Level 2 BGM 재생
			system("cls");
			system("mode con:cols=50 lines=18");
			fontSize(38); // 글꼴 크기를 크게 조절(유니버셜 디자인)
			BombCount = 1;
			Playing(17);
			Fmod->playSound(StageBGM, 0, false, &channel1); // 게임을 클리어한 후에, 스테이지 선택 메뉴 배경음악 다시 재생
			break;
		case 9: // 메인 화면으로 돌아가는 메뉴를 선택한 경우
			Fmod->playSound(StageBGM, 0, true, &channel1); // 메인 화면으로 돌아가므로 스테이지 선택 메뉴 배경음악 재생 정지
			system("cls");
			return 9;
		default: // 올바르지 않은 스테이지 메뉴 번호를 입력한 경우
			cin.clear(); // cin 오류 플래그를 초기화
			cin.ignore(LLONG_MAX, '\n'); // 충분히 큰 수(LLONG_MAX) 만큼 입력 버퍼를 비워서 오류 문구가 반복 출력되는 문제를 해결
			gotoxy(6, 23, "※ 잘못된 스테이지 번호입니다. ", DEEP_RED);
			system("pause");
			system("cls");
			continue;
		}
	}
}

void DrawClear() { // 미로 탈출 성공하면 게임 클리어 화면을 그리는 함수
	endTime = chrono::steady_clock::now(); // 플레이 시간 측정 종료
	elapsed_seconds = endTime - startTime;
	Fmod->update();
	Fmod->playSound(Stg1Lv1, 0, true, &channel1); // 게임을 플레이하는 동안 재생하였던 스테이지 BGM 재생 정지
	Fmod->playSound(Stg1Lv2, 0, true, &channel1);
	Fmod->playSound(Stg2Lv1, 0, true, &channel1);
	Fmod->playSound(Stg2Lv2, 0, true, &channel1);
	Fmod->playSound(Stg3Lv1, 0, true, &channel1);
	Fmod->playSound(Stg3Lv2, 0, true, &channel1);
	Fmod->update();
	Fmod->createSound(".\\Sounds\\Stage_Clear.mp3", FMOD_LOOP_OFF, 0, &Stage_Clear); // 스테이지 클리어 효과음 객체 생성
	Fmod->playSound(Stage_Clear, 0, false, &channel2); // 스테이지 클리어 효과음 재생
	Fmod->update();

	fontSize(16); // 원래의 글꼴 크기로 복원
	system("mode con:cols=94 lines=30"); // 원래의 콘솔 창 크기로 복원
	TextColor(DEEP_WHITE); // 메인 화면의 테두리와 디자인 요소는 그대로 출력
	gotoxy(2, 1, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	gotoxy(2, 2, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 27, "□  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □  □");
	gotoxy(2, 28, "■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■");
	TextColor(DEEP_YELLOW);
	gotoxy(24, 5, "★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆");
	gotoxy(24, 13, "★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆");
	gotoxy(6, 25, "★ 즐거운 BGM과 함께하는 아찔아찔 미로게임~♬");
	TextColor(DEEP_WHITE);
	gotoxy(70, 25, "Made by Object");
	TextColor(DEEP_OC);
	gotoxy(66, 25, "▦");
	gotoxy(86, 25, "▦");
	TextColor(DEEP_JAJU);
	gotoxy(68, 25, "δ");
	gotoxy(84, 25, "δ");

	TextColor(DEEP_GREEN);
	gotoxy(24, 7,  "   ####    #       #####      #      ####   ");
	gotoxy(24, 8,  "  #        #       #         # #     #   #  ");
	gotoxy(24, 9,  "  #        #       #####    #####    ####   ");
	gotoxy(24, 10, "  #        #       #        #   #    #   #  ");
	gotoxy(24, 11, "   ####    #####   #####    #   #    #   #  ");
	gotoxy(20, 17, "▦ 축하합니다~! 미로 탈출에 성공하셨습니다! ♩♪♬", DEEP_OC);
	gotoxy(20, 20, "▦ 플레이어의 닉네임 입력 : ", DEEP_WHITE);
	//gotoxy(20, 20, "▦ 아무키나 누르면 스테이지 선택 화면으로 돌아갑니다.", DEEP_WHITE);
	//system("pause>null");
	cin.clear(); // cin 오류 플래그를 초기화
	cin.ignore(LLONG_MAX, '\n'); // 충분히 큰 수(LLONG_MAX) 만큼 입력 버퍼를 비워서 플레이어 닉네임을 입력받지 못 하는 문제를 해결
	PlayTime(); // 게임 클리어 시 PlayTime 함수를 호출하여 플레이 시간을 파일에 기록 및 닉네임 입력
}

int main(void)
{
	FMOD::System_Create(&Fmod); // FMOD 라이브러리를 사용하기 위한 FMOD 시스템 객체 생성
	Fmod->init(3, FMOD_INIT_NORMAL, extradriverdata); // 채널 개수 3으로 FMOD 라이브러리를 사용하기 위하여 준비 및 초기화
	Fmod->createStream(".\\Sounds\\Menu_Main.mp3", FMOD_LOOP_NORMAL, 0, &MainBGM); // 배경음악 사운드 객체 생성, 효과음은 createStream() 대신에 createSound() 함수를 사용하기
	Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 재생
	Fmod->update();

	system("mode con:cols=94 lines=30");
	StartMenu MenuControl;

	while (1)
	{
		Fmod->update();
		switch (MenuControl.InitStartMenu()) // 메인 화면에서 선택한 메뉴에 해당하는 값으로 메뉴를 판단 (DrawMenu 함수의 return y - 14; 구문에서 반환되는 값)
		{
		case 0: // 게임 시작을 선택한 경우
			Fmod->playSound(MainBGM, 0, true, &channel1); // 스테이지 선택 메뉴로 이동하므로 기존에 재생하던 배경음악 재생 정지
			Fmod->update();
			system("cls");
			if (StageMenu() == 9) // 스테이지 선택 메뉴에서 메인 화면으로 돌아가는 메뉴를 선택한 경우 (반환값이 9인 경우)
			{
				Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 다시 재생
				Fmod->update();
				break;
			}
		case 1: // 게임 정보를 선택한 경우
			system("cls");
			DrawGameInfo();
			break;
		case 2: // 게임 정보를 선택한 경우
			system("cls");
			DrawRanking();
			break;
		case 3: // 게임 종료를 선택한 경우
			return 0;
		}
	}

	Fmod->release(); // 게임이 종료되기 전에 사운드 관련 객체 해제
	return 0;
}
