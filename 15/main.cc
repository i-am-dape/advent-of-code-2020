#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

ABSL_FLAG(int, n, 2020, "the turn to stop at");

std::vector<int> ReadInput() {    
    std::string line;
    std::vector<int> input;

    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        
        int in = 0;
        absl::SimpleAtoi(line, &in);

        input.push_back(in);
    }

    return input;
}

struct History {
    int position;
};


int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "--- 15 ---" << std::endl;
    auto input = ReadInput();    

    std::unordered_map<int, History> spoken;
    int n = 0;
    int last = 0;
    int input_cur = 0;
    
    for (;;) {
        if (input_cur < input.size()) {
            last = input[input_cur];
            input_cur++;
        }

        std::cout << n << " : " << last << std::endl;

        auto fit = spoken.find(last);
        if (fit == spoken.end()) {
            // first time
            spoken[last] = History{n};
            last = 0;            
        } else {
            // previous            
            auto h = *fit;
            last = n - h.second.position;
            h.second.position = n;
            spoken[h.first] = h.second;
        }

        n++;

        if (n == absl::GetFlag(FLAGS_n)) {
            std::cout << last << std::endl;
            break;
        }
    }
    
    return 0;
}
