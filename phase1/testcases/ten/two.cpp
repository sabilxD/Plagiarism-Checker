#include <iostream>
#include <vector>

using namespace std;

struct Cell {
    int x, y;
};

// Check if the cell is valid (within bounds, not a wall, and not visited)
bool isValid(vector<vector<int>>& maze, vector<vector<bool>>& visited, int x, int y) {
    int rows = maze.size();
    int cols = maze[0].size();
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == 0 && !visited[x][y]);
}

// Perform DFS to find a path from the start to the end
bool dfs(vector<vector<int>>& maze, vector<vector<bool>>& visited, Cell curr, Cell end, vector<Cell>& path) {
    // If we reach the end, add it to the path and return true
    if (curr.x == end.x && curr.y == end.y) {
        path.push_back(curr);
        return true;
    }

    // Mark the current cell as visited
    visited[curr.x][curr.y] = true;

    // Directions for moving up, down, left, right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Explore all 4 directions
    for (int i = 0; i < 4; i++) {
        int newX = curr.x + directions[i][0];
        int newY = curr.y + directions[i][1];

        if (isValid(maze, visited, newX, newY)) {
            // Recursively try to find a path
            if (dfs(maze, visited, {newX, newY}, end, path)) {
                path.push_back(curr);  // If a path is found, add the current cell to the path
                return true;
            }
        }
    }

    return false;  // No path found from this cell
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

    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
    vector<Cell> path;

    if (dfs(maze, visited, start, end, path)) {
        cout << "Path found:" << endl;
        reverse(path.begin(), path.end());  // Reverse the path to start from the beginning
        for (const auto& cell : path) {
            cout << "(" << cell.x << ", " << cell.y << ") ";
        }
        cout << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
    