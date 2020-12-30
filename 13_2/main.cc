#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>    
#include <deque>
#include <numeric>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

ABSL_FLAG(bool, step, false, "require enter for every line");
ABSL_FLAG(uint64_t, start, 0, "initial time to start from");

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

// each element is a bus ID or -1
struct Departure {
    int bus;
    int offset;
};

struct Constraints {
    std::vector<Departure> deps;
    int count;
};

Constraints ParseConstraints(const std::vector<std::string> all) {
    int cnt = 0;
    std::vector<int> ids;
    for (auto &bus : all) {        
        if (bus != "x") {
            int id = 0;
            absl::SimpleAtoi(bus, &id);
            ids.push_back(id);
            cnt++;
        } else {
            ids.push_back(-1);
        }        
    }

    std::vector<Departure> deps;

    for (size_t i = 0; i < ids.size(); i++) {
        int id = ids[i];
        if (id < 0) {
            continue;
        }

        deps.push_back({id, (int)i});
    }

    std::sort(deps.begin(), deps.end(), [](Departure &d1, Departure &d2) {
        return d1.bus > d2.bus;
    });

    return {deps, cnt};
}

// Coprime: two integers that only have 1 as common divisor
// Ex. 14 and 25 are coprime
// Ex. 14 and 21 are not coprime (can be divided by 7)
//
// Euclidean division: division to get quotient and a remainder.
// Getting only the remainder is normal module division.
// Ex. 9 / 4 => qoutient = 2 and remainder = 1
//
// Congruent modulo: two values that have the same remainder.
// Ex. 37 and 57 are congruent modulo 10 since both have a remainder of 7.
// 37 % 10 = 7 and 57 % 10 = 7
//



int Scan(const Constraints &c) {
    uint64_t prev = -1;
    uint64_t cur = 0;
    uint64_t steps = 0;    
    for (;;) {
        prev = cur;

        auto b0 = c.deps[0];
        if (cur == 0) {
            // This is strange, if I use the example with this line active the
            // I will not hit the line from the example...
            // Reading through the instructions again I think I got it wrong,
            // all busses still start at t = 0. So then this line should not
            // be active.            
            //cur += b0.offset;
        }
        cur += b0.bus;

        if (cur < prev) {
            std::cout << "roll over " << cur << "|" << prev << std::endl;
            std::cout << b0.bus << std::endl;
            std::cout << steps << std::endl;
            exit(1);
        }        

        bool trace = (cur > 1068779) && (cur < 1068792);
        int found = 0;

        if (trace) {
            std::cout << ".";
        }

        for (size_t i = 1; i < c.deps.size(); i++) {
            steps++;

            if (cur == 1068785) {
                std::cout << "HIT" << std::endl;
            }

            int at = cur - b0.offset + c.deps[i].offset;
            if (trace) {
                std::cout << "checking:" << at;
            }

            if (at % c.deps[i].bus != 0) {
                //std::cout << std::endl;
                break;
            }
            
            found++;
            //std::cout << "X" << found << std::endl;
        }

        if (steps % 10000000 == 0) {
            std::cout << "." << steps << std::endl;
        }
        
        if (found+1 == c.deps.size()) {
            std::cout << found << "|" << cur << std::endl;
            std::cout << (cur - b0.offset) << std::endl;
            std::cout << steps << std::endl;
            exit(1);
        }
    }

    return -1;
}

int AlignPeriods(int a, int b) {
    int min = std::min(a, b);
    int max = std::max(a, b);
    if (max % min == 0) {
        return max;
    }    

    return a * b;
}

int SearchAlignPeriods(int a, int b) {
    for (int i = 1; i < 1000; i++) {        
        if (i % a == 0 && i % b == 0) {
            return i;
        }        
    }

    exit(1);
    return 0;
}

uint64_t NextDepartureTime(uint64_t target, uint64_t bus) {
    // # of full periods the bus can make it within the target time.
    uint64_t q = target / bus;
    // # of minutes that past the last full period
    uint64_t rem = target % bus;
    if (rem > 0) {
        //...fraction remains, we need one more full period
        q++;
    }
    return q * bus;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "13" << std::endl;
    auto input = ReadInput();
    
    std::vector<std::string> busses = absl::StrSplit(input[1], ",");
    auto constraints = ParseConstraints(busses);
    for (auto &dep : constraints.deps) {
        std::cout << dep.bus << "|" << dep.offset << std::endl;
    }
        
    uint64_t cur = 0;
    uint64_t start = absl::GetFlag(FLAGS_start);
    if (start != 0) {
        cur = NextDepartureTime(start, (*constraints.deps.begin()).bus);
    }
    std::cout << "initial time: " << cur << " bus: " << constraints.deps[0].bus << std::endl;
    // /uint64_t cur = 0;
    uint64_t step = 0;
    for (;;) {
        step++;
        std::vector<int> found;
        Departure d0 = constraints.deps[0];
        found.push_back(d0.bus);        

        if (cur == 1068785) {
            std::cout << "HIT" << std::endl;
        }

        for (size_t i = 1; i < constraints.deps.size(); i++) {
            Departure dep = constraints.deps[i];
            uint64_t target = cur - d0.offset + dep.offset;
            uint64_t next = NextDepartureTime(target, dep.bus);
            if (next != target) {
                break;
            }

            found.push_back(dep.bus);
        }

        if (step % 10000000 == 0) {
            std::cout << "." << cur << std::endl;
        }

        if (found.size() == constraints.deps.size()) {
            std::cout << "time: " << cur - d0.offset << std::endl;
            std::cout << "found: " << found.size() << std::endl;
            for (auto bus : found) {
                std::cout << bus << " ";
            }
            std::cout << std::endl;
            exit(0);
        }

        cur += d0.bus;
    }
    
    return 0;
}
