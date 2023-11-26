#include <chrono>
#include <thread>
#include <algorithm>
#include <string>
#include <iomanip>
#include "ranking.hpp"
#include "Text.hpp"
using namespace std;

// 플레이 시간 측정을 위한 전역 변수 정의
chrono::steady_clock::time_point startTime;
chrono::steady_clock::time_point endTime;
chrono::duration<double> elapsed_seconds;

void RecordPlaytime(const string& playerName) { // 플레이 시간을 파일에 기록하여 저장
    ofstream outputFile("C:\\Temp\\playtime.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << playerName << "의 플레이 시간: " << elapsed_seconds.count() << " 초\n";
        outputFile.close();
    }
}

void UpdateRanking(const string& playerName, double playTime) {
    // 플레이어 랭킹 정보 읽기
    vector<Rank> playerRanking;
    ifstream rankingFile("C:\\Temp\\playtime.txt");

    if (rankingFile.is_open()) {
        Rank tempPlayer;
        while (rankingFile >> tempPlayer.name >> tempPlayer.playTime) {
            playerRanking.push_back(tempPlayer);
        }
        rankingFile.close();
    }

    // 플레이어 랭킹에 추가하고 정렬
    playerRanking.push_back({ playerName, playTime });
    sort(playerRanking.begin(), playerRanking.end(), [](const Rank& a, const Rank& b) {
        return a < b; // Rank 클래스의 객체에 대하여 연산자 오버로딩(다형성) 특징 적용
        });

    // 상위 10명만 유지
    if (playerRanking.size() > 10) {
        playerRanking.resize(10);
    }

    // 플레이어 랭킹 파일에 저장
    ofstream rankingFileOut("C:\\Temp\\playtime.txt");
    if (rankingFileOut.is_open()) {
        for (const Rank& player : playerRanking) {
            rankingFileOut << player.name << " " << player.playTime << endl;
        }
        rankingFileOut.close();
        cout << "랭킹이 저장되었습니다." << endl;
    }
    else {
        cerr << "랭킹 파일을 열 수 없습니다." << endl;
    }
}

string GetPlayerName() {
    string playerName;
    getline(cin, playerName);
    return playerName;
}

void PlayTime() {
    // 플레이어의 닉네임 입력 받기
    string playerName = GetPlayerName();

    // 게임이 끝나면 플레이 시간 기록
    RecordPlaytime(playerName);

    // 랭킹 갱신
    UpdateRanking(playerName, chrono::duration<double>(chrono::steady_clock::now() - startTime).count());
}

void showRanking() {
    vector<Rank> playerRanking;
    ifstream rankingFile("C:\\Temp\\playtime.txt");
    short i = 0;

    if (rankingFile.is_open()) {
        Rank tempPlayer;
        int rank = 1;
        while (rankingFile >> tempPlayer.name >> tempPlayer.playTime) {
            gotoxy(7, 6 + i, "");
            cout << "▦ " << setw(2) << rank << "등 " << setw(20) << tempPlayer.name << " : " << setw(18) << tempPlayer.playTime << " 초" << endl;
            cout << "      ------------------------------------------------------" << endl; // 순위 항목 사이에 구분선 추가
            rank++;
            i = i + 2;
        }
        rankingFile.close();
    }
    else {
        gotoxy(6, 7, "▦ 게임을 플레이한 기록이 없기 때문에 순위 랭킹이 존재하지 않습니다.");
        gotoxy(6, 9, "▦ 미로 게임을 최소 한 번 클리어해야 순위 랭킹이 표시됩니다.");
    }
}
