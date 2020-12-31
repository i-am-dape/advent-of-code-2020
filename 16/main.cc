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

struct Input {
    std::vector<std::string> rules;
    std::string my_ticket;
    std::vector<std::string> nearby_tickets;
};

Input ReadInput() {    
    std::string line;

    bool read_rules = true;
    bool read_ticket = false;
    bool read_nearby_tickets = false;

    std::vector<std::string> rules;
    std::string ticket;
    std::vector<std::string> nearby_tickets;

    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        //std::cout << line << std::endl;
        if (line.empty()) {
            if (read_rules) {
                read_rules = false;
                read_nearby_tickets = false;
                read_ticket = true;
            } else if (read_ticket) {
                read_rules = false;
                read_nearby_tickets = true;
                read_ticket = false;
            } else {
                exit(1);
            }

            continue;
        }        

        if (read_rules) {
            rules.push_back(line);
        } else if (read_ticket) {
            if (line == "your ticket:") {
                std::cout << "skip your ticket header" << std::endl;
                continue;
            }
            ticket = line;
        } else if (read_nearby_tickets) {
            if (line == "nearby tickets:") {
                std::cout << "skip nearby tickets header" << std::endl;
                continue;
            }
            nearby_tickets.push_back(line);
        }
    }

    return {rules, ticket, nearby_tickets};
}

struct Rule {
    std::string name;
    std::pair<int, int> r0;
    std::pair<int, int> r1;
};

std::pair<int, int> ParseRange(std::string txt) {
    auto sep_ix = txt.find('-');
    auto first_txt = txt.substr(0, sep_ix);
    //std::cout << first_txt << std::endl;

    auto second_txt = txt.substr(sep_ix+1);
    //std::cout << second_txt << std::endl;

    int first = 0;
    absl::SimpleAtoi(first_txt, &first);

    int second = 0;
    absl::SimpleAtoi(second_txt, &second);

    return std::make_pair(first, second);
}

std::vector<Rule> ParseRules(std::vector<std::string> rules_txt) {
    std::vector<Rule> rules;
    for (auto txt : rules_txt) {
        auto col_ix = txt.find(':');
        auto name = txt.substr(0, col_ix);
        //std::cout << name << std::endl;

        auto or_ix = txt.find("or", col_ix);
        auto r0_txt = txt.substr(col_ix+2, or_ix-3-col_ix);
        auto r1_txt = txt.substr(or_ix+3);
        //std::cout << r0_txt << ":::" << r1_txt << std::endl;

        rules.push_back({name, ParseRange(r0_txt), ParseRange(r1_txt)});
    }

    return rules;
}

std::vector<int> ParseTicket(std::string txt) {
    auto parts = absl::StrSplit(txt, ",");
    std::vector<int> ticket;
    for (auto &part : parts) {
        int tmp;
        absl::SimpleAtoi(part, &tmp);
        ticket.push_back(tmp);
    }
    return ticket;
}

int Run(Input &input) {
    auto rules = ParseRules(input.rules);
    int sum = 0;

    for (auto &ticket_txt : input.nearby_tickets) {
        auto ticket = ParseTicket(ticket_txt);
        for (auto &field : ticket) {
            int valid = 0;
            for (auto &rule : rules) {
                if (field >= rule.r0.first && field <= rule.r0.second) {
                    valid++;                    
                    continue;
                }

                if (field >= rule.r1.first && field <= rule.r1.second) {
                    valid++;
                    continue;                    
                }

                //std::cout << "invalid " << rule.name << std::endl;
                //std::cout << rule.r0.first << " " << rule.r0.second << std::endl;
                //std::cout << rule.r1.first << " " << rule.r1.second << std::endl;
            }

            if (valid == 0) {
                sum += field;
            }
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {    
    absl::ParseCommandLine(argc, argv);
    std::cout << "--- 16 ---" << std::endl;
    auto input = ReadInput();    
    std::cout << "# rules:" << input.rules.size() << std::endl;
    std::cout << "ticket:" << input.my_ticket << std::endl;
    std::cout << "# nearby tickets:" << input.nearby_tickets.size() << std::endl;

    int rate = Run(input);
    std::cout << "real: " << rate << std::endl;

    Input ex = {
        {"class: 1-3 or 5-7", "row: 6-11 or 33-44", "seat: 13-40 or 45-50"},
        "ignored",
        {"7,3,47", "40,4,50", "55,2,20", "38,6,12"},
    };

    rate = Run(ex);
    std::cout << "example:" << rate << std::endl;

    return 0;
}
