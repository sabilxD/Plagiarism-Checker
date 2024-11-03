#include <iostream>
#include <queue>
#include <vector>
#include <stack>

using namespace std;

struct Cell {
    int x, y;
};

bool isValid(vector<vector<int>>& maze, vector<vector<bool>>& visited, int x, int y) {
    int rows = maze.size();
    int cols = maze[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == 0 && !visited[x][y]);
}

vector<Cell> bfs(vector<vector<int>>& maze, Cell start, Cell end) {
    int rows = maze.size();
    int cols = maze[0].size();

    // Directions for moving up, down, left, right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Queue for BFS
    queue<Cell> q;
    q.push(start);

    // Visited array
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    visited[start.x][start.y] = true;

    // Path tracking (to backtrack and reconstruct the path)
    vector<vector<Cell>> parent(rows, vector<Cell>(cols));

    while (!q.empty()) {
        Cell curr = q.front();
        q.pop();

        // If we reached the end
        if (curr.x == end.x && curr.y == end.y) {
            vector<Cell> path;
            for (Cell at = end; !(at.x == start.x && at.y == start.y); at = parent[at.x][at.y]) {
                path.push_back(at);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore all 4 directions
        for (int i = 0; i < 4; i++) {
            int newX = curr.x + directions[i][0];
            int newY = curr.y + directions[i][1];

            if (isValid(maze, visited, newX, newY)) {
                q.push({newX, newY});
                visited[newX][newY] = true;
                parent[newX][newY] = curr;
            }
        }
    }

    // Return an empty path if no solution
    return {};
}

int main() {
    vector<vector<int>> maze = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };

    Cell start = {0, 0}; // Start position (top-left)
    Cell end = {4, 4};   // End position (bottom-right)

    vector<Cell> path = bfs(maze, start, end);

    if (!path.empty()) {
        cout << "Path found:" << endl;
        for (const auto& cell : path) {
            cout << "(" << cell.x << ", " << cell.y << ") ";
        }
        cout << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
    