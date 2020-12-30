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
    uint64_t position;
};


int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "--- 15 ---" << std::endl;
    auto input = ReadInput();    

    std::unordered_map<uint64_t, History> spoken;
    uint64_t n = 1;
    int last = 0;
    int input_cur = 0;
    
    for (;;) {
        if (input_cur < input.size()) {
            last = input[input_cur];
            input_cur++;
        }

        if (n == absl::GetFlag(FLAGS_n)) {
            std::cout << n << " : " << last << std::endl;   
            std::cout << std::endl << "DIE" << last << std::endl;
            break;
        }

        //std::cout << n << " : " << last << std::endl;        
        if (n % 100000 == 0) {
            std::cout << ".";
        }


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
    }
    
    return 0;
}
