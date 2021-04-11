//
// Created by pratyush on 25/03/21.
//

#ifndef HELLO_WORLD_MAZE_H
#define HELLO_WORLD_MAZE_H


#include <vector>

class Maze {

    int getIdx(int x, int y, const std::vector<std::pair<int, std::pair<int, int>>>& cell_list);
    void createMaze(int M, int N);

public:
    int                                              M, N, n, m;
    char                                             **maze;
    std::vector<std::vector<std::pair<int, int>>>    adj;
// constructor/destructor
    Maze(int m,int n);
    ~Maze();
    void Init();
    int shortestPathBFS(std::pair<int, int> s, std::pair<int, int> dest);
    bool BFS(std::pair<int, int> src, std::pair<int, int> dest, std::pair<int, int> pred[], int dist[]);
};


#endif //HELLO_WORLD_MAZE_H
