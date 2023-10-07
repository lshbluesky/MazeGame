/*

*/
#include "Player_move.hpp"
#include "Text.hpp"
#include <windows.h>	
#include <iostream>
#include <conio.h> // getch 함수를 사용하기 위함
using namespace std;

const int MAZE_SIZE = 5; // 샘플 미로 크기. 실제 미로 생성 코드가 완성되었다면 삭제해도 됨, 아래 부분도 마찬가지

char maze[MAZE_SIZE][MAZE_SIZE] = { // 미로가 어떤 구조로 만들어져 있는지 몰라서 아래 코드를 구현하기 위해 가상의 미로를 만든 것. '#' 이 부분이 벽
	{' ', ' ', ' ', '#', ' '},
	{'#', '#', ' ', '#', ' '},
	{' ', ' ', ' ', '#', ' '},
	{'#', ' ', '#', '#', '#'},
	{' ', ' ', ' ', ' ', 'E'}
};

class Move { // 이동 함수를 담당하는 기본 클래스
public:
	virtual bool isMoveBlocked(int x, int y) {
		if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) { // 이동할려는 방향이 미로의 크기에 벗어나지 않는지 확인
			return true; // 벽에 막혔을 때 경고음 소리로 잘못된 키보드 입력이란걸 알림
		}
		if (maze[x][y] == '■') { // 이동할려는 방향에 벽이 있는지 확인
			return true;
		}
		return false; // 기본 전제로 벽이 없다고 가정함
	}

	virtual void move(int& x, int& y) = 0; // 플레이어 이동 함수. 순수 가상 함수로 정의했고, 파생 클래스에서 무조건 동작을 정의해야 함
};

class MoveUp :public Move { // 상속 받은 Up 클래스. 위로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override { // isMoveBlocked 함수 오버라이딩 
		return Move::isMoveBlocked(x, y - 1); // 위로 가니깐 y좌표를 -1 함. 즉 움직일 위치의 좌표값을 넣음
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) { // 미로 크기에 벗어나지 않거나 진행 방향에 벽이 없다면 위로 이동
			y = y - 1; // 위로 가니깐 현재 위치의 y좌표값을 -1
		}
	}
};

class MoveDown :public Move { // 상속 받은 Down 클래스. 아래로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x, y + 1); // 아래로 가니깐 y좌표를 +1 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			y = y + 1;
		}
	}
};

class MoveLeft :public Move { // 상속 받은 Up 클래스. 왼쪽으로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x - 2, y); // 왼쪽으로 가니깐 x좌표를 -2 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x - 2;
		}
	}
};

class MoveRight :public Move { // 상속 받은 Up 클래스. 오른쪽으로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x + 2, y); // 오른쪽으로 가니깐 x좌표를 +2 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x + 2;
		}
	}
};

class Player { // 여기서 키보드 입력값을 받고, 이동 함수들을 호출 할 예정. 
private:
	int x, y;// 플레이어 좌표
	int e_x, e_y;
	Move* currentMove; // 부모 클래스의 포인터로 자식 클래스의 객체를 동적 할당 함. 다형성을 드러냄

public:
	Player(int start_x, int start_y, int end_x, int end_y) : x(start_x), y(start_y), e_x(end_x), e_y(end_y), currentMove(nullptr) {} // 생성자 정의. 좌표값 0으로 초기화(일반적인 시작위치), 포인터 값 null로 지정
	~Player() {                                    // 소멸자 정의
		delete	currentMove;                       // 매 판이 끝나면 플레이어 이동을 담당한 동적 할당된 메모리들을 삭제함
	}

	void handleInput() {
		char direction; // 키보드 값 입력 받을 변수
		direction = _getch(); // 키보드 값 입력 받음

		switch (direction) { 
		case'w':
		case'W':
			currentMove = new MoveUp(); // 포인터 변수에 객체를 동적 할당
			break;
		case's':
		case'S':
			currentMove = new MoveDown();
			break;
		case'a':
		case'A':
			currentMove = new MoveLeft();
			break;
		case'd':
		case'D':
			currentMove = new MoveRight();
			break;
		}

		if (currentMove != nullptr) {
			currentMove->move(x, y); // move 함수 호출. 다형성 사용
		}
	}
	//void GameFinish() { // 탈출 성공
	//	if (x = e_x && y = e_y) { 
	//		// 미로 탈출 성공을 표현하는 GAME OVER 나 CLEAR 같은 문구를 이용한 화면이 출력 + 성공을 알리는 효과음도 들어가면 좋을 거 같음
	//		// 다만 성공 화면이 현재 구현되어 있지 않고 효과음도 없는 상태라 어떻게 코드를 만들어야 할 지 몰라 비워 둠
	//		// 혹시 이 부분 코드 완성 하실 수 있다면 부탁드립니다...
	//	}
	//}
	void print() {					// 화면 갱신
		gotoxy(x, y, "\u2605");// 플레이어의 위치는 ★로 표기
		gotoxy(e_x, e_y, "\u25c8");// 도착점 위치는 ◈로 표기
	}

	int getX() const { return x; } // 외부에서 x값을 가져갈 때. x 값을 건들이지 않도록 const를 사용
	int getY() const { return y; }
};

int start_x, start_y; // 시작 위치 변수 저장용
int end_x, end_y; // 도착점 위치 변수 저장용 

// 아래 함수 내용은 그냥 main에 복붙하는 편이 편할듯, 최소한 Randomstart랑 Player는 해야함
void Playing() {

	Player player(start_x, start_y, end_x, end_y);		// 객체 생성, RandomStart 나중 먼저 상관 없을듯?

	while (true)			// 입력을 받으면 움직이도록
	{
		player.print();
		cursor(0);			// 0 = 깜빡임 제거 / 1 = 깜빡임 생성

		player.handleInput(); // 키보드 입력 받는 함수 호출

		Sleep(5);			// 5ms 간격으로 화면 갱신
		system("cls");		// 화면 갱신
	}
}

void cursor(int n) { // 커서 깜빡임 제거 용도
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


/* 밑에 나올 StartFinishPoint 함수를 설계한 과정 및 논리 설명
* 젤 바깥은 벽으로 둘러져 있는 상태에서, 미로 내부의 각 꼭짓점 4군데는 무조건 벽이 아닌 길로 생성될거라 가정하에 만들어짐(확신은 못하지만 몇 번의 테스트 결과 그렇게 생성됨)
* 그래서 그 4곳의 꼭짓점 중 랜덤으로 한 곳을 출발점이라 지정하면, 자동으로 그 대각선 반대편에 있는 꼭짓점이 도착점이 되도록 만듬
* 그러면 출발점과 도착점이 너무 가까워 게임이 싱거워질 우려를 해결할 수 있을거라 생각되어서 고민 끝에 이 방식을 사용하기로 결정
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*				저희가 많은 시간 동안 고민하고 학습하며 테스트를 돌린 결과, 4곳의 꼭짓점이 막힌 미로가 생성되는 경우는 버그이거나 오류일 것이라 판단하였습니다
*							혹시나 저희의 판단이 틀린 거라면 이에 대한 해결 방안을 다 같이 생각해봐야 될 거 같아요
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* 혹시나 이 부분에 대해 궁금하거나 이해가 가지 않는 부분이 있다면 질문 부탁드립니다.
*/


void StartFinishPoint(int MAZE_SIZE) { // 출발점과 도착점을 생성하는 함수
	srand(static_cast<unsigned>(time(0))); // 난수 생성기 초기화

	// 출발점과 도착점 설정
	int n = MAZE_SIZE - 2; // 미로사이즈가 10이라 가정하면 바깥에 존재하는 벽은 0과 9로 이루어져 있을테니, 움직일 수 있는 미로 내부의 젤 끝은 1과 8이 되므로 -2를 함
	int startX, startY, endX, endY;

	// 출발점 설정
	int randomStart = rand() % 4; // rand 함수가 반환하는 0부터 RAND_MAX의 값 중 4로 나눈 나머지, 즉 0, 1, 2, 3만 나오게 끔 하는 함수
	switch (randomStart) {
	case 0: // (1, 1)을 출발점으로 설정
		startX = 1;
		startY = 1;
		break;
	case 1: // (1, n)을 출발점으로 설정
		startX = 1;
		startY = n;
		break;
	case 2: // (n, 1)을 출발점으로 설정
		startX = n;
		startY = 1;
		break;
	case 3: // (n, n)을 출발점으로 설정
		startX = n;
		startY = n;
		break;

	}
	endX = MAZE_SIZE - 1 - startX; // 이 수식을 계산하면 출발점의 대각선 반대에 있는 꼭짓점의 좌표가 도착점으로 설정 됨
	endY = MAZE_SIZE - 1 - startY;
}


