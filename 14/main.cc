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

struct WriteInstr {
    int address;
    uint64_t value;
};

class VM {

    WriteInstr ParseWrite(const std::string&line) {
        auto p0 = line.substr(4);        

        auto end = p0.find("]", 0);
        auto address_txt = p0.substr(0, end);        

        auto value_txt = p0.substr(end+4);

        int address;
        uint64_t value;

        absl::SimpleAtoi(address_txt, &address);
        absl::SimpleAtoi(value_txt, &value);

        return {address, value};
    }

    void Write(const WriteInstr &i) {
        std::cout << "write: " << i.address << " val:" << i.value << std::endl;

        uint64_t val = i.value;
        for (int n = 0; n < 36; n++) {
            auto c = _mask[_mask.size()-1-n];
            //std::cout << n << ": " << val << std::endl;
            if (c == 'X') {
            } else if (c == '0') {
                //std::cout << "0 " << val << std::endl;
                val &= ~(1UL << n);
            } else if (c == '1') {
                //std::cout << "1" << std::endl;
                val |= 1UL << n;
            }
        }

        std::cout << i.value << " --> " << val << std::endl;
        _mem[i.address] = val;
    }

  public:
    void Process(const std::string &line) {
        std::cout << line << std::endl;
        if (line.find("mask", 0) == 0) {
            auto new_mask = line.substr(7);
            std::cout << "mask " << _mask << " --> " << new_mask << " len:" << new_mask.size() << std::endl;
            _mask = new_mask;
        } else if (line.find("mem", 0) == 0) {
            Write(ParseWrite(line));
        } else {
            std::cout << line << std::endl;
        }
    }

    uint64_t Sum() {
        uint64_t sum = 0;
        for (auto &kv : _mem) {
            sum += kv.second;
        }
        return sum;
    }

  private:
    std::string _mask;
    std::unordered_map<int, uint64_t> _mem;
};

void ProcessInput() {    
    std::string line;
    VM m;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        //std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
        
        m.Process(line);
    }

    std::cout << "sum: " << m.Sum() << std::endl;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "14" << std::endl;
    ProcessInput();

    return 0;
}
