#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

struct Instr {
    int id;
    std::string op;
    int arg;
};

struct Status {
    const int status;
    const int acc;
    const int pc;
};

Instr ParseInstruction(int line_no, const std::string& txt) {
    std::pair<std::string, std::string> parts = absl::StrSplit(txt, " ");
    int arg = 0;
    absl::SimpleAtoi(parts.second, &arg);
    return Instr {line_no, parts.first, arg};
}

std::vector<Instr> ReadProgram() {
    int line_no = 0;
    std::string line;
    std::vector<Instr> p;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }

        p.push_back(ParseInstruction(line_no, line));
        line_no++;
    }
    return p;
}

Status Run(int mutate_count, int acc, size_t pc, std::set<int> trace, std::vector<Instr> &prog) {
    std::cout << "prog" << std::endl;
    for (auto i : prog) {
        std::cout << i.id << " " << i.op << " " << i.arg << std::endl;
    }

    std::cout << std::endl << "trace" << std::endl;

    for (auto i : trace) {
        std::cout << i << std::endl;
    }

    std::cout << std::endl;

    for (;;) {
        if (pc >= prog.size()) {
            return {0, acc, pc};
        }

        auto cur = prog[pc];

        if (trace.find(cur.id) != trace.end()) {
            return {2, acc};
        }        

        if (cur.op != "acc" && mutate_count == 0) {

            // TODO(dape): we are current mutating the same row over and over again
            // we need to prevent this from happening, essentially we need to track
            // where we have already modified the program to ensure we don't do it
            // again ... doh :(

            Instr m = {cur.id, cur.op == "jmp" ? "nop" : "jmp", cur.arg};
            prog.at(pc) = m;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            auto s = Run(1, acc, pc, trace, prog);
            if (s.status == 0) {
                std::cout << "speculation ok" << std::endl;    
                return s;
            }
            std::cout << "speculation failed:" << s.status << std::endl;
            std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            prog[pc] = cur;
        }

        trace.insert(cur.id);
    
        std::cout << "pc:" << pc << " \t[" << cur.op << " " << cur.arg << "] acc:[" << acc << "]" << std::endl;

        if (cur.op == "nop") {            
            pc++;
        } else if (cur.op == "acc") {
            pc++;
            acc += cur.arg;
        } else if (cur.op == "jmp") {
            pc = pc + cur.arg;        
        } else {
            return {3, acc};
        }
    }    
}

Status Run(std::vector<Instr>& prog) {
    std::set<int> trace;
    return Run(0, 0, 0, trace, prog);
}

int main() {
    std::cout << "8" << std::endl;
    auto p = ReadProgram();
    std::cout << "program loaded:" << p.size() << "instructions" << std::endl;
    auto r = Run(p);
    std::cout << "status:" << r.status << "|acc:" << r.acc << std::endl;

    return 0;
}
