#include <algorithm>
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

bool IsValid(int field, Rule rule) {
    if (field >= rule.r0.first && field <= rule.r0.second) {
        //std::cout << "incr:" << field << std::endl;
        return true;
    }

    if (field >= rule.r1.first && field <= rule.r1.second) {
        //std::cout << "incr-2 " << field << std::endl;
        return true;
    }

    return false;
}

// Contains values that are not valid for any field
bool IsValid(std::vector<int> ticket, std::vector<Rule> rules) {
    // a ticket is valid if every field is in the valid range of at least one of the rules.    
    for (int field : ticket) {
        int valid = 0;
        for (auto &rule : rules) {
            if (IsValid(field, rule)) {
                valid++;
            } else {
                std::cout << field << " invalid " << rule.name << std::endl;
            }
        }

        if (valid == 0) {
            return false;
        }
    }

    return true;
}

std::vector<std::set<std::string>> FindValidRules(std::vector<int> &ticket, std::vector<Rule> &rules) {
    std::vector<std::set<std::string>> valid_ticket_fields;
    for (int field : ticket) {
        std::set<std::string> valid;
        for (auto &rule : rules) {
            if (IsValid(field, rule)) {
                valid.insert(rule.name);
            }
        }

        std::cout << field << valid.size() << std::endl;
        valid_ticket_fields.push_back(valid);
    }

    return valid_ticket_fields;
}

int FindStartIndex(std::vector<std::set<std::string>> &per_field_candidates) {
    for (int i = 0; i < per_field_candidates.size(); i++) {
        if (per_field_candidates[i].size() == 1) {
            return i;
        }
    }
    return -1;
}

bool ShouldContinueDeduce(std::vector<std::set<std::string>> &per_field_candidates) {
    for (int i = 0; i < per_field_candidates.size(); i++) {
        if (per_field_candidates[i].size() > 0) {
            return true;
        }
    }
    return false;
}

std::vector<int> Solve(std::vector<std::vector<std::set<std::string>>> tickets) {
    std::vector<int> res;

    std::vector<std::set<std::string>> per_field_candidates;
    int field_count = tickets[0].size();
    for (int i = 0; i < field_count; i++) {
        auto &t0 = tickets[0][i];
        std::set<std::string> full_ix(t0);
        for (int j = 1; j < tickets.size(); j++) {
            std::set<std::string> ix;
            std::set_intersection(full_ix.begin(), full_ix.end(), tickets[j][i].begin(), tickets[j][i].end(),
                std::inserter(ix, ix.begin()));

            full_ix = ix;
        }

        per_field_candidates.push_back(full_ix);
    }

    for (auto &pf : per_field_candidates) {
        for (auto &type : pf) {
            std::cout << type << " ";
        }
        std::cout << std::endl;
    }

    std::unordered_map<int, std::string> field_types;
    while (ShouldContinueDeduce(per_field_candidates)) {
        std::cout << "$$$";

        int start = FindStartIndex(per_field_candidates);
        if (start < 0) {
            std::cout << "invalid result :(" << std::endl;
            exit(1);            
        }
        
        //std::cout << start << "::" << per_field_candidates.size() << "  ";

        std::string type = *(per_field_candidates[start].begin());
        field_types[start] = type;
        std::cout << start << " " << type << std::endl;
        for (int i = 0; i < per_field_candidates.size(); i++) {
            per_field_candidates[i].erase(type);
            //std::cout << "x" << i << " ";
        }
        std::cout << std::endl;
    }

    for (auto &kv : field_types) {
        std::cout << kv.first << " : " << kv.second << std::endl;
        if (kv.second.find("departure") == 0) {
            res.push_back(kv.first);
        }
    }

    return res;
}

int Run(Input &input) {
    auto rules = ParseRules(input.rules);
    std::vector<std::vector<int>> valid;

    for (auto &ticket_txt : input.nearby_tickets) {
        std::cout << ticket_txt << std::endl;
        auto ticket = ParseTicket(ticket_txt);
        if (IsValid(ticket, rules)) {
            valid.push_back(ticket);
        }
    }

    std::cout << "# valid tickets: " << valid.size() << std::endl;
    for (auto &t : valid) {
        for (auto field : t) {
            std::cout << field << " ";
        }
        std::cout << std::endl;
    }
    
    std::vector<std::vector<std::set<std::string>>> ticket_fields;
    for (auto &t : valid) {
        auto field_ix_validated_rules = FindValidRules(t, rules);
        ticket_fields.push_back(field_ix_validated_rules);
    }

    int i = 0;
    for (auto &t : valid) {
        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        auto &valid_fields = ticket_fields[i];
        int j = 0;
        for (auto field : t) {            
            std::cout << field;            
            for (auto &r : valid_fields[j]) {            
                std::cout << r << " ";
            }
            j++;
            std::cout << std::endl;
        }
        i++;
    }

    auto fields = Solve(ticket_fields);
    uint64_t res = 1;
    auto my_ticket = ParseTicket(input.my_ticket);
    std::cout << fields.size();
    for (auto field : fields) {
        res *= my_ticket[field];
    }

    std::cout << "departure x:" << res << std::endl;
    return 0;
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

    // Input ex = {
    //     {"class: 1-3 or 5-7", "row: 6-11 or 33-44", "seat: 13-40 or 45-50"},
    //     "7,1,14",
    //     {"7,3,47", "40,4,50", "55,2,20", "38,6,12"},
    // };

    // int rate2 = Run(ex);
    // std::cout << "example:" << rate2 << std::endl;

    Input ex2 = {
        {"class: 0-1 or 4-19", "row: 0-5 or 8-19", "seat: 0-13 or 16-19"},
        "11,12,13",
        {"3,9,18", "15,1,5", "5,14,9"},
    };

    int rate3 = Run(ex2);
    std::cout << "example 2:" << rate3 << std::endl;

    return 0;
}
