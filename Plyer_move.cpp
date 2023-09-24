/*
���� ���: ������
Ŭ������ 2�� ��� ����(Player, Move)
Player Ŭ���������� ��ġ������ �߰��� ���(��ź, ���� ���)
Move Ŭ���������� �̵��� �߰��� ���(�� �б�, ���� ������)
Player�� �����ϴٸ� �̱��浵 ���
*/

#include <random>
#include <conio.h>			//getch�� �ʿ�
#include <windows.h>		//gotoxy�� �ʿ�
#include "Plyer_move.h"

using namespace std;

class Player
{
private:
	int x = NULL;
	int y = NULL;

	//���� ��ġ

	int prev_x;
	int prev_y;
	//���� ��ġ
public:
	Player(int start_x, int start_y) : x(start_x), y(start_y), prev_x(start_x), prev_y(start_y) {}
	//������������ġ�� ������ġ�� �ʱ�ȭ

	void Move(int direction) {		//�Է¹޾� �ൿ�ϴ� �Լ�
		prev_x = x;
		prev_y = y;
		//���� ��ġ�� ���

		switch (direction)			//�Է¿� ���� �ൿ ����
		{
		case UP:
			y--; break;
		case DOWN:
			y++; break;
		case LEFT:
			x--; break;
		case RIGHT:
			x++; break;
		}

		int new_x = x;		//�̵��� ��ġ�� ���
		int new_y = y;


		//�̵� ��ġ�� ���� ���(isWall()�� ��ȯ���� true�� ���) >>>  ���� ��ġ�� ���ư�
		//�̵� ��ġ�� ���� �ƴ� ���(isWall()�� ��ȯ���� false�� ���) >>>  �ش� ��ġ�� �̵�	
		if (!isWall(new_x, new_y)) {
			x = new_x;
			y = new_y;
		}
		else {
			x = prev_x;
			y = prev_y;
		}
	}

	//��ǻ� �÷��̾� ��ġ�� ǥ���ϸ� ��
	void print() {					//ȭ�� ����

		gotoxy(x, y);				//�÷��̾��� ��ġ�� �ڷ� ǥ��
		cout << "��";
	}

	int get_x() {			//���߿� x,y ��ġ �ʿ��� �� ���°�
		return x;
	}
	int get_y() {
		return y;
	}
};


int start_x, start_y; //���� ��ġ ���� �����

//�Ʒ� �Լ� ������ �׳� main�� �����ϴ� ���� ���ҵ�, �ּ��� Randomstart�� Player�� �ؾ���
int Playing() {

	char input;				//�Է� ����� ����

	RandomStart(5, 5);		//�̷� ������ �޵���
	Player player(start_x, start_y);		//��ü ����, RandomStart ���� ���� ��� ������?


	while (true)			//�Է��� ������ �����̵���
	{
		player.print();
		cursor(0);			//0 = ������ ���� / 1 = ������ ����

		if (kbhit()) {		//kbhit() <<< Ű���� �Է� Ȯ��

			input = getch();//�Է� ����

			switch (input)
			{
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
				player.Move(input); break;
			}
		}
		Sleep(5);			//5ms �������� ȭ�� ����
		system("cls");		//ȭ�� ����
	}
}

void gotoxy(int x, int y) {

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

void cursor(int n) {

	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

//���� ��ġ�� �����ϰ�, �̷��� �ִ� ������ �޵���
int RandomStart(int max_x, int max_y) {

	random_device rd;
	mt19937  generator(rd());			//mt19937 ���� ���

	//x, y�� �ִ� ���� ���� ���� �����ϴ� distribution
	uniform_int_distribution<int> distribution_x(0, max_x); // x ��ǥ�� ����
	uniform_int_distribution<int> distribution_y(0, max_y); // y ��ǥ�� ����

	do {
		start_x = distribution_x(generator);			//������ ��ġ���� ����
		start_y = distribution_y(generator);

	} while (isWall(start_x, start_y)); // ���� (x, y)�� ������ �����Ǹ� �ٽ� ��ġ�� �����ϰ� ����, isWall �Լ��� �� ������ �Ѵٰ� ����

	return start_x, start_y;
}

//�ش� ��ġ�� ������ Ȯ���ϴ� �Լ� / ���⼭�� ���� ���� '#'���� ����
bool isWall(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);		//�ְܼ� ��ȣ�ۿ��ϱ� ���� �ڵ�
	CHAR_INFO Pos_buffer;									//ȭ�� ���ڸ� ������ ����
	COORD bufferSize = { 1, 1 };							//1x1 ũ���� ���ڸ� �а� ����
	COORD bufferCoord = { 0, 0 };							//�»�ܺ��� ���ڸ� ����

	//�о�� ȭ�� ������ ����, ���� ��ġ�� 1x1 ũ���� ������ ����
	SMALL_RECT readRegion = { static_cast<SHORT>(x), static_cast<SHORT>(y), static_cast<SHORT>(x), static_cast<SHORT>(y) };

	// ���� ��ġ�� ȭ�� ���ڸ� �а� Pos_buffer�� ����
	ReadConsoleOutput(hConsole, &Pos_buffer, bufferSize, bufferCoord, &readRegion);

	// �о�� ���ڰ� '#'(��) �������� Ȯ���Ͽ� ��ȯ
	return (Pos_buffer.Char.AsciiChar == '#');
}