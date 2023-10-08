#ifndef Player_move_hpp
#define Player_move_hpp

#include <iostream>	
#pragma warning(disable : 4996) // kbhit 오류 무시
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
using namespace std;

void Playing();
void cursor(int n); // 커서 깜박임 제거
void StartFinishPoint(int MAZE_SIZE);

#endif
