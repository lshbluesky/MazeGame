#include <iostream>
#include "fmod.hpp"
using namespace std;
using namespace FMOD;

//FMOD 라이브러리 및 사운드 출력을 위한 전역 변수 정의
FMOD::System* Fmod(nullptr); // FMOD 라이브러리를 사용하기 위한 Fmod 시스템 클래스를 가리키는 Fmod 포인터 생성
FMOD::Sound* MainBGM(nullptr); // 배경음악을 재생하기 위한 사운드 객체를 가리키는 MainBGM 포인터 생성
FMOD::Channel* channel1(nullptr); // 채널 1에 배경음악을 재생
FMOD_RESULT result; // FMOD 관련 함수가 잘 작동하는지의 여부를 체크하기 위한 변수를 생성
void* extradriverdata(nullptr); // FMOD 라이브러리의 init()에서 사용되는 포인터

// 메인 화면을 그리기 위한 클래스 생성
class StartMenu {
private:
	void DrawStartMenu() {
		cout << "이 부분에 메인 화면을 구성합니다.\n";
	}
public:
	void InitStartMenu() {
		StartMenu::DrawStartMenu();
	}
};

int main(void)
{
	StartMenu MenuControl;
	MenuControl.InitStartMenu();

	result = FMOD::System_Create(&Fmod); // FMOD 라이브러리를 사용하기 위한 FMOD 시스템 객체 생성
	if (result != FMOD_OK) return -1;

	result = Fmod->init(3, FMOD_INIT_NORMAL, extradriverdata); // 채널 개수 3으로 FMOD 라이브러리를 사용하기 위하여 준비 및 초기화
	if (result != FMOD_OK) return -2;

	result = Fmod->createStream(".\\Sounds\\stage3.mp3", FMOD_LOOP_NORMAL, 0, &MainBGM); // 배경음악 사운드 객체 생성, 효과음은 createStream() 대신에 createSound() 함수를 사용하기
	if (result != FMOD_OK) return -3;

	result = Fmod->playSound(MainBGM, 0, false, &channel1); // 배경음악 재생
	if (result != FMOD_OK) return -4;

	while (1)
	{
		result = Fmod->update(); // 사운드를 계속 재생하기 위해서는 While 문을 사용하여 Fmod->update() 가 계속 실행되어야 함.

		if (getchar())
			break;
	}

	Fmod->release(); // 게임이 종료되기 전에 사운드 관련 객체 해제
	return 0;
}
