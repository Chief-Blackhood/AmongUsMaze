#include "Maze.h"
#include <stack>
#include <vector>
#include <random>
#include <iostream>
#include <list>
#include <limits.h>

int Maze::getIdx(int x, int y, const std::vector<std::pair<int, std::pair<int, int>>>& cell_list)
{
    for (auto & i : cell_list)
    {
        if (i.second.first == x && i.second.second == y)
        return i.first;
    }
    std::cout << "getIdx() couldn't find the index!" << std::endl;
    return -1;
}

void Maze::createMaze(int M, int N)
{
    std::vector<std::pair<int, std::pair<int, int>>> cell_list;
    std::vector<bool> visited(this->m * this->n, false);
    std::stack<std::pair<int, std::pair<int, int>>> m_stack;
    std::random_device rdev;
    std::mt19937 rng(rdev());
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < M; i += 2)
    {
        for (int j = 1; j < N; j += 2)
        {
            cell_list.emplace_back(k, std::make_pair(i, j));
            k++;
        }
    }

    int randIdx = dist100(rng) % this->m * this->n;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    // Algo
    while (nVisited < m * n)
    {
        std::vector<int> neighbours;
        // North
        if (m_stack.top().second.first > 1)
        {
            if (this->maze[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)])
            {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < N - 2)
        {
            if (this->maze[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)])
            {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < M - 2)
        {
            if (this->maze[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)])
            {
                neighbours.push_back(2);
            }
        }
        // West
        if (m_stack.top().second.second > 1)
        {
            if (this->maze[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)])
            {
                neighbours.push_back(3);
            }
        }
        // Neighbours available?
        if (!neighbours.empty())
        {
            // Choose a random direction
            int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
            // Create a path between this cell and neighbour
            switch (next_cell_dir)
            {
                case 0: // North
                    this->maze[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 1: // East
                    this->maze[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]);
                    break;
                case 2: // South
                    this->maze[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 3: // West
                    this->maze[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = ' ';
                    m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]);
                    break;
            }

            visited[m_stack.top().first] = true;
            nVisited++;
        }
        else
        {
            m_stack.pop();
        }
    }
}

Maze::~Maze() {

}

Maze::Maze(int m, int n)
        : M(2*m+1), N(2*n+1), m(m), n(n)
{

}



void Maze::Init() {
    this->maze = new char *[M];

    for (int i = 0; i < M; i++) {
        this->maze[i] = new char[N];
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (!(i & 1) || !(j & 1))
                this->maze[i][j] = '#';
            else
                this->maze[i][j] = ' ';
        }
    }

    for (int i = 1; i < M; i += 2) {
        for (int j = 1; j < N; j += 2) {
            this->maze[i][j] = ' ';
        }
    }
    Maze::createMaze(M, N);
    this->maze[0][1] = '#';
    this->maze[M - 1][N - 2] = 'E';
    for(int i=1; i< M-1; i++) {
        for (int j = 1; j < N-1; j++) {
            if(maze[i][j] == '#' and
                not((maze[i-1][j] == '#' and maze[i][j-1] == '#') or (maze[i-1][j] == '#' and maze[i][j+1] == '#')
                or (maze[i+1][j] == '#' and maze[i][j-1] == '#') or (maze[i+1][j] == '#' and maze[i][j+1] == '#'))) {
                auto chance = ((double) rand() / (RAND_MAX));
                if (chance<0.12) {
                    maze[i][j] = ' ';
                }
            }
        }
    }
    adj.resize(M * N);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == ' ') {
                if (i > 0) {
                    if (maze[i - 1][j] == ' ')
                        adj[i * N + j].push_back(std::make_pair(i - 1, j));
                }
                if (j > 0) {
                    if (maze[i][j - 1] == ' ')
                        adj[i * N + j].push_back(std::make_pair(i, j - 1));
                }
                if (i < M - 1) {
                    if (maze[i + 1][j] == ' ')
                        adj[i * N + j].push_back(std::make_pair(i + 1, j));
                }
                if (j < N - 1) {
                    if (maze[i][j + 1] == ' ')
                        adj[i * N + j].push_back(std::make_pair(i, j + 1));
                }
            }
        }
    }
}


int Maze::shortestPathBFS(std::pair<int, int> src, std::pair<int, int> dest) {
    std::pair<int, int> pred[M*N];
    int dist[M*N];
    if (BFS(src, dest, pred, dist) == false) {
        std::cout << "No path found\n";
        return -1;
    }
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> crawl = dest;
    path.push_back(crawl);
    while (pred[crawl.first*N+crawl.second] != std::make_pair(-1, -1)) {
        path.push_back(pred[crawl.first*N+crawl.second]);
        crawl = pred[crawl.first*N+crawl.second];
    }
    auto l = path.size()-2;
    if(path[l].first == src.first - 1 && path[l].second == src.second)
        return 3;
    if(path[l].first == src.first + 1 && path[l].second == src.second)
        return 1;
    if(path[l].first == src.first && path[l].second == src.second - 1)
        return 2;
    if(path[l].first == src.first && path[l].second == src.second + 1)
        return 0;
    return -1;
}

bool Maze::BFS(std::pair<int, int> src, std::pair<int, int> dest, std::pair<int, int> *pred, int *dist) {
    std::list<std::pair<int, int>> queue;
    bool visited[M*N];
    for (int i = 0; i < M*N; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        pred[i] = std::make_pair(-1, -1);
    }
    visited[src.first*N+src.second] = true;
    dist[src.first*N+src.second] = 0;
    queue.push_back(src);

    while (!queue.empty()) {
        std::pair<int, int> u = queue.front();
        int row = u.first*N;
        int col = u.second;
        queue.pop_front();
        for (int i = 0; i < adj[row+col].size(); i++) {
            int neighbor = adj[row+col][i].first*N+adj[row+col][i].second;
            if (visited[neighbor] == false) {
                visited[neighbor] = true;
                dist[neighbor] = dist[row+col] + 1;
                pred[neighbor] = u;
                queue.emplace_back(adj[row+col][i].first, adj[row+col][i].second);

                if (adj[row+col][i] == dest)
                    return true;
            }
        }
    }

    return false;
}
