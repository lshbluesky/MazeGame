#include "ranking.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>


using namespace std;

chrono::steady_clock::time_point startTime;

void RecordPlaytime(const string& playerName) {
    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = endTime - startTime;

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
        return a.playTime < b.playTime;
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
    cout << " 플레이어의 닉네임을 입력하세요: ";
    getline(cin, playerName);
    return playerName;
}

void PlayTime() {
    startTime = chrono::steady_clock::now();

    // 플레이어의 닉네임 입력 받기
    string playerName = GetPlayerName();

    // 게임이 끝나면 플레이 시간 기록
    RecordPlaytime(playerName);

    // 랭킹 갱신
    UpdateRanking(playerName, chrono::duration<double>(chrono::steady_clock::now() - startTime).count());
}

void showRanking() {
   
    cout << "==================== 랭킹 ====================" << endl;

    vector<Rank> playerRanking;
    ifstream rankingFile("C:\\Temp\\playtime.txt");

    if (rankingFile.is_open()) {
        Rank tempPlayer;
        int rank = 1;
        while (rankingFile >> tempPlayer.name >> tempPlayer.playTime) {
            cout << rank << "등 " << tempPlayer.name << ": " << tempPlayer.playTime << " 초" << endl;
            rank++;
        }
        rankingFile.close();
    }
    else {
        cerr << "랭킹 파일을 열 수 없습니다." << endl;
    }
}


