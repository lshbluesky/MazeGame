#include <iostream>	
#pragma warning(disable : 4996)		//kbhit ���� ����
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

using namespace std;

void cursor(int n);				//Ŀ�� ������ ����
void gotoxy(int x, int y);		//�ܼ� �� ��ǥ�̵�
int RandomStart(int min, int max);	//������ ��ġ ����
bool isWall(int x, int y);			//���� �ִ��� Ȯ��
