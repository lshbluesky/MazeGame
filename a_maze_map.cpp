#include <iostream>
#include <vector>
#include <cstdlib> //난수 생성과 관련된 함수를 제공
#include <ctime>   //미로 생성 시마다 다른 난수 패턴 얻을 수 있음
#include <fstream> //파일 입출력

using namespace std;

class Maze {
public:
    Maze(int level) {
        // 레벨에 따라 미로 크기 설정
        if (level == 1) {
            size = 11;
        }
        else if (level == 2) {
            size = 21;
        }
        else if (level == 3) {
            size = 51;
        }
        else { // 다른 레벨을 입력하였을 때
            cerr << "잘못된 레벨을 입력하였습니다." << endl;
        }

        maze = vector<vector<int>>(size, vector<int>(size, 1)); // 모든 칸을 벽으로 초기화
        dx = { 0, 1, 0, -1 }; // 상하좌우 이동을 위한 배열
        dy = { -1, 0, 1, 0 };
        srand(static_cast<unsigned>(time(nullptr)));
    }

    void generateMaze() {
        // 미로 생성 시작 위치 설정
        int startX = rand() % (size / 2) * 2 + 1;
        int startY = rand() % (size / 2) * 2 + 1;

        // DFS로 미로 생성
        generateMaze(startX, startY);
    }

    void printMaze() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (maze[i][j] == 0) cout << "  "; // 길
                else cout << "■"; // 벽
            }
            cout << endl;
        }
    }

    void saveMaze(const string& filename) { //미로 데이터를 파일을 받아 저장
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file for saving." << endl; //파일이 열리지 않을시 출력
            return;
        }

        file << size << endl;

        for (int i = 0; i < size; i++) { 
            for (int j = 0; j < size; j++) {
                file << maze[i][j] << " ";
            }
            file << endl;
        }

        file.close();
        cout << "Maze saved to " << filename << endl;
    }

    void loadMaze(const string& filename) { //미로 상태를 로드해 현재 미로에 적용
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file for loading." << endl; //파일이 열리지 않을시 출력
            return;
        }

        file >> size;
        maze = vector<vector<int>>(size, vector<int>(size));

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file >> maze[i][j];
            }
        }

        file.close();
        cout << "Maze loaded from " << filename << endl;
    }

private:
    int size;
    vector<vector<int>> maze;
    vector<int> dx;
    vector<int> dy;

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

    bool exist(int x, int y) {
        return x >= 0 && x < size&& y >= 0 && y < size; 
    }

    bool wall(int x, int y) {
        return maze[x][y] == 1;
    }
};

void maze_play() {
    int level;
    cout << "레벨 1, 2, 3 중 하나를 입력하시오: ";
    cin >> level;

    Maze maze(level);

    maze.generateMaze();
    maze.printMaze();
    maze.saveMaze("maze.txt");
}

int main() { //메인 함수에 호출만 하면 됨
    maze_play();
    return 0;
}
