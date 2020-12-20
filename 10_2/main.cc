#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <deque>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

std::vector<std::string> ReadInput() {   
    std::vector<std::string> lines;
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        //std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}

std::vector<int> ReadAdapters() {
    auto txts = ReadInput();
    std::vector<int> adapters;
    for (auto &t : txts) {
        int max;
        absl::SimpleAtoi(t, &max);
        adapters.push_back(max);
    }
    return adapters;
}

int MaxAdapters(std::vector<int> &adapters, int n) {
    if (n == 1) {
        return 1;
    }

    return 0;
}

int get(std::unordered_map<int64_t, int64_t> &m, int key) {
    auto fit = m.find(key);
    if (fit == m.end()) {
        return 0;
    }

    return (*fit).second;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "10" << std::endl;
    auto adapters = ReadAdapters();
    std::cout << "adapters size:" << adapters.size() << std::endl;
    std::sort(adapters.begin(), adapters.end());
    adapters.push_back((*adapters.rbegin())+3);

    std::unordered_map<int64_t, int64_t> memo;
    memo[0] = 1;

    for (auto a : adapters) {
        auto max = memo[a-3] + memo[a-2] + memo[a-1];
        std::cout << memo[a-3] << std::endl;
        std::cout << memo[a-2] << std::endl;
        std::cout << memo[a-1] << std::endl;
        std::cout << max << std::endl;
        memo[a] = max;
        std::cout << std::endl;        
    }

    std::cout << memo[*adapters.rbegin()] << std::endl;

    return 0;
}
