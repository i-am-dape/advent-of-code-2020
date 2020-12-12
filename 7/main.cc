#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/numbers.h"

void trace(std::vector<std::string> &words) {
    for (std::string &w : words) {
        std::cout << w << "|";
    }
    std::cout << std::endl;
}

void trace(std::pair<std::string, std::string> &words) {
    std::cout << words.first << std::endl;
    std::cout << words.second << std::endl;
}

std::vector<std::pair<int, std::string>> parseContent(std::string& txt_content) {
    std::vector<std::string> parts = absl::StrSplit(txt_content, ",");
    trace(parts);
    std::vector<std::pair<int, std::string>> content;
    
    for (auto bag_spec : parts) {
        std::vector<std::string> spec_parts = absl::StrSplit(bag_spec, " ");
        trace(spec_parts);

        std::string txt_cnt = spec_parts[1];
        std::cout << "-----" << txt_cnt << std::endl;
        int cnt = 0;
        absl::SimpleAtoi(txt_cnt, &cnt);

        //std::string p0 = spec_parts[2];
        //std::string p1 = spec_parts[3];
        //std::string bag = absl::StrCat(p0, p1);
        //std::cout << cnt << bag << std::endl;
        // content.push_back(std::make_pair(cnt, bag));
    }

    return content;
}

int main() {
    std::cout << "7" << std::endl;
    std::string line;
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> table;

    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        std::cout << line << std::endl;

        std::pair<std::string, std::string> parts = absl::StrSplit(line, "contain");
        trace(parts);
        std::pair<std::string, std::string> bag_parts = absl::StrSplit(parts.first, " ");
        trace(bag_parts);

        std::string bag = absl::StrCat(bag_parts.first, " ", bag_parts.second);
        std::cout << bag << std::endl;

        table[bag] = parseContent(parts.second);


        //std::string bag = absl::StrCat(bag_parts[0], " ", bag_parts[0]);
        //std::cout << bag << std::endl;

        // auto content_parts = absl::StrSplit(parts[1], ",");
        // trace(content_parts);
    }

    return 0; 
}
