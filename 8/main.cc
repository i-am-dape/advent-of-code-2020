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
    const std::string id;
    const std::string op;
    const int arg;
};

struct Status {
    const int status;
    const int acc;
};

Instr ParseInstruction(int id, const std::string& txt) {
    std::pair<std::string, std::string> parts = absl::StrSplit(txt, " ");
    int arg = 0;
    absl::SimpleAtoi(parts.second, &arg);
    auto txt_id = absl::StrCat(id, ":", txt);
    return Instr {txt_id, parts.first, arg};
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

Status RunUntilRepeat(const std::vector<Instr>& prog) {
    std::set<std::string> trace;
    int acc = 0;
    size_t pc = 0;

    for (;;) {
        if (pc >= prog.size()) {
            return {1, acc};
        }

        auto cur = prog[pc];
        std::cout << "pc:" << pc << " \t[" << cur.op << " " << cur.arg << "]" << std::endl;

        if (trace.find(cur.id) != trace.end()) {
            return {2, acc};
        }

        trace.insert(cur.id);

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
    return {0, acc};
}

int main() {
    std::cout << "8" << std::endl;
    auto p = ReadProgram();
    std::cout << "program loaded:" << p.size() << "instructions" << std::endl;
    auto r = RunUntilRepeat(p);
    std::cout << "status:" << r.status << "|acc:" << r.acc << std::endl;

    return 0;
}
