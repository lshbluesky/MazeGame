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
    
    bool operator<(const Rank& other) const { // Rank 클래스의 객체에 대하여 연산자 오버로딩(다형성) 특징 적용
        return playTime < other.playTime;
    }
};

void RecordPlaytime(const string& playerName);
void UpdateRanking(const string& playerName, double playTime);
string GetPlayerName();
void showRanking();
void PlayTime();

#endif  
