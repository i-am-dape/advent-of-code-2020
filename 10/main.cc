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

std::vector<std::string> ReadInput() {   
    std::vector<std::string> lines;
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}

std::set<int> ReadAdapters() {
    auto txts = ReadInput();
    std::set<int> adapters;
    for (auto &t : txts) {
        int max;
        absl::SimpleAtoi(t, &max);
        adapters.insert(max);
    }
    return adapters;
}

int FindAdapter(int cur, std::set<int> &adapters) {
    for (int i = 1; i <= 3; i++) {
        int candidate = cur + i;
        auto removed = adapters.erase(candidate);
        if (removed == 1) {
            //std::cout << removed << std::endl;
            return candidate;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "10" << std::endl;
    auto adapters = ReadAdapters();
    std::cout << "adapters size:" << adapters.size() << std::endl;
    std::unordered_map<int, int> diffs;

    int cur = 0;
    while (!adapters.empty()) {
        auto adapter = FindAdapter(cur, adapters);
        std::cout << "c:" << cur << "=> a:" << adapter << " : " << adapters.size() << std::endl;
        if (adapter < 0) {
            std::cerr << "failed!" << std::endl;
            return 1;
        }

        int diff = adapter - cur;
        diffs[diff]++;
        cur = adapter;
    }

    int max = cur + 3;
    diffs[3]++;
    std::cout << "max:" << max << std::endl;


    for (auto kv : diffs) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    std::cout << diffs[1] * diffs[3] << std::endl;

    return 0;
}
