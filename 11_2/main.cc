#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

ABSL_FLAG(int, max_cycles, 100, "max cycles to try to find a stable solution");

using Map = std::vector<std::vector<char>>;

Map ReadInput() {   
    Map m;
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        //std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }

        std::vector<char> row;
        for (auto c : line) {
            row.push_back(c);
        }

        m.push_back(row);
    }
    return m;
}

bool IsEmpty(const Map& m, int row, int col) {
    return m[row][col] == 'L';
}

bool IsFloor(const Map& m, int row, int col) {
    return m[row][col] == '.';
}

bool IsOccupied(const Map& m, int row, int col) {
    return m[row][col] == '#';
}

void TraceMaps(const Map& m1, const Map& m2) {
    std::cout << "map sizes:" << m1.size() << " : " << m2.size() << std::endl;
    for (size_t r = 0; r < m1.size(); r++) {        
        std::cout << (r < 10 ? "0" : "") << r << "  ";

        for (size_t c = 0; c < m1[r].size(); c++) {
            std::cout << m1[r][c];
        }

        std::cout << " | ";

        for (size_t c = 0; c < m2[r].size(); c++) {
            std::cout << m2[r][c];
        }

        std::cout << std::endl;
    }
}

std::vector<bool> Scan(const Map& map, int row, int col) {
    //  movement deltas (row, col)
    //
    //  (-1, -1) (-1,  0) (-1,  1)
    //  ( 0, -1)     x    ( 0,  1)
    //  ( 1, -1) ( 1,  0) ( 1,  1)  
    std::vector<std::pair<int, int>> movements = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1}, {0, 1},
        {1, -1}, {1, 0}, {1, 1},
    };

    std::vector<bool> occupied;    
    int scan_count = 0;

    for (auto &m : movements) {
        int r = row;
        int c = col;

        for (;;) {
            scan_count++;
            r += m.first;
            c += m.second;
            //std::cout << r << " | " << c << "{" << m.first << ", " << m.second << "}" << std::endl;

            if (r < 0 || r >= (int)map.size()) {
                //std::cout << "r break" << std::endl;
                break;
            }

            if (c < 0 || c >= (int)map[r].size()) {
                //std::cout << "c break" << std::endl;
                break;
            }
            
            char cur = map[r][c];
            //std::cout << c << ":" << map[0][2] << std::endl;
            if (cur == 'L') {
                occupied.push_back(false);
                break;
            } else if (cur == '#') {
                occupied.push_back(true);
                break;
            } else if (cur != '.') {
                std::cerr << "unexpected: " << c << std::endl;
                exit(1);
            }
        }
    }

    //std::cout << "scan:" << scan_count << std::endl;

    // std::cout << occupied.size() << ":";
    // for (auto occ : occupied) {
    //     std::cout << occ << " ";
    // }
    // std::cout << std::endl;
    return occupied;
}

bool CanOccupy(const Map& map, int row, int col) {
    auto occupied = Scan(map, row, col);
    auto fit = std::find(occupied.begin(), occupied.end(), true);
    return fit == occupied.end();
}

bool ShouldVacate(const Map& map, size_t row, size_t col) {
    auto occupied = Scan(map, row, col);
    auto cnt = std::count(occupied.begin(), occupied.end(), true);
    return cnt >= 5;
}

bool IsSame(const Map &m1, const Map &m2) {
    for (size_t r = 0; r < m1.size(); r++) {        
        for (size_t c = 0; c < m1[r].size(); c++) {
            if (m1[r][c] != m2[r][c]) {
                std::cout << m1[r][c] << " " << m2[r][c] << std::endl;
                return false;
            }
        }    
    }

    return true;
}

int CountOccupied(const Map& map) {
    int occupied = 0;
    for (size_t r = 0; r < map.size(); r++) {        
        for (size_t c = 0; c < map[r].size(); c++) {
            if (IsOccupied(map, r, c)) {
                occupied++;
            }
        }    
    }
    return occupied;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "11" << std::endl;

    Map m = ReadInput();

    std::cout << m.size() << " x " << m[0].size() << std::endl;

    Map n = m;

    int max = absl::GetFlag(FLAGS_max_cycles);
    int i = 0;
    bool stable = false;
    for (i = 0; !stable && i < max; i++) {
        for (size_t r = 0; r < m.size(); r++) {
            for (size_t c = 0; c < m[r].size(); c++) {
                if (IsFloor(m, r, c)) {
                    continue;
                }

                if (IsEmpty(m, r, c)) {
                    if (CanOccupy(m, r, c)) {
                        n[r][c] = '#';
                    }
                    continue;
                }

                if (IsOccupied(m, r, c)) {
                    //std::cout << r << "," << c << " | ";
                    if (ShouldVacate(m, r, c)) {
                        n[r][c] = 'L';
                    }
                    //std::cout  << m[r][c] << " > " << n[r][c] << std::endl;
                    continue;
                }
            }
        }

        TraceMaps(m, n);

        stable = IsSame(m, n);

        m = n;
    }

    if (i == max) {
        std::cerr << "ERROR: max count reached" << std::endl;        
    }

    std::cout << "count:" << i << std::endl;
    std::cout << "max:" << max << std::endl;
    std::cout << "occupied:" << CountOccupied(m) << std::endl;
    
    return 0;
}
