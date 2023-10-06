/* 

*/


#include <iostream>
#include <conio.h> // getch �Լ��� ����ϱ� ����
using namespace std;

const int MAZE_SIZE = 5; // �̷� ũ��

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
			return true;
		}
		if (maze[x][y] == '#') { // �̵��ҷ��� ���⿡ ���� �ִ��� Ȯ��
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
			y--; // ���� ���ϱ� ���� ��ġ�� y��ǥ���� -1
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
			y++;
		}
	}
};

class MoveLeft :public Move { // ��� ���� Up Ŭ����. �������� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override { 
		return Move::isMoveBlocked(x - 1, y); // �������� ���ϱ� x��ǥ�� -1 ��
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) { 
			x--;
		}
	}
};

class MoveRight :public Move{ // ��� ���� Up Ŭ����. ���������� �����̴� ��� ����
public:
	bool isMoveBlocked(int x, int y) override { 
		return Move::isMoveBlocked(x + 1, y); // ���������� ���ϱ� x��ǥ�� +1 ��
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) { 
			x++;
		}
	}
};

class Player { // ���⼭ Ű���� �Է°��� �ް�, �̵� �Լ����� ȣ�� �� ����. 
private:
	int x, y; // �÷��̾� ��ǥ
	Move* currentMove; // �θ� Ŭ������ �����ͷ� �ڽ� Ŭ������ ��ü�� ���� �Ҵ� ��. �������� �巯��

public:
	Player() : x(0), y(0), currentMove(nullptr) {} // ������ ����. ��ǥ�� 0���� �ʱ�ȭ(�Ϲ����� ������ġ), ������ �� null�� ����
	~Player() {                                    // �Ҹ��� ����
		delete	currentMove;                       // �� ���� ������ �÷��̾� �̵��� ����� ���� �Ҵ�� �޸𸮵��� ������
	}

	void handleInput() {
		char direction; // Ű���� �� �Է� ���� ����
		direction = _getch(); // Ű���� �� �Է� ����

		switch (direction) { // 
		case'w':
		case'W':
			currentMove = new MoveUp(); // ������ ������ ��ü�� ���� �Ҵ�
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
			currentMove->move(x, y); // move �Լ� ȣ��. ������ ���
		}
	}  
	int getX() const { return x; } // �ܺο��� x���� ������ ��. x ���� �ǵ����� �ʵ��� const�� ���
	int getY() const { return y; }
};

