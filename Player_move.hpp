#ifndef Player_move_hpp
#define Player_move_hpp

#include <iostream>	
#pragma warning(disable : 4996) // kbhit 오류 무시
using namespace std;

void DrawClear();
void Playing(int size);
void cursor(int n); // 커서 깜박임 제거
void StartFinishPoint(int MAZE_SIZE);

#endif
