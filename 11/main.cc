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

bool CanOccupy(const Map& map, int row, int col) {
    for (int r = row - 1; r <= row + 1; r++) {
        //std::cout << r;
        if (r < 0 || r >= map.size()) {
            continue;
        }

        for (int c = col - 1; c <= col + 1; c++) {
            //std::cout << c;

            if (c < 0 || c >= map[r].size() || IsFloor(map, r, c)) {
                continue;
            }

            if (IsOccupied(map, r, c)) {
                return false;
            }
        }

        //std::cout << std::endl;
    }

    return true;
}

bool ShouldVacate(const Map& map, size_t row, size_t col) {
    int cnt = 0;    

    for (int r = (int)row - 1; r <= (int)row + 1; r++) {
        //std::cout << "r" << r;
        if (r < 0 || r >= map.size()) {
            continue;
        }

        for (int c = (int)col - 1; c <= (int)col + 1; c++) {
            //std::cout << c;
            if (r == row && c == col) {
                continue;
            }

            if (c < 0 || c >= map[r].size()) {
                continue;
            }            

            if (IsFloor(map, r, c)) {
                continue;
            }

            if (IsOccupied(map, r, c)) {
                cnt++;
            }

            if (cnt == 4) {
                //std::cout << cnt;
                return true;
            }
        }
    }
    //std::cout << cnt;
    return false;
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
