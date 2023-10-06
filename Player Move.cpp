/* 

*/


#include <iostream>
#include <conio.h> // getch 함수를 사용하기 위함
using namespace std;

const int MAZE_SIZE = 5; // 미로 크기

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
			return true;
		}
		if (maze[x][y] == '#') { // 이동할려는 방향에 벽이 있는지 확인
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
			y--; // 위로 가니깐 현재 위치의 y좌표값을 -1
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
			y++;
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
			x--;
		}
	}
};

class MoveRight :public Move{ // 상속 받은 Up 클래스. 오른쪽으로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override { 
		return Move::isMoveBlocked(x + 1, y); // 오른쪽으로 가니깐 x좌표를 +1 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) { 
			x++;
		}
	}
};

class Player { // 여기서 키보드 입력값을 받고, 이동 함수들을 호출 할 예정. 
private:
	int x, y; // 플레이어 좌표
	Move* currentMove; // 부모 클래스의 포인터로 자식 클래스의 객체를 동적 할당 함. 다형성을 드러냄

public:
	Player() : x(0), y(0), currentMove(nullptr) {} // 생성자 정의. 좌표값 0으로 초기화(일반적인 시작위치), 포인터 값 null로 지정
	~Player() {                                    // 소멸자 정의
		delete	currentMove;                       // 매 판이 끝나면 플레이어 이동을 담당한 동적 할당된 메모리들을 삭제함
	}

	void handleInput() {
		char direction; // 키보드 값 입력 받을 변수
		direction = _getch(); // 키보드 값 입력 받음

		switch (direction) { // 
		case'w':
		case'W':
			currentMove = new MoveUp(); // 포인터 변수에 객체를 동적 할당
			// MoveUp
			break;
		case's':
		case'S':
			currentMove = new MoveDown();
			// MoveDown 
			break;
		case'a':
		case'A':
			currentMove = new MoveLeft();
			// MoveLeft 
			break;
		case'd':
		case'D':
			currentMove = new MoveRight();
			// MoveRight 
			break;
		}

		if (currentMove != nullptr) {
			currentMove->move(x, y); // move 함수 호출. 다형성 사용
		}
	}  
	int getX() const { return x; } // 외부에서 x값을 가져갈 때. x 값을 건들이지 않도록 const를 사용
	int getY() const { return y; }
};

