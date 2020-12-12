#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"


void trace(std::set<std::string> &words) {
    for (auto &w : words) {
        std::cout << w << "|";
    }
    std::cout << std::endl;
}

void trace(std::vector<absl::string_view> &words) {
    for (absl::string_view &w : words) {
        std::cout << w << "|";
    }
    std::cout << std::endl;
}

void trace(std::vector<std::string> &words) {
    for (std::string &w : words) {
        std::cout << w << "|";
    }
    std::cout << std::endl;
}

void trace(std::pair<std::string, std::string> words) {
    std::cout << words.first << std::endl;
    std::cout << words.second << std::endl;
}

std::vector<std::pair<int, std::string>> ParseContent(std::string txt_content) {
    std::cout << txt_content << ":" << std::endl;
    std::vector<std::pair<int, std::string>> content;

    for (auto &bag_spec : absl::StrSplit(txt_content, ",")) {
        std::vector<std::string> spec_parts = absl::StrSplit(bag_spec, " ");
        trace(spec_parts);
        std::cout << "spec:" << spec_parts.size() << "|" << bag_spec << "|" << std::endl;                    
        
        int cnt = 0;
        absl::SimpleAtoi(spec_parts[1], &cnt);
        std::cout << cnt << std::endl;
        
        std::string bag = absl::StrCat(spec_parts[2], " ", spec_parts[3]);
        std::cout << cnt << bag << std::endl;
        content.push_back(std::make_pair(cnt, bag));
    }

    return content;
}

struct Rule {
    std::string txt;
    std::string bag;
    std::vector<std::pair<int, std::string>> content;
};

Rule ParseRule(const std::string& txt) {
    std::pair<std::string, std::string> parts = absl::StrSplit(txt, "contain");
    trace(parts);

    std::pair<std::string, std::string> bag_parts = absl::StrSplit(parts.first, " ");
    trace(bag_parts);

    std::string bag = absl::StrCat(bag_parts.first, " ", bag_parts.second);
    std::cout << bag << std::endl;

    auto content = ParseContent(parts.second);
    return Rule {txt, bag, content};
}

std::set<std::string> FindOuterAlternatives(
    const std::string &color, 
    std::unordered_map<std::string, std::vector<std::string>> &table) {

    std::set<std::string> outer;
    std::queue<std::string> remaining;
    remaining.push(color);

    while (!remaining.empty()) {
        std::string &cur = remaining.front();
        std::vector<std::string> &parents = table[cur];
        
        for (auto &p : parents) {
            remaining.push(p);
            outer.insert(p);
        }
        remaining.pop();
    }

    return outer;
}

int main() {
    std::cout << "7" << std::endl;
    std::string line;
    std::unordered_map<std::string, std::vector<std::string>> table;

    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
                    
        auto rule = ParseRule(line);
        for (auto &p : rule.content) {
            std::cout << p.first << "|" << p.second << std::endl;
            auto &bs = table[p.second];
            bs.push_back(rule.bag);
        }        
    }

    for (auto &x : table) {
        std::cout << x.first << std::endl;
        for (auto &p : x.second) {
            std::cout << "  " << p << std::endl;
        }
    }

    auto outer = FindOuterAlternatives("shiny gold", table);
    std::cout << "outer:" << outer.size() << std::endl;
    trace(outer);

    return 0; 
}
