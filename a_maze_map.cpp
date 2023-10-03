#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 11; // 미로의 크기 (홀수로 설정)
vector<vector<int>> maze(N, vector<int>(N, 1)); // 미로를 나타내는 2D 벡터 (1은 벽, 0은 길)

int dx[] = { 0, 1, 0, -1 }; // 상하좌우 이동을 위한 배열
int dy[] = { -1, 0, 1, 0 };

void printMaze() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == 0) cout << " "; // 길
            else cout << "#"; // 벽
        }
        cout << endl;
    }
}

bool exist(int x, int y) {
    return x >= 0 && x < N&& y >= 0 && y < N;
}

bool wall(int x, int y) {
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

int main() {
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

    // DFS로 미로 생성 시작
    generateMaze(startX, startY);

    // 미로 출력
    printMaze();

    return 0;
}
