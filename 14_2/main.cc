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
    uint64_t address;
    uint64_t value;
};

class VM {

    WriteInstr ParseWrite(const std::string&line) {
        auto p0 = line.substr(4);        

        auto end = p0.find("]", 0);
        auto address_txt = p0.substr(0, end);        

        auto value_txt = p0.substr(end+4);

        uint64_t address;
        uint64_t value;

        absl::SimpleAtoi(address_txt, &address);
        absl::SimpleAtoi(value_txt, &value);

        return {address, value};
    }

    std::string Expand(uint64_t address) {
        std::string res;
        uint64_t tmp = address;
        for (int i = 0; i < 36; i++) {
            char m = _mask[_mask.size() - 1 - i];
            bool b0 = 1UL & tmp;

            if (b0) {
                //std::cout << 1;
            } else {
                //std::cout << 0;
            }

            if (m == '0') {
                res.insert(0, b0 ? "1" : "0");
            } else if (m == '1') {
                res.insert(0, "1");
            } else {
                res.insert(0, "X");
            }

            tmp = tmp >> 1;
        }
        return res;
    }    

    void Write(const WriteInstr &i) {
        std::cout << "write: " << i.address << " val:" << i.value << std::endl;
        auto address_template = Expand(i.address);
        std::cout << "expanded: " << address_template << std::endl;
        std::vector<uint64_t> dests;
        dests.push_back(0);
        for (int i = 0; i < 36; i++) {
            char c = address_template[i];
            if (c == '0') {
                for (int i = 0; i < dests.size(); i++) {
                    dests[i] = dests[i] << 1;
                }
            } else if (c == '1') {
                for (int i = 0; i < dests.size(); i++) {
                    dests[i] = ((dests[i] << 1) | 1UL);
                }
            } else {
                auto initial = dests.size();
                for (int i = 0; i < initial; i++) {
                    dests.push_back(dests[i] << 1);
                    dests[i] = ((dests[i] << 1) | 1UL);
                }
            }
        }

        std::cout << "expanded: " << dests.size() << std::endl;
        for (auto dest : dests) {
            _mem[dest] = i.value;
        }
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
    std::unordered_map<uint64_t, uint64_t> _mem;
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
