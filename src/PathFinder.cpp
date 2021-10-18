#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;

enum class State { kEmpty, kObstacle, kClosed, kPath };
// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

string CellString(State cell) {
  return (cell == State::kEmpty) ? "0 " : "⛰️ ";
}

vector<State> ParseLine(string line) {
  istringstream stream(line);
  vector<State> row{};
  char c;
  int n;
  while (stream >> n >> c && c == ',')
    (!n) ? row.push_back(State::kEmpty) : row.push_back(State::kObstacle);
  return row;
}

vector<vector<State>> ReadBoardFile(string path) {
  ifstream file(path);
  vector<vector<State>> board{};
  if (file) {
    string line;
    while (getline(file, line))
      board.push_back(ParseLine(line));
  }
  return board;
}

void PrintBoard(vector<vector<State>> board) {

  for (auto row : board) {
    for (auto col : row)
      cout << CellString(col);
    cout << endl;
  }
}

/*******************************************************
 A* functions
*******************************************************/
bool Compare(vector<int> a, vector<int> b) {
  return (a[2] + a[3]) > (b[2] + b[3]);
}
void CellSort(vector<vector<int>> *v) { sort(v->begin(), v->end(), Compare); }
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist,
               vector<vector<State>> &grid) {
  vector<int> node{x, y, g, h};
  openlist.push_back(node);
  grid[x][y] = State::kClosed;
}
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  return ((x >= 0 && y >= 0) && (x < grid.size() && y < grid[0].size()) &&
          grid[x][y] == State::kEmpty);
}
void ExpandNeighbors(vector<int> &current, vector<vector<int>> &open,
                     vector<vector<State>> &grid, int goal[2]) {
  // TODO: ExpandNeighbors {

  // TODO: Get current node's data.
  int x1 = current[0];
  int y1 = current[1];
  int g = current[2];
  int h = 0;
  // TODO: Loop through current node's potential neighbors.
  for (auto neighbor : delta) {
    // TODO: Check that the potential neighbor's x2 and y2 values are on the
    // grid and not closed.
    int x2 = x1 + neighbor[0];
    int y2 = y1 + neighbor[1];
    if (CheckValidCell(y2, y2, grid)) {
      h = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g + 1, h, open, grid);
    }

    // TODO: Increment g value, compute h value, and add neighbor to open list.
  }
}
vector<vector<State>> Search(vector<vector<State>> grid, int start[2],
                             int goal[2]) {
  vector<vector<State>> path{};
  vector<vector<int>> open{};
  // Initialize open list for the first node
  int x = start[0];
  int y = start[1];
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, grid);

  // TODO: while open vector is non empty {
  while (!open.empty()) {

    // TODO: Sort the open list using `CellSort`, and get the current node.
    CellSort(&open);
    vector<int> currentNode = open.back();
    open.pop_back();

    // TODO: Get the x and y values from the current node,
    // and set grid[x][y] to kPath.
    grid[currentNode[0]][currentNode[1]] = State::kPath;
    // TODO: Check if you've reached the goal. If so, return grid.
    if (currentNode[0] == goal[0] && currentNode[1] == goal[1])
      return grid;
    // If we're not done, expand search to current node's neighbors. This step
    // will be completed in a later quiz. ExpandNeighbors

  } // TODO: End while loop

  cout << "No path found!" << endl;
  return path;
}

int main() {

  // vector<vector<int>> board = {{0, 1, 0, 0, 0, 0},
  //                             {0, 1, 0, 0, 0, 0},
  //                             {0, 1, 0, 0, 0, 0},
  //                             {0, 1, 0, 0, 0, 0},
  //                             {0, 0, 0, 0, 1, 0}};
  auto board = ReadBoardFile("1.board");
  // PrintBoard(board);
  int start[2]{0, 0};
  int goal[2]{4, 5};
  auto solution = Search(board, start, goal);
  PrintBoard(solution);
}