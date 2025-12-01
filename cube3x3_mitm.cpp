/*
This program solves a Rubik's cube using the meet-in-the-middle algorithm.
It performs two searches:
1. A "forward" search from the solved state.
2. A "backward" search from the scrambled state.
When the two searches find a common cube state, a solution is found by combining the paths.

Usage: ./<executable_name> <forward_depth> <backward_depth>
Example: ./a.out 7 8
*/

#include <bits/stdc++.h>
using namespace std;

string solved_cube =
               "C""C""C"
               "C"   "C"
               "C""C""C"

   "B""B""B"   "A""A""A"   "E""E""E"   "F""F""F"
   "B"   "B"   "A"   "A"   "E"   "E"   "F"   "F"
   "B""B""B"   "A""A""A"   "E""E""E"   "F""F""F"

               "D""D""D"
               "D"   "D"
               "D""D""D";


void print_cube(string & s) {
  printf(
    "         ---------\n"
    "         | %c %c %c |\n"
    "         | %c C %c |\n"
    "         | %c %c %c |\n"
    " ---------------------------------\n"
    " | %c %c %c | %c %c %c | %c %c %c | %c %c %c |\n"
    " | %c B %c | %c A %c | %c E %c | %c F %c |\n"
    " | %c %c %c | %c %c %c | %c %c %c | %c %c %c |\n"
    " ---------------------------------\n"
    "         | %c %c %c |\n"
    "         | %c D %c |\n"
    "         | %c %c %c |\n"
    "         ---------\n", 
    s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15], s[16], s[17], s[18], s[19], s[20], s[21], s[22], s[23], s[24], s[25], s[26], s[27], s[28], s[29], s[30], s[31], s[32], s[33], s[34], s[35], s[36], s[37], s[38], s[39], s[40], s[41], s[42], s[43], s[44], s[45], s[46], s[47]
  );
}

// sticker indices that move for each of the 6 faces.
const int MOVES[6][5][4] = {
  {{11, 13, 33, 31}, {12, 23, 32, 22}, { 5, 14, 42, 30}, { 6, 24, 41, 21}, { 7, 34, 40, 10}}, // face A (Front)
  {{ 8, 10, 30, 28}, { 9, 21, 29, 20}, { 0, 11, 40, 39}, { 3, 22, 43, 27}, { 5, 31, 45, 19}}, // face B (Left)
  {{ 0,  2,  7,  5}, { 1,  4,  6,  3}, {19, 16, 13, 10}, {18, 15, 12,  9}, {17, 14, 11,  8}}, // face C (Up)
  {{40, 42, 47, 45}, {41, 44, 46, 43}, {31, 34, 37, 28}, {32, 35, 38, 29}, {33, 36, 39, 30}}, // face D (Down)
  {{14, 16, 36, 34}, {15, 25, 35, 24}, { 7, 17, 47, 33}, { 4, 26, 44, 23}, { 2, 37, 42, 13}}, // face E (Right)
  {{17, 19, 39, 37}, {18, 27, 38, 26}, { 2,  8, 45, 36}, { 1, 20, 46, 25}, { 0, 28, 47, 16}}  // face F (Back)
};
const string FACE_NAMES[] = {"F", "L", "U", "D", "R", "B"};

// Applies a single clockwise turn to a face.
void turn_face(string & cube, int face) {
  for (int i = 0; i < 5; ++i) {
    char temp = cube[MOVES[face][i][3]];
    cube[MOVES[face][i][3]] = cube[MOVES[face][i][2]];
    cube[MOVES[face][i][2]] = cube[MOVES[face][i][1]];
    cube[MOVES[face][i][1]] = cube[MOVES[face][i][0]];
    cube[MOVES[face][i][0]] = temp;
  }
}

// initial scrambled cube
string scrambled_cube;
void setup_scrambled_cube(){
  int scramble_moves[18][2] = {
    {2, 2}, {0, 1}, {1, 2}, {2, 2}, {4, 2}, {0, 1}, {1, 2}, {0, 2}, {1, 3}
  };

  scrambled_cube = solved_cube;

  for (int i = 0; i < 18; ++i) {
    for (int t = 0; t < scramble_moves[i][1]; ++t) {
      turn_face(scrambled_cube, scramble_moves[i][0]);
    }
  }
}

// Inverts the sequence of moves from the backward search to get the correct solution path.
string invert_moves(string path) {
    stringstream ss(path);
    string move;
    vector<string> moves;
    while (ss >> move) {
        moves.push_back(move);
    }
    reverse(moves.begin(), moves.end());
    string inverted_path = "";
    for (const auto& m : moves) {
        if (m.length() == 1) {
            inverted_path += m + "' ";
        } else if (m[1] == '\'') {
            inverted_path += m[0];
            inverted_path += " ";
        } else { // It's a "2" move
            inverted_path += m + " ";
        }
    }
    return inverted_path;
}

long long g_counter = 0;

// Recursively explores cube states up to a maximum depth.
bool search(
  string & cube,
  int depth,
  int max_depth,
  int last_face,
  string current_path,
  unordered_map<string, string> & visited,
  bool is_forward_search,
  unordered_map<string, string> & other_map
) {

  g_counter++;

  // Store the path to the current state, avoiding longer paths to the same state.
  if (visited.count(cube) && visited[cube].length() <= current_path.length()) {
      return false;
  }
  visited[cube] = current_path;

  // If this is the backward search, check if we've met the forward search.
  if (!is_forward_search) {
    if (other_map.count(cube)) {
      cout << "Found a meeting point!" << endl;
      print_cube(cube);
      string path1 = other_map[cube];
      string path2 = current_path;
      cout << "Solution: " << path1 << invert_moves(path2) << endl;
      return true;
    }
  }

  // Continue searching until max depth is reached.
  if (depth < max_depth) {
    for (int face = 0; face < 6; ++face) {
      // Small optimization: avoid redundant move sequences like F F' or U D U (if U D was already explored).
      if (face == last_face || (face < last_face && (face + last_face == 5))) {
        continue;
      }
      string temp_cube = cube;
      string temp_path = current_path;
      // Try 1, 2, and 3 turns for the current face.
      for (int i = 1; i <= 3; ++i) {
        turn_face(temp_cube, face);
        string move_str;
        if (i == 1) move_str = FACE_NAMES[face];
        if (i == 2) move_str = FACE_NAMES[face] + "2";
        if (i == 3) move_str = FACE_NAMES[face] + "'";
        
        bool found = search(temp_cube, depth + 1, max_depth, face, temp_path + move_str + " ", visited, is_forward_search, other_map);
        if (!is_forward_search && found) {
          return true;
        }
      }
    }
  }
  return false;
}

int main(int argc, char * argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <depth1> <depth2>\n";
    return 1;
  }

  int depth1 = atoi(argv[1]);
  int depth2 = atoi(argv[2]);

  if (depth1 < 0 || depth2 < 0) {
    cerr << "Depths must be positive.\n";
    return 1;
  }

  setup_scrambled_cube();

  // Phase 1: Forward search from the solved cube.
  string current_cube = solved_cube;
  unordered_map<string, string> visited1;
  unordered_map<string, string> dummy;

  search(current_cube, 0, depth1, -1, "", visited1, true, dummy);

  cout << "Forward search finished. Visited " << visited1.size() << " states." << endl;

  // Phase 2: Backward search from the scrambled cube.
  current_cube = scrambled_cube;
  unordered_map<string, string> visited2;

  bool found = search(current_cube, 0, depth2, -1, "", visited2, false, visited1);

  if (!found) {
    cout << "No solution found." << endl;
  }

  cout << "Total states explored: " << g_counter << endl;

  return 0;
}