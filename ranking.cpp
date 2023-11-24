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
        outputFile << playerName << "�� �÷��� �ð�: " << elapsed_seconds.count() << " ��\n";
        outputFile.close();
    }
}

void UpdateRanking(const string& playerName, double playTime) {
    // �÷��̾� ��ŷ ���� �б�
    vector<Rank> playerRanking;
    ifstream rankingFile("C:\\Temp\\playtime.txt");

    if (rankingFile.is_open()) {
        Rank tempPlayer;
        while (rankingFile >> tempPlayer.name >> tempPlayer.playTime) {
            playerRanking.push_back(tempPlayer);
        }
        rankingFile.close();
    }

    // �÷��̾� ��ŷ�� �߰��ϰ� ����
    playerRanking.push_back({ playerName, playTime });
    sort(playerRanking.begin(), playerRanking.end(), [](const Rank& a, const Rank& b) {
        return a.playTime < b.playTime;
        });

    // ���� 10�� ����
    if (playerRanking.size() > 10) {
        playerRanking.resize(10);
    }

    // �÷��̾� ��ŷ ���Ͽ� ����
    ofstream rankingFileOut("C:\\Temp\\playtime.txt");
    if (rankingFileOut.is_open()) {
        for (const Rank& player : playerRanking) {
            rankingFileOut << player.name << " " << player.playTime << endl;
        }
        rankingFileOut.close();
        cout << "��ŷ�� ����Ǿ����ϴ�." << endl;
    }
    else {
        cerr << "��ŷ ������ �� �� �����ϴ�." << endl;
    }
}

string GetPlayerName() {
    string playerName;
    cout << " �÷��̾��� �г����� �Է��ϼ���: ";
    getline(cin, playerName);
    return playerName;
}

void PlayTime() {
    startTime = chrono::steady_clock::now();

    // �÷��̾��� �г��� �Է� �ޱ�
    string playerName = GetPlayerName();

    // ������ ������ �÷��� �ð� ���
    RecordPlaytime(playerName);

    // ��ŷ ����
    UpdateRanking(playerName, chrono::duration<double>(chrono::steady_clock::now() - startTime).count());
}

void showRanking() {
   
    cout << "==================== ��ŷ ====================" << endl;

    vector<Rank> playerRanking;
    ifstream rankingFile("C:\\Temp\\playtime.txt");

    if (rankingFile.is_open()) {
        Rank tempPlayer;
        int rank = 1;
        while (rankingFile >> tempPlayer.name >> tempPlayer.playTime) {
            cout << rank << "�� " << tempPlayer.name << ": " << tempPlayer.playTime << " ��" << endl;
            rank++;
        }
        rankingFile.close();
    }
    else {
        cerr << "��ŷ ������ �� �� �����ϴ�." << endl;
    }
}


