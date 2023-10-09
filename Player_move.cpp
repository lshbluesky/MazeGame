#include <windows.h>
#include <iostream>
#include <conio.h> // _getch 함수를 사용하기 위함
#include <vector>
#include "Text.hpp"
#include "fmod.hpp"
#include "Player_move.hpp"
#include "a_maze_map.hpp"
using namespace std;

extern const int N; // a_maze_map.cpp 에서 참조한 미로 크기 
extern vector<vector<int>> maze; // a_maze_map.cpp 에서 참조한 미로 선언
const int MAZE_SIZE = N; // 미로 크기

class Move { // 이동 함수를 담당하는 기본 클래스
public:
	virtual bool isMoveBlocked(int x, int y) {

		if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) { // 이동할려는 방향이 미로의 크기에 벗어나지 않는지 확인
			return true; // 벽에 막혔을 때 경고음 소리로 잘못된 키보드 입력이란걸 알림
		}

		if (maze[x][y] == 1) { // 이동할려는 방향에 벽이 있는지 확인
			return true;
		}
		return false; // 기본 전제로 벽이 없다고 가정함
	}

	virtual void move(int& x, int& y) = 0; // 플레이어 이동 함수. 순수 가상 함수로 정의했고, 파생 클래스에서 무조건 동작을 정의해야 함
};

class MoveUp :public Move { // 상속 받은 Up 클래스. 위로 움직이는 기능 구현
public:
	void move(int& x, int& y) override {
		if (!Move::isMoveBlocked(x, y - 1)) { // 미로 크기에 벗어나지 않거나 진행 방향에 벽이 없다면 위로 이동
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
		return Move::isMoveBlocked(x - 1, y); // 왼쪽으로 가니깐 x좌표를 -1 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x - 1;
		}
	}
};

class MoveRight :public Move { // 상속 받은 Up 클래스. 오른쪽으로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x + 1, y); // 오른쪽으로 가니깐 x좌표를 +1 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x + 1;
		}
	}
};

class Player { // 여기서 키보드 입력값을 받고, 이동 함수들을 호출 할 예정. 
private:
	int x, y;// 플레이어 좌표
	int e_x, e_y; // 도착점 좌표 저장용
	Move* currentMove; // 부모 클래스의 포인터로 자식 클래스의 객체를 동적 할당 함. 다형성을 드러냄

public:
	Player(int start_x, int start_y, int end_x, int end_y) : x(start_x), y(start_y), e_x(end_x), e_y(end_y), currentMove(nullptr) {} // 생성자 정의.포인터 값 null로 지정
	~Player() {                // 소멸자 정의
		delete	currentMove;   // 매 판이 끝나면 플레이어 이동을 담당한 동적 할당된 메모리들을 삭제함
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

	int getX() const { return x; } // 외부에서 x값을 가져갈 때. x 값을 건들이지 않도록 const를 사용
	int getY() const { return y; }
};

int start_x, start_y; // 시작 위치 변수 저장용
int end_x, end_y; // 도착점 위치 변수 저장용 

// 스테이지 난이도 선택 후 본격적으로 게임 실행하는 함수
void Playing() {
	srand(static_cast<unsigned>(time(0))); // 난수 생성기 초기화
	StartMaze(); // 미로를 그리기 위한 준비 및 미로 생성 함수 호출
	StartFinishPoint(MAZE_SIZE); // 출발점과 도착점을 생성하는 함수 호출

	Player player(start_x, start_y, end_x, end_y); // 플레이어 객체 생성
	
	while (true)			// 입력을 받으면 움직이도록 while 문을 계속 수행
	{
		printMaze(player.getX(), player.getY(), end_x, end_y); // 미로를 그리는 함수 호출
		cursor(0);			// 0 = 깜빡임 제거 / 1 = 깜빡임 생성

		player.handleInput(); // 키보드 입력 받는 함수 호출

		Sleep(5);			// 5ms 간격으로 화면 갱신
		system("cls");		// 화면 갱신

		if (player.getX() == end_x && player.getY() == end_y) { // 플레이어가 도착 지점에 도착한 경우, 게임 클리어!
			DrawClear();
			system("cls");
			break;
		}	
	}
}

void StartFinishPoint(int MAZE_SIZE) { // 출발점과 도착점을 생성하는 함수

	// 출발점과 도착점 설정
	int n = MAZE_SIZE - 2; // 미로사이즈가 10이라 가정하면 바깥에 존재하는 벽은 0과 9로 이루어져 있을테니, 움직일 수 있는 미로 내부의 젤 끝은 1과 8이 되므로 -2를 함

	// 출발점 설정
	int randomStart = rand() % 4; // rand 함수가 반환하는 0부터 RAND_MAX의 값 중 4로 나눈 나머지, 즉 0, 1, 2, 3만 나오게 끔 하는 함수
	switch (randomStart) {
	case 0: // (1, 1)을 출발점으로 설정
		start_x = 1;
		start_y = 1;
		break;
	case 1: // (1, n)을 출발점으로 설정
		start_x = 1;
		start_y = n;
		break;
	case 2: // (n, 1)을 출발점으로 설정
		start_x = n;
		start_y = 1;
		break;
	case 3: // (n, n)을 출발점으로 설정
		start_x = n;
		start_y = n;
		break;

	}
	end_x = MAZE_SIZE - 1 - start_x; // 이 수식을 계산하면 출발점의 대각선 반대에 있는 꼭짓점의 좌표가 도착점으로 설정 됨
	end_y = MAZE_SIZE - 1 - start_y;
}

void cursor(int n) { // 커서 깜빡임 제거 용도
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
