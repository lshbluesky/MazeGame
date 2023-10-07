/*

*/
#include "Player_move.hpp"
#include "Text.hpp"
#include <windows.h>	
#include <iostream>
#include <conio.h> // getch �Լ��� ����ϱ� ����
using namespace std;

const int MAZE_SIZE = 5; // ���� �̷� ũ��. ���� �̷� ���� �ڵ尡 �ϼ��Ǿ��ٸ� �����ص� ��, �Ʒ� �κе� ��������

char maze[MAZE_SIZE][MAZE_SIZE] = { // �̷ΰ� � ������ ������� �ִ��� ���� �Ʒ� �ڵ带 �����ϱ� ���� ������ �̷θ� ���� ��. '#' �� �κ��� ��
	{' ', ' ', ' ', '#', ' '},
	{'#', '#', ' ', '#', ' '},
	{' ', ' ', ' ', '#', ' '},
	{'#', ' ', '#', '#', '#'},
	{' ', ' ', ' ', ' ', 'E'}
};

class Move { // �̵� �Լ��� ����ϴ� �⺻ Ŭ����
public:
	virtual bool isMoveBlocked(int x, int y) {
		if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) { // �̵��ҷ��� ������ �̷��� ũ�⿡ ����� �ʴ��� Ȯ��
			return true; // ���� ������ �� ����� �Ҹ��� �߸��� Ű���� �Է��̶��� �˸�
		}
		if (maze[x][y] == '��') { // �̵��ҷ��� ���⿡ ���� �ִ��� Ȯ��
			return true;
		}
		return false; // �⺻ ������ ���� ���ٰ� ������
	}

	virtual void move(int& x, int& y) = 0; // �÷��̾� �̵� �Լ�. ���� ���� �Լ��� �����߰�, �Ļ� Ŭ�������� ������ ������ �����ؾ� ��
};

class MoveUp :public Move { // ��� ���� Up Ŭ����. ���� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override { // isMoveBlocked �Լ� �������̵� 
		return Move::isMoveBlocked(x, y - 1); // ���� ���ϱ� y��ǥ�� -1 ��. �� ������ ��ġ�� ��ǥ���� ����
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) { // �̷� ũ�⿡ ����� �ʰų� ���� ���⿡ ���� ���ٸ� ���� �̵�
			y = y - 1; // ���� ���ϱ� ���� ��ġ�� y��ǥ���� -1
		}
	}
};

class MoveDown :public Move { // ��� ���� Down Ŭ����. �Ʒ��� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x, y + 1); // �Ʒ��� ���ϱ� y��ǥ�� +1 ��
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			y = y + 1;
		}
	}
};

class MoveLeft :public Move { // ��� ���� Up Ŭ����. �������� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x - 2, y); // �������� ���ϱ� x��ǥ�� -2 ��
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x - 2;
		}
	}
};

class MoveRight :public Move { // ��� ���� Up Ŭ����. ���������� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x + 2, y); // ���������� ���ϱ� x��ǥ�� +2 ��
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x + 2;
		}
	}
};

class Player { // ���⼭ Ű���� �Է°��� �ް�, �̵� �Լ����� ȣ�� �� ����. 
private:
	int x, y;// �÷��̾� ��ǥ
	int e_x, e_y;
	Move* currentMove; // �θ� Ŭ������ �����ͷ� �ڽ� Ŭ������ ��ü�� ���� �Ҵ� ��. �������� �巯��

public:
	Player(int start_x, int start_y, int end_x, int end_y) : x(start_x), y(start_y), e_x(end_x), e_y(end_y), currentMove(nullptr) {} // ������ ����. ��ǥ�� 0���� �ʱ�ȭ(�Ϲ����� ������ġ), ������ �� null�� ����
	~Player() {                                    // �Ҹ��� ����
		delete	currentMove;                       // �� ���� ������ �÷��̾� �̵��� ����� ���� �Ҵ�� �޸𸮵��� ������
	}

	void handleInput() {
		char direction; // Ű���� �� �Է� ���� ����
		direction = _getch(); // Ű���� �� �Է� ����

		switch (direction) { 
		case'w':
		case'W':
			currentMove = new MoveUp(); // ������ ������ ��ü�� ���� �Ҵ�
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
			currentMove->move(x, y); // move �Լ� ȣ��. ������ ���
		}
	}
	//void GameFinish() { // Ż�� ����
	//	if (x = e_x && y = e_y) { 
	//		// �̷� Ż�� ������ ǥ���ϴ� GAME OVER �� CLEAR ���� ������ �̿��� ȭ���� ��� + ������ �˸��� ȿ������ ���� ���� �� ����
	//		// �ٸ� ���� ȭ���� ���� �����Ǿ� ���� �ʰ� ȿ������ ���� ���¶� ��� �ڵ带 ������ �� �� ���� ��� ��
	//		// Ȥ�� �� �κ� �ڵ� �ϼ� �Ͻ� �� �ִٸ� ��Ź�帳�ϴ�...
	//	}
	//}
	void print() {					// ȭ�� ����
		gotoxy(x, y, "\u2605");// �÷��̾��� ��ġ�� �ڷ� ǥ��
		gotoxy(e_x, e_y, "\u25c8");// ������ ��ġ�� �·� ǥ��
	}

	int getX() const { return x; } // �ܺο��� x���� ������ ��. x ���� �ǵ����� �ʵ��� const�� ���
	int getY() const { return y; }
};

int start_x, start_y; // ���� ��ġ ���� �����
int end_x, end_y; // ������ ��ġ ���� ����� 

// �Ʒ� �Լ� ������ �׳� main�� �����ϴ� ���� ���ҵ�, �ּ��� Randomstart�� Player�� �ؾ���
void Playing() {

	Player player(start_x, start_y, end_x, end_y);		// ��ü ����, RandomStart ���� ���� ��� ������?

	while (true)			// �Է��� ������ �����̵���
	{
		player.print();
		cursor(0);			// 0 = ������ ���� / 1 = ������ ����

		player.handleInput(); // Ű���� �Է� �޴� �Լ� ȣ��

		Sleep(5);			// 5ms �������� ȭ�� ����
		system("cls");		// ȭ�� ����
	}
}

void cursor(int n) { // Ŀ�� ������ ���� �뵵
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


/* �ؿ� ���� StartFinishPoint �Լ��� ������ ���� �� �� ����
* �� �ٱ��� ������ �ѷ��� �ִ� ���¿���, �̷� ������ �� ������ 4������ ������ ���� �ƴ� ��� �����ɰŶ� �����Ͽ� �������(Ȯ���� �������� �� ���� �׽�Ʈ ��� �׷��� ������)
* �׷��� �� 4���� ������ �� �������� �� ���� ������̶� �����ϸ�, �ڵ����� �� �밢�� �ݴ��� �ִ� �������� �������� �ǵ��� ����
* �׷��� ������� �������� �ʹ� ����� ������ �̰ſ��� ����� �ذ��� �� �����Ŷ� �����Ǿ ��� ���� �� ����� ����ϱ�� ����
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*				���� ���� �ð� ���� ����ϰ� �н��ϸ� �׽�Ʈ�� ���� ���, 4���� �������� ���� �̷ΰ� �����Ǵ� ���� �����̰ų� ������ ���̶� �Ǵ��Ͽ����ϴ�
*							Ȥ�ó� ������ �Ǵ��� Ʋ�� �Ŷ�� �̿� ���� �ذ� ����� �� ���� �����غ��� �� �� ���ƿ�
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*							!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Ȥ�ó� �� �κп� ���� �ñ��ϰų� ���ذ� ���� �ʴ� �κ��� �ִٸ� ���� ��Ź�帳�ϴ�.
*/


void StartFinishPoint(int MAZE_SIZE) { // ������� �������� �����ϴ� �Լ�
	srand(static_cast<unsigned>(time(0))); // ���� ������ �ʱ�ȭ

	// ������� ������ ����
	int n = MAZE_SIZE - 2; // �̷λ���� 10�̶� �����ϸ� �ٱ��� �����ϴ� ���� 0�� 9�� �̷���� �����״�, ������ �� �ִ� �̷� ������ �� ���� 1�� 8�� �ǹǷ� -2�� ��
	int startX, startY, endX, endY;

	// ����� ����
	int randomStart = rand() % 4; // rand �Լ��� ��ȯ�ϴ� 0���� RAND_MAX�� �� �� 4�� ���� ������, �� 0, 1, 2, 3�� ������ �� �ϴ� �Լ�
	switch (randomStart) {
	case 0: // (1, 1)�� ��������� ����
		startX = 1;
		startY = 1;
		break;
	case 1: // (1, n)�� ��������� ����
		startX = 1;
		startY = n;
		break;
	case 2: // (n, 1)�� ��������� ����
		startX = n;
		startY = 1;
		break;
	case 3: // (n, n)�� ��������� ����
		startX = n;
		startY = n;
		break;

	}
	endX = MAZE_SIZE - 1 - startX; // �� ������ ����ϸ� ������� �밢�� �ݴ뿡 �ִ� �������� ��ǥ�� ���������� ���� ��
	endY = MAZE_SIZE - 1 - startY;
}


