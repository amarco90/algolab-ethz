#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

struct node {
  int x, y;   // position
  int sx, sy; // speed
  int d;      // depth
};

int X, Y;
int end_x, end_y;
bool visited[30][30][7][7];
queue<node> frontier;
int hops;

void clear(queue<node> &q) {
  queue<node> empty;
  swap(q, empty);
}

// just to stay inside the bounds of the array
int speed(int s) {
  if      (s == -1) { s = 4; }
  else if (s == -2) { s = 5; }
  else if (s == -3) { s = 6; }
  return s;
}

void bfs() {
  while (!frontier.empty()) {
    node extracted = frontier.front();
    frontier.pop();
    // insert new states here
    for (int dx = -1; dx != 2; ++dx) {
      for (int dy = -1; dy != 2; ++dy) {
        node tmp = extracted;
        tmp.sx += dx; tmp.sy += dy;
        int newx = tmp.x + tmp.sx, newy = tmp.y + tmp.sy;
        if (tmp.sx >= -3 && tmp.sx <= 3 && tmp.sy >= -3 && tmp.sy <= 3
            && newx >= 0 && newx < X && newy >= 0 && newy < Y
            && !visited[newx][newy][speed(tmp.sx)][speed(tmp.sy)]) {
          visited[newx][newy][speed(tmp.sx)][speed(tmp.sy)] = true;
          ++tmp.d;
          if (newx == end_x && newy == end_y) {
            hops = tmp.d;
            return;
          }
          tmp.x = newx; tmp.y = newy;
          frontier.push(tmp);
        }
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    memset(visited, false, 30 * 30 * 7 * 7);
    clear(frontier);

    node start;
    start.sx = start.sy = start.d = 0;

    int P;
    cin >> X >> Y;
    cin >> start.x >> start.y >> end_x >> end_y;
    cin >> P;
    for (int p = 0; p < P; ++p) {
      int px1, py1, px2, py2;
      cin >> px1 >> py1 >> px2 >> py2;
      for (int i = px1; i <= px2; ++i)
        for (int j = py1; j <= py2; ++j)
          memset(visited[i][j], 1, 7 * 7);
    }
    visited[start.x][start.y][0][0] = true;
    frontier.push(start);
    hops = (start.x == end_x && start.y == end_y) ? 0 : dfs();
    if (hops == -1)
      cout << "No solution." << endl;
    else
      cout << "Optimal solution takes " << hops << " hops." << endl;
  }
  return 0;
}
