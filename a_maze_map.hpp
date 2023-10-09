#ifndef a_maze_map_hpp
#define a_maze_map_hpp

const int N = 13;

void printMaze(int player_x, int player_y, int end_x, int end_y);
bool exist(int x, int y);
bool wall(int x, int y);
void generateMaze(int x, int y);
void StartMaze();

#endif
