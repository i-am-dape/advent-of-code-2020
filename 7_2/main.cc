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

int CountTotalContent(const std::string &bag, std::unordered_map<std::string, Rule> &table) {
    Rule &r = table[bag];
    int total = 0;
    for (auto &content : r.content) {
        total += content.first + (content.first * CountTotalContent(content.second, table));
        std::cout << total << std::endl;
    }

    return total;
}


int main() {
    std::cout << "7" << std::endl;
    std::string line;
    std::unordered_map<std::string, Rule> table;
    size_t count = 0;

    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;
        if (line.empty()) {
            continue;
        }
                    
        count++;
        auto rule = ParseRule(line);
        table[rule.bag] = rule;        
    }

    std::cout << "lines|table" << count << "|" << table.size() << std::endl;
    if (count != table.size()) {
        return 1;
    }

    int total = CountTotalContent("shiny gold", table);
    std::cout << "total: " << total << std::endl;

    return 0; 
}
