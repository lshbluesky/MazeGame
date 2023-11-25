﻿#include "a_maze_map.hpp"
#include "Text.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int N = 101; // 미로의 크기 (홀수로 설정)
vector<vector<int>> maze(N, vector<int>(N, 1)); // (1은 벽, 0은 길)
extern int turnCount;

int dx[] = { 0, 1, 0, -1 }; // 상하좌우 이동을 위한 배열
int dy[] = { -1, 0, 1, 0 };

void printMaze(int player_x, int player_y, int end_x, int end_y) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == player_x && i == player_y) {
                TextColor(DEEP_YELLOW);
                cout << "★"; // 플레이어 아이콘
                TextColor(DEEP_WHITE);
            }
            else if (j == end_x && i == end_y) {
                TextColor(DEEP_GREEN);
                cout << "◈"; // 도착점 아이콘
                TextColor(DEEP_WHITE);
            }
            else if (maze[j][i] == 0 || (maze[j][i] == 2 && turnCount % 2 == 1)) { // 폭탄을 보이지 않게 해 난이도를 상승시킴
                cout << "  "; // 길
            }
            else if (maze[j][i] == 2 && turnCount % 2 == 0) { // 짝수 번 움직였을 때에는 폭탄을 표시
                TextColor(DEEP_OC);
                cout << "♠"; // 폭탄
                TextColor(DEEP_WHITE);
            }
            else {
                cout << "■"; // 벽
            }
        }
        cout << endl;
    }
}

// 적 출력 함수
void printEnemy(int enemy_x, int enemy_y) {
    gotoxy(enemy_x, enemy_y);
    TextColor(DEEP_RED);
    cout << "♣"; // 적 아이콘
    TextColor(DEEP_WHITE);
}

bool exist(int x, int y) {  //좌표가 유효한지 확인
    return x >= 0 && x < N&& y >= 0 && y < N;
}

bool wall(int x, int y) { //좌표에 벽이 있는지 확인
    return maze[x][y] == 1;
}

void generateMaze(int x, int y) {
    maze[x][y] = 0; // 현재 위치를 길로 설정

    // 무작위 순서로 상하좌우 방향 설정
    int direction[] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; i++) {
        int randIdx = rand() % 4;
        swap(direction[i], direction[randIdx]);
    }

    for (int i = 0; i < 4; i++) {
        int newX = x + dx[direction[i]] * 2;
        int newY = y + dy[direction[i]] * 2;

        if (exist(newX, newY) && wall(newX, newY)) {
            // 새로운 위치로 이동하며 중간 벽을 부숨
            maze[x + dx[direction[i]]][y + dy[direction[i]]] = 0;
            generateMaze(newX, newY);
        }
    }
}

void StartMaze() {
    srand(static_cast<unsigned>(time(nullptr)));

    // 초기 미로 생성 (테두리와 내부를 모두 벽으로 설정)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                maze[i][j] = 1; // 테두리는 벽으로 설정
            }
            else {
                maze[i][j] = 1; // 내부도 벽으로 설정
            }
        }
    }

    // 랜덤한 시작 위치 생성 (테두리에서 멀리)
    int startX = rand() % (N / 2) * 2 + 1;
    int startY = rand() % (N / 2) * 2 + 1;

    generateMaze(startX, startY);
    //printMaze(); // 미로를 그리는 함수는 Player_move.cpp 의 Playing 함수에서 while 문 안에서 따로 계속 수행하기.
}
