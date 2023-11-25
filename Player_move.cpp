#include <windows.h>
#include <iostream>
#include <conio.h> // _getch 함수를 사용하기 위함
#include<queue>
#include<set>
#include<map>
#include<chrono>
#include <vector>
#include <chrono>
#include "Text.hpp"
#include "fmod.hpp"
#include "Player_move.hpp"
#include "a_maze_map.hpp"
using namespace std;
using namespace chrono;

extern FMOD::System* Fmod; // Fmod 시스템 클래스를 가리키는 Fmod 포인터를 외부 참조
extern FMOD::Sound* Die; // 플레이어 사망 효과음 재생 포인터를 외부 참조
extern FMOD::Channel* channel2; // 채널 2에서 효과음을 재생하도록 외부 참조

extern int N; // a_maze_map.cpp 에서 참조한 미로 크기 
extern vector<vector<int>> maze; // a_maze_map.cpp 에서 참조한 이차원 벡터 미로
int MAZE_SIZE = N; // 미로 크기
extern chrono::steady_clock::time_point startTime; // 플레이 시간 측정을 위한 전역 변수를 외부 참조
extern int BombCount; // main.cpp 에서 참조한 폭탄 개수
extern bool HighLevel; // main.cpp 에서 참조한 적 기능 포함 여부

class Move { // 이동 함수를 담당하는 기본 클래스
public:
	virtual bool isMoveBlocked(int x, int y) {

		if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) { // 이동하려는 방향이 미로의 크기에 벗어나지 않는지 확인
			return true; // 벽에 막혔을 때 경고음 소리로 잘못된 키보드 입력이란걸 알림
		}

		if (maze[x][y] == 1) { // 이동할려는 방향에 벽이 있는지 확인
			return true;
		}
		return false; // 기본 전제로 벽이 없다고 가정함
	}

	virtual void move(int& x, int& y) = 0; // 플레이어 이동 함수. 순수 가상 함수로 정의했고, 파생 클래스에서 무조건 동작을 정의해야 함
};

class MoveUp :public Move { // 상속 받은 Up 클래스. 위로 움직이는 기능 구현
public:
	void move(int& x, int& y) override {
		if (!Move::isMoveBlocked(x, y - 1)) { // 미로 크기에 벗어나지 않거나 진행 방향에 벽이 없다면 위로 이동
			y = y - 1; // 위로 가니깐 현재 위치의 y좌표값을 -1
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
			y = y + 1;
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
			x = x - 1;
		}
	}
};

class MoveRight :public Move { // 상속 받은 Up 클래스. 오른쪽으로 움직이는 기능 구현
public:
	bool isMoveBlocked(int x, int y) override {
		return Move::isMoveBlocked(x + 1, y); // 오른쪽으로 가니깐 x좌표를 +1 함
	}
	void move(int& x, int& y) override {
		if (!isMoveBlocked(x, y)) {
			x = x + 1;
		}
	}
};

// 적 클래스	
class Enemy {
private:
	int x, y; // 적의 좌표(최단 경로 계산을 위한 배열 번호)
	int fx, fy; // 화면 출력을 위한 콘솔창 좌표값
	vector<vector<int>> maze; // 미로 정보

	int dx[4] = { 0, 0, 1, -1 }; // 상하좌우
	int dy[4] = { 1, -1, 0, 0 };
	map<pair<int, int>, pair<int, int>> parent; // 경로 추적을 위한 맵

	// 해당 좌표가 이동 가능한지 확인하는 함수
	bool isMoveBlocked(int x, int y) const {
		if (x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size()) {
			return true;
		}
		return maze[x][y] == 1; // 벽이면 이동 불가
	}

	// 적의 현재 위치에서 목적지까지의 최단 경로를 찾는 함수
	vector<pair<int, int>> findShortestPath(int player_x, int player_y) {
		queue<pair<int, int>> q; // BFS를 위한 큐
		set<pair<int, int>> visited; // 방문한 좌표를 저장하기 위한 set

		// 시작 위치를 큐에 넣고 방문 표시
		q.push(make_pair(x, y));
		visited.insert(make_pair(x, y));

		// BFS 수행
		while (!q.empty()) {
			pair<int, int> current = q.front();
			q.pop();

			// 목적지에 도달하면 경로 추적
			if (current.first == player_x && current.second == player_y) {
				vector<pair<int, int>> shortestPath;
				while (current.first != x || current.second != y) {
					shortestPath.push_back(current);
					current = parent[current];
				}
				reverse(shortestPath.begin(), shortestPath.end());
				return shortestPath;
			}

			// 현재 위치에서 이동 가능한 모든 방향을 확인
			for (int i = 0; i < 4; ++i) {
				int next_x = current.first + dx[i];
				int next_y = current.second + dy[i];
				pair<int, int> nextCoordinate = make_pair(next_x, next_y);

				// 이동 가능한 좌표이고 아직 방문하지 않았다면 큐에 추가
				if (!isMoveBlocked(next_x, next_y) && visited.find(nextCoordinate) == visited.end()) {
					q.push(nextCoordinate);
					visited.insert(nextCoordinate);
					parent[nextCoordinate] = current;
				}
			}
		}

		// 도달할 수 없는 경우 빈 벡터 반환
		return vector<pair<int, int>>();
	}

public:
	// 생성자
	Enemy(int enemy_x, int enemy_y, const vector<vector<int>>maze) : x(enemy_x), y(enemy_y), maze(maze), fx(enemy_x * 2), fy(enemy_y){} 

	void moveTowardsExit(int player_x, int player_y) {
		// 최단 경로 찾기
		vector<pair<int, int>> shortestPath = findShortestPath(player_x, player_y);

		// 최단 경로 따라 이동
		for (const auto& next : shortestPath) {
			x = next.first ;
			y = next.second;
			fx = x * 2;
			fy = y;
			break;
		}		
	}

	// 화면 출력을 위한 좌표값 반환
	int getX() const { return fx; }
	int getY() const { return fy; }

	// 배열 번호 반환
	int GetX() const { return x; }
	int GetY() const { return y; }

};

int start_x, start_y; // 시작 위치 변수 저장용
int end_x, end_y; // 도착점 위치 변수 저장용 

class Player { // 여기서 키보드 입력값을 받고, 이동 함수들을 호출 할 예정. 
private:
	int x, y; // 플레이어 좌표
	int e_x, e_y; // 도착점 좌표 저장용
	Move* currentMove; // 부모 클래스의 포인터로 자식 클래스의 객체를 동적 할당 함. 다형성을 드러냄

public:
	Player(int start_x, int start_y, int end_x, int end_y) : x(start_x), y(start_y), e_x(end_x), e_y(end_y), currentMove(nullptr) {} // 생성자 정의.포인터 값 null로 지정
	~Player() {                // 소멸자 정의
		delete	currentMove;   // 매 판이 끝나면 플레이어 이동을 담당한 동적 할당된 메모리들을 삭제함
	}

	void handleInput() {
		char direction; // 키보드 값 입력 받을 변수
		direction = _getch(); // 키보드 값 입력 받음

		switch (direction) { 
		case'w':
		case'W':
			currentMove = new MoveUp(); // 포인터 변수에 객체를 동적 할당
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
			currentMove->move(x, y); // move 함수 호출. 다형성 사용
		}
	}

	int getX() const { return x; } // 외부에서 x값을 가져갈 때. x 값을 건들이지 않도록 const를 사용
	int getY() const { return y; }

	void restart() { x = start_x; y = start_y; }	//폭탄을 밟았을 때 쓰는 함수 / 플레이어의 위치를 처음 위치로 이동시킴
};


int turnCount = 0; //이동 횟수를 저장하는 변수
class Bomb {
public:

	//폭탄 설치 함수
	void bombset() {
		int bombX, bombY;		// 폭탄의 좌표값
		int count = 0;			// 폭탄 개수

		// 난이도 별로 폭탄 생성
		while (count != BombCount) {

			bombX = rand() % (MAZE_SIZE - 2) + 1;		//무작위 좌표 선택, 난수 생성기 초기화는 Playing함수에서 이미 함
			bombY = rand() % (MAZE_SIZE - 2) + 1;

			// 폭탄이 미로 밖에 생성될 경우 이를 미로에 반영하지 않고 재생성
			if (bombX < 0 || bombX >= MAZE_SIZE || bombY < 0 || bombY >= MAZE_SIZE) {
				continue;
			}

			// 폭탄이 시작지점이나 목적지, 혹은 벽에 생길 경우 역시 재생성
			if (maze[bombX][bombY] == 1 || bombX == start_x && bombY == start_y || bombX == end_x && bombY == end_y) {
				continue;
			}

			// 벡터 내에서는 폭탄을 2로 표시
			maze[bombX][bombY] = 2;

			// 폭탄이 올바르게 생성되면 값이 1 증가, continue문으로 스킵하기 때문에 설정한 개수에 달할 때 까지 계속 생성
			count += 1;
		}
	};

	//폭탄 폭발 함수
	bool explosion(int x, int y) {
		if (maze[x][y] == 2 && turnCount % 2 == 0) {	// 플레이어가 짝수 번 이동한 후 그 위치에 폭탄이 있다면
			Fmod->playSound(Die, 0, false, &channel2);  // 폭탄을 밟으면 플레이어 사망 효과음 재생
			Fmod->update();
			maze[x][y] = 0;		// 우선 폭탄이 터졌으므르 폭탄을 없애고
			return true;		// true 반환
		}
		return false;			// 일반적으로는 false 반환
	}
};


// 스테이지 난이도 선택 후 본격적으로 게임 실행하는 함수, 미로 크기를 size 매개 변수로 받음.
void Playing(int size) {
	N = size; // 매개 변수 size 로 미로 크기 정적 변수 재설정
	MAZE_SIZE = size; // 매개 변수 size 로 MAZE_SIZE 정적 변수 재설정
	maze.clear();
	maze.resize(size, vector<int>(size, 1)); // 매개 변수 size 로 이차원 벡터 미로 정적 변수의 크기 재설정

	startTime = chrono::steady_clock::now(); // 플레이 시간 측정 시작

	srand(static_cast<unsigned>(time(0))); // 난수 생성기 초기화
	StartMaze(); // 미로를 그리기 위한 준비 및 미로 생성 함수 호출
	StartFinishPoint(MAZE_SIZE); // 출발점과 도착점을 생성하는 함수 호출
	Fmod->createSound(".\\Sounds\\Die.mp3", FMOD_LOOP_OFF, 0, &Die); // 플레이어 사망 효과음 객체 생성
	Fmod->update();

	Player player(start_x, start_y, end_x, end_y); // 플레이어 객체 생성
	Enemy enemy(start_x, start_y, maze); // 적 객체 생성
	Bomb bomb;			//폭탄 개체 생성
	bomb.bombset();		// 미로 내에 폭탄을 무작위로 생성

	steady_clock::time_point start_time = steady_clock::now();

	while (true)			// 입력을 받으면 움직이도록 while 문을 계속 수행
	{
		Fmod->update();
		printMaze(player.getX(), player.getY(), end_x, end_y); // 미로를 그리는 함수 호출
		cursor(0);			// 0 = 깜빡임 제거 / 1 = 깜빡임 생성

		steady_clock::time_point current_time = steady_clock::now(); // 현재 시간을 기록하는 시점
		duration<double> elapsed_seconds = current_time - start_time; // 시작 시간부터 현재까지 경과한 시간을 측정

		if (HighLevel) { //스테이지 3,4번만 해당. 고난이도용
			if (elapsed_seconds.count() >= 3.0) { // 게임 시작 후 3초가 경과했을 때 적 생성
				printEnemy(enemy.getX(), enemy.getY()); // 적의 현재 위치를 화면에 출력
				if (turnCount % 1 == 0) { // 플레이어 이동할 때마다 적도 같이 이동
					enemy.moveTowardsExit(player.getX(), player.getY()); // 적이 플레이어 쪽으로 이동
				}
				if (player.getX() == enemy.GetX() && player.getY() == enemy.GetY()) { // 플레이어가 적에게 잡힐 경우(적의 범위는 2칸), 탈출 실패 !
					DrawGameOver(); 
					system("cls");
					break;
				}
			}
		}

		player.handleInput(); // 키보드 입력 받는 함수 호출
		turnCount++;		  // 키보드 입력시 숫자 증가

		if (bomb.explosion(player.getX(), player.getY())) {	//폭발 조건을 만족하면
			player.restart();							// 플레이어를 시작 위치로 이동시킴
		}

		Sleep(5);			// 5ms 간격으로 화면 갱신
		system("cls");		// 화면 갱신

		if (player.getX() == end_x && player.getY() == end_y) { // 플레이어가 도착 지점에 도착한 경우, 게임 클리어!
			DrawClear();
			system("cls");
			break;
		}	
	}
}

void StartFinishPoint(int MAZE_SIZE) { // 출발점과 도착점을 생성하는 함수

	// 출발점과 도착점 설정
	int n = MAZE_SIZE - 2; // 미로사이즈가 10이라 가정하면 바깥에 존재하는 벽은 0과 9로 이루어져 있을테니, 움직일 수 있는 미로 내부의 젤 끝은 1과 8이 되므로 -2를 함

	// 출발점 설정
	int randomStart = rand() % 4; // rand 함수가 반환하는 0부터 RAND_MAX의 값 중 4로 나눈 나머지, 즉 0, 1, 2, 3만 나오게 끔 하는 함수
	switch (randomStart) {
	case 0: // (1, 1)을 출발점으로 설정
		start_x = 1;
		start_y = 1;
		break;
	case 1: // (1, n)을 출발점으로 설정
		start_x = 1;
		start_y = n;
		break;
	case 2: // (n, 1)을 출발점으로 설정
		start_x = n;
		start_y = 1;
		break;
	case 3: // (n, n)을 출발점으로 설정
		start_x = n;
		start_y = n;
		break;

	}
	end_x = MAZE_SIZE - 1 - start_x; // 이 수식을 계산하면 출발점의 대각선 반대에 있는 꼭짓점의 좌표가 도착점으로 설정 됨
	end_y = MAZE_SIZE - 1 - start_y;
}

//폭탄 설치 함수
void bombset() {	
	int bombX, bombY;		// 폭탄의 좌표값
	int count = 0;			// 폭탄 개수

	// 난이도 별로 폭탄 생성
	while (count != BombCount) {

		bombX = rand() % (MAZE_SIZE - 2) + 1;		//무작위 좌표 선택, 난수 생성기 초기화는 Playing함수에서 이미 함
		bombY = rand() % (MAZE_SIZE - 2) + 1;

		// 폭탄이 미로 밖에 생성될 경우 이를 미로에 반영하지 않고 재생성
		if (bombX < 0 || bombX >= MAZE_SIZE || bombY < 0 || bombY >= MAZE_SIZE) {
			continue;
		}

		// 폭탄이 시작지점이나 목적지, 혹은 벽에 생길 경우 역시 재생성
		if (maze[bombX][bombY] == 1 || bombX == start_x && bombY == start_y || bombX == end_x && bombY == end_y) {
			continue;
		}

		// 벡터 내에서는 폭탄을 2로 표시
		maze[bombX][bombY] = 2;

		// 폭탄이 올바르게 생성되면 값이 1 증가, continue문으로 스킵하기 때문에 설정한 개수에 달할 때 까지 계속 생성
		count += 1;
	}
};

void cursor(int n) { // 커서 깜빡임 제거 용도
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
