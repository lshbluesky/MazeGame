#ifndef ranking_hpp
#define ranking_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

class Rank {
public:
    std::string name;
    double playTime;

    bool operator<(const Rank& other) const {
        return playTime < other.playTime;
    }
};

void RecordPlaytime(const string& playerName);
void UpdateRanking(const string& playerName, double playTime);
string GetPlayerName();
void showRanking();
void PlayTime();

#endif  
