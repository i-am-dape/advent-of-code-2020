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

ABSL_FLAG(bool, step, false, "require enter for every line");

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

// all started at t0 = time = zero
// 1:   start from sea port (sp)
// 2:   drive to airport (ap)
// 3:   drive to N various locations N
// 3+N: drive to sp
// 4:   goto 1
// - the time of this loop is the ID of the bus
// - the ID is their departure time in minutes from sp
// - ex: ID=5 => 0, 5, 10, 15
// - ex: ID=11 => 0, 11, 22, 33

std::vector<int> InTraffic(const std::vector<std::string> all) {
    std::vector<int> in_traffic;
    for (auto &bus : all) {
        if (bus == "x") {
            continue;
        }            

        int id = 0;
        absl::SimpleAtoi(bus, &id);
        in_traffic.push_back(id);
    }

    std::sort(in_traffic.begin(), in_traffic.end());
    return in_traffic;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "13" << std::endl;
    auto input = ReadInput();
    int depart_zero = 0;
    absl::SimpleAtoi(input[0], &depart_zero);
    std::cout << "depart 0:" << depart_zero << std::endl;
    std::vector<std::string> busses = absl::StrSplit(input[1], ",");
    std::vector<int> in_traffic = InTraffic(busses);    
    for (auto id : in_traffic) {
        std::cout << id << std::endl;
    }

    int min = 60;
    std::unordered_map<int, int> time_to_wait;
    for (auto id : in_traffic) {
        auto wait_time = id - (depart_zero % id);
        time_to_wait[wait_time] = id;
        if (wait_time < min) {
            min = wait_time;
        }
    }

    std::cout << min << ":" << time_to_wait[min] << std::endl;    
    std::cout << min * time_to_wait[min] << std::endl;
    return 0;
}
